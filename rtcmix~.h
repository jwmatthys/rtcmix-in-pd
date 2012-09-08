#define VERSION "0.01"
#define RTcmixVERSION "RTcmix-pd-4.0.1.6"

#include "m_pd.h"

int dylibincr = 0;
// for where the rtcmix-dylibs folder is located
char mpathname[MAXPDSTRING];

#define MAX_INPUTS 20    //switched to 8 for sanity sake (have to contruct manually)
#define MAX_OUTPUTS 20	//do we ever need more than 8? we'll cross that bridge when we come to it
#define MAX_SCRIPTS 20	//how many scripts can we store internally
#define MAXSCRIPTSIZE 16384

// JWM: since Tk's openpanel & savepanel both use callback(),
// I use a flag to indicate whether we're loading or writing
#define RTcmixREADFLAG 0
#define RTcmixWRITEFLAG 1
#define CRFLAG 1 // Flag for carriage return translation for binbufs

/*** RTcmix stuff ---------------------------------------------------------------------------***/

typedef int (*rtcmixmainFunctionPtr)();
typedef int (*pd_rtsetparamsFunctionPtr)(float sr, int nchans, int vecsize, float *pd_inbuf, float *pd_outbuf, char *theerror);
typedef int (*parse_scoreFunctionPtr)();
typedef void (*pullTraverseFunctionPtr)();
typedef int (*check_bangFunctionPtr)();
typedef int (*check_valsFunctionPtr)(float *thevals);
typedef double (*parse_dispatchFunctionPtr)(char *cmd, double *p, int n_args, void *retval);
typedef int (*check_errorFunctionPtr)();
typedef void (*pfield_setFunctionPtr)(int inlet, float pval);
typedef void (*buffer_setFunctionPtr)(char *bufname, float *bufstart, int nframes, int nchans, int modtime);
typedef int (*flushPtr)();


/*** PD FUNCTIONS ---------------------------------------------------------------------------***/

static t_class *rtcmix_class;

typedef struct _rtcmix
{
  //header
  t_object x_obj;

  //variables specific to this object
  float srate;                                        //sample rate
  short num_inputs, num_outputs;       //number of inputs and outputs
  short num_pinlets;				// number of inlets for dynamic PField control
  float in[MAX_INPUTS];			// values received for dynamic PFields
  float in_connected[MAX_INPUTS]; //booleans: true if signals connected to the input in question
  //we use this "connected" boolean so that users can connect *either* signals or floats
  //to the various inputs; sometimes it's easier just to have floats, but other times
  //it's essential to have signals.... but we have to know.
  //JWM: We'll see if this works in Pd
  t_outlet *outpointer;

  /******* RTcmix stuff *******/

  rtcmixmainFunctionPtr rtcmixmain;
  pd_rtsetparamsFunctionPtr pd_rtsetparams;
  parse_scoreFunctionPtr parse_score;
  pullTraverseFunctionPtr pullTraverse;
  check_bangFunctionPtr check_bang;
  check_valsFunctionPtr check_vals;
  parse_dispatchFunctionPtr parse_dispatch;
  check_errorFunctionPtr check_error;
  pfield_setFunctionPtr pfield_set;
  buffer_setFunctionPtr buffer_set;
  flushPtr flush;

  // for the load of rtcmixdylibN.so
  int dylibincr;
  void *rtcmixdylib;
  // for the full path to the rtcmixdylib.so file
  char *pathname; //[MAXPDSTRING];

  // space for these malloc'd in rtcmix_dsp()
  float *pd_outbuf;
  float *pd_inbuf;

  // script buffer pointer for large binbuf restores
  char *restore_buf_ptr;

  // for the rtmix_var() and rtcmix_varlist() $n variable scheme
#define NVARS 9
  float var_array[NVARS];
  int var_set[NVARS];

  // stuff for check_vals
#define MAXDISPARGS 1024 // from RTcmix H/maxdispargs.h
  float thevals[MAXDISPARGS];
  t_atom valslist[MAXDISPARGS];

  // buffer for error-reporting
  char theerror[MAXPDSTRING];
  /*
  // editor stuff
  // JWM: TODO: will try to implement custom editor later
  t_object m_obj;
  t_object *m_editor;
  char *rtcmix_script[MAX_SCRIPTS], s_name[MAX_SCRIPTS][256];
  long rtcmix_script_len[MAX_SCRIPTS];
  short current_script, path[MAX_SCRIPTS];
  */
  // JWM: changing to binbufs for all internal scores
  t_binbuf *rtcmix_script[MAX_SCRIPTS];
  short current_script, rw_flag;

  // JWM : canvas objects for callback addressing
  t_canvas *x_canvas;
  t_symbol *canvas_path;
  t_symbol *x_s;

  // for flushing all events on the queue/heap (resets to new ones inside RTcmix)

  int flushflag;
  t_float f;
} t_rtcmix;


/*** FUNCTION PROTOTYPES ---------------------------------------------------------------------------***/

//setup funcs; this probably won't change, unless you decide to change the number of
//args that the user can input, in which case rtcmix_new will have to change
void rtcmix_tilde_setup(void);
void *rtcmix_tilde_new(t_symbol *s, int argc, t_atom *argv);
void rtcmix_dsp(t_rtcmix *x, t_signal **sp, short *count);
t_int *rtcmix_perform(t_int *w);
void rtcmix_free(t_rtcmix *x);
static void load_dylib(t_rtcmix* x, short ni, short no, short npi);

//for getting floats, ints or bangs at inputs
void rtcmix_float(t_rtcmix *x, double f);
void rtcmix_bang(t_rtcmix *x);

//for custom messages
void rtcmix_version(t_rtcmix *x);
void rtcmix_text(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
//void rtcmix_dotext(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_badquotes(char *cmd, char *buf); // this is to check for 'split' quoted params, called in rtcmix_dotext
void rtcmix_rtcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_dortcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_var(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_varlist(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_bufset(t_rtcmix *x, t_symbol *s);
void rtcmix_flush(t_rtcmix *x);

//for the text editor
void rtcmix_edclose (t_rtcmix *x, char **text, long size);
void rtcmix_dblclick(t_rtcmix *x);
void rtcmix_goscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_dogoscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_openscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_setscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_read(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_write(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_writeas(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_dowrite(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result);

//for binbuf storage of scripts
void rtcmix_save(t_rtcmix *x, void *w);
void rtcmix_callback(t_rtcmix *x, t_symbol *s);
