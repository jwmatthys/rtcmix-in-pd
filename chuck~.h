#include "m_pd.h"

#define CHUCKEXTERNALNAME "chuck~.pd_linux"

int dylibincr = 0;
// for where the chuck-dylibs folder is located
char mpathname[MAXPDSTRING];

#define MAX_INPUTS 20    //switched to 8 for sanity sake (have to contruct manually)
#define MAX_OUTPUTS 20	//do we ever need more than 8? we'll cross that bridge when we come to it
#define MAX_SCRIPTS 20	//how many scripts can we store internally
#define MAXSCRIPTSIZE 16384

// JWM: since Tk's openpanel & savepanel both use callback(),
// I use a flag to indicate whether we're loading or writing
#define ChuckREADFLAG 0
#define ChuckWRITEFLAG 1

// JWM: since Pd has no decent internal text editor, I use an external application built in python
// which reads temp.sco, modifies it, and rewrites it. If <modified>, chuck_goscript() rereads the
// temp file so we're sure to have the most recent edit. Modified also supresses some unnecessary
// messages in the doread and dosave functions.
#define UNEDITED_SCRIPT 0
#define EDITED_SCRIPT 1


/*** Chuck stuff ---------------------------------------------------------------------------***/

typedef int (*chuckmainFunctionPtr)(int vecsize, int srate);
typedef void (*pull_cb2FunctionPtr)(float *maxmsp_outbuf, int vecsize);
typedef int (*parse_scoreFunctionPtr)(char *buf);
typedef int (*check_bangFunctionPtr)();
typedef int (*check_valsFunctionPtr)(float *thevals);
typedef int (*inlet_setFunctionPtr)(int inlet, float pval, int ninlets);



/*** PD FUNCTIONS ---------------------------------------------------------------------------***/

static t_class *chuck_class;

typedef struct _chuck
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

  /******* Chuck stuff *******/
  chuckmainFunctionPtr chuckmain;
  pull_cb2FunctionPtr pull_cb2;
  parse_scoreFunctionPtr parse_score;
  check_bangFunctionPtr check_bang;
  check_valsFunctionPtr check_vals;
  inlet_setFunctionPtr inlet_set;

  // for the load of chuckdylibN.so
  int dylibincr;
  void *chuckdylib;
  // for the full path to the chuckdylib.so file
  char pathname[MAXPDSTRING];

  // space for these malloc'd in chuck_dsp()
  float *pd_outbuf;
  float *pd_inbuf;

  // script buffer pointer for large binbuf restores
  char *restore_buf_ptr;

  // for the rtmix_var() and chuck_varlist() $n variable scheme
#define NVARS 9
  float *var_array;
  short *var_set;

  // stuff for check_vals
#define MAXDISPARGS 1024 // from Chuck H/maxdispargs.h
  float thevals[MAXDISPARGS];
  t_atom valslist[MAXDISPARGS];

  // buffer for error-reporting
  char theerror[MAXPDSTRING];
  short editor_flag;

  // editor stuff
  char **chuck_script;
  char script_name[MAX_SCRIPTS][256];
  t_int script_size[MAX_SCRIPTS];
  t_int current_script, rw_flag;
  char tempfile_path[MAXPDSTRING];
  // JWM : canvas objects for callback addressing
  t_canvas *x_canvas;
  t_symbol *canvas_path;
  t_symbol *x_s;

  // for flushing all events on the queue/heap (resets to new ones inside Chuck)

  int flushflag;
  t_float f;
} t_chuck;


/*** FUNCTION PROTOTYPES ---------------------------------------------------------------------------***/

//setup funcs; this probably won't change, unless you decide to change the number of
//args that the user can input, in which case chuck_new will have to change
void chuck_tilde_setup(void);
void *chuck_tilde_new(t_symbol *s, int argc, t_atom *argv);
void chuck_dsp(t_chuck *x, t_signal **sp); //, short *count);
t_int *chuck_perform(t_int *w);
void chuck_free(t_chuck *x);
static void load_dylib(t_chuck* x);
static void chuck_dlopen_and_errorcheck(t_chuck *x);

// JWM: for getting bang at left inlet only
void chuck_bang(t_chuck *x);
void chuck_float(t_chuck *x, t_float scriptnum);
// JWM: float inlets are rewritten (in a horrible embarassing way) below


//for custom messages
void chuck_version(t_chuck *x);
void chuck_text(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
//void chuck_dotext(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_badquotes(char *cmd, char *buf); // this is to check for 'split' quoted params, called in chuck_dotext
void chuck_chuck(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_dochuck(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_var(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_varlist(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_bufset(t_chuck *x, t_symbol *s);
void chuck_flush(t_chuck *x);

//for the text editor
void chuck_dblclick(t_chuck *x);
void chuck_goscript(t_chuck *x, t_float s);
static void chuck_openeditor(t_chuck *x);
void chuck_setscript(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_read(t_chuck *x, t_symbol *s, short argc, t_atom *argv);
void chuck_save(t_chuck *x);
void chuck_saveas(t_chuck *x);
void chuck_callback(t_chuck *x, t_symbol *s);
static void chuck_doread(t_chuck *x, char* filename);
static void chuck_dosave(t_chuck *x, char* filename);

// for receiving pfields from inlets
static void chuck_float_inlet(t_chuck *x, short inlet, t_float f);
static void chuck_inletp0(t_chuck *x, t_float f);
static void chuck_inletp1(t_chuck *x, t_float f);
static void chuck_inletp2(t_chuck *x, t_float f);
static void chuck_inletp3(t_chuck *x, t_float f);
static void chuck_inletp4(t_chuck *x, t_float f);
static void chuck_inletp5(t_chuck *x, t_float f);
static void chuck_inletp6(t_chuck *x, t_float f);
static void chuck_inletp7(t_chuck *x, t_float f);
static void chuck_inletp8(t_chuck *x, t_float f);
static void chuck_inletp9(t_chuck *x, t_float f);
