// rtcmix~ v 0.01, Joel Matthys (8/2012) (Linux, Pd support), based on:
// rtcmix~ v 1.81, Brad Garton (2/2011) (OS 10.5/6, Max5 support)
// uses the RTcmix bundled executable lib, now based on RTcmix-4.0.1.6
// see http://music.columbia.edu/cmc/RTcmix for more info
//
// see rtcmix~.c in BGG's rtcmix~ for Max/MSP thank yous & changelog

#define VERSION "0.01"
#define RTcmixVERSION "RTcmix-pd-4.0.1.6"

// JWM: since Tk's openpanel & savepanel both use callback(),
// I use a flag to indicate whether we're loading or writing
#define RTcmixREADFLAG 0
#define RTcmixWRITEFLAG 1
// Flag for carriage return translation for binbufs
#define CRFLAG 1 // 0 for no CR, 1 for semis?

// JWM - Pd headers
#include "m_pd.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h> // JWM: for getcwd()

// JWM WHAT A ROCKSTAR Brad is! Reverting to dlopen for linux
// BGG kept the dlopen() stuff in for future use
// for the dlopen() loader
#include <dlfcn.h>
int dylibincr = 0;
// for where the rtcmix-dylibs folder is located
char mpathname[MAXPDSTRING];

#define MAX_INPUTS 20    //switched to 8 for sanity sake (have to contruct manually)
#define MAX_OUTPUTS 20	//do we ever need more than 8? we'll cross that bridge when we come to it
#define MAX_SCRIPTS 20	//how many scripts can we store internally
#define MAXSCRIPTSIZE 16384

/******* RTcmix stuff *******/
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


/*** PD FUNCTIONS ***/
static t_class *rtcmix_class;

typedef struct _rtcmix
{
  //header
  t_object x_obj;

  //variables specific to this object
  float srate;                                        //sample rate
  long num_inputs, num_outputs;       //number of inputs and outputs
  long num_pinlets;				// number of inlets for dynamic PField control
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
  char pathname[MAXPDSTRING];

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

  // editor stuff
  // JWM: TODO: will try to implement custom editor later
  /*
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

/****PROTOTYPES****/

//setup funcs; this probably won't change, unless you decide to change the number of
//args that the user can input, in which case rtcmix_new will have to change
void rtcmix_tilde_setup(void);
static void *rtcmix_tilde_new(t_symbol *s, int argc, t_atom *argv);
void rtcmix_dsp(t_rtcmix *x, t_signal **sp, short *count);
t_int *rtcmix_perform(t_int *w);
static void rtcmix_free(t_rtcmix *x);
static void load_dylib(t_rtcmix* x);

//for getting floats, ints or bangs at inputs
static void rtcmix_float(t_rtcmix *x, double f);
//void rtcmix_int(t_rtcmix *x, int i);
static void rtcmix_bang(t_rtcmix *x);
// JWM: removed this since there's no defer in Pd
//void rtcmix_dobangout(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv); // for the defer

//for custom messages
static void rtcmix_version(t_rtcmix *x);
static void rtcmix_text(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_dotext(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_badquotes(char *cmd, char *buf); // this is to check for 'split' quoted params, called in rtcmix_dotext
static void rtcmix_rtcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_dortcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_var(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_varlist(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_bufset(t_rtcmix *x, t_symbol *s);
static void rtcmix_flush(t_rtcmix *x);

//for the text editor
static void rtcmix_edclose (t_rtcmix *x, char **text, long size);
static void rtcmix_dblclick(t_rtcmix *x);
static void rtcmix_goscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_dogoscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_openscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_setscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_read(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_write(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_writeas(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_dowrite(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv);
static void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result);

//for binbuf storage of scripts
static void rtcmix_save(t_rtcmix *x, void *w);
static void rtcmix_callback(t_rtcmix *x, t_symbol *s);

// JWM: TODO - attach to table?
//t_symbol *ps_buffer; // for [buffer~]


/****FUNCTIONS****/

//primary Pd funcs
void rtcmix_tilde_setup(void)
{
  rtcmix_class = class_new (gensym("rtcmix~"),
                            (t_newmethod)rtcmix_tilde_new,
                            (t_method)rtcmix_free,
                            sizeof(t_rtcmix),
                            0, A_GIMME, 0);

  //the A_DEFLONG arguments give us the object arguments for the user to set number of ins/outs, etc.
  // JWM: Pd has no long type; presumably it becomes A_DEFFLOAT
  //change these if you want different user args
  //setup((struct messlist **)&rtcmix_class, (method)rtcmix_new, (method)rtcmix_free, (short)sizeof(t_rtcmix), 0L, A_DEFFLOAT, A_DEFFLOAT, 0);

  //standard messages; don't change these
  //addmess((method)rtcmix_assist,"assist", A_CANT,0);
  CLASS_MAINSIGNALIN(rtcmix_class, t_rtcmix, f);

  //addmess((method)rtcmix_dsp, "dsp", A_CANT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_dsp, gensym("dsp"), 0);

  //our own messages
  /*addmess((method)rtcmix_text, "text", A_GIMME, 0);
  addmess((method)rtcmix_rtcmix, "rtcmix", A_GIMME, 0);
  addmess((method)rtcmix_var, "var", A_GIMME, 0);
  addmess((method)rtcmix_varlist, "varlist", A_GIMME, 0);
  addmess((method)rtcmix_bufset, "bufset", A_SYMBOL, 0);
  addmess((method)rtcmix_flush, "flush", 0);
  addmess((method)rtcmix_version, "version", 0);*/
  class_addmethod(rtcmix_class,(t_method)rtcmix_version, gensym("version"), 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_rtcmix, gensym("rtcmix"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_var, gensym("var"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_varlist, gensym("varlist"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_bufset, gensym("bufset"), A_SYMBOL, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_flush, gensym("flush"), 0);
  /*
  //so we know what to do with floats that we receive at the inputs
  addfloat((method)rtcmix_float);
  addint((method)rtcmix_int);
  addbang((method)rtcmix_bang);
  */
  class_addlist(rtcmix_class, rtcmix_text);
  class_addfloat(rtcmix_class, rtcmix_float);
  // trigger scripts
  class_addbang(rtcmix_class, rtcmix_bang);

  class_addmethod(rtcmix_class,(t_method)rtcmix_read, gensym("read"), A_GIMME, 0);
  //for the text editor and scripts
  //JWM - TODO
  /*addmess ((method)rtcmix_edclose, "edclose", A_CANT, 0);
  addmess((method)rtcmix_dblclick,	"dblclick",	A_CANT, 0);
  addmess((method)rtcmix_goscript, "goscript", A_GIMME, 0);
  addmess((method)rtcmix_openscript, "openscript", A_GIMME, 0);
  addmess((method)rtcmix_setscript, "setscript", A_GIMME, 0);
  addmess((method)rtcmix_read, "read", A_GIMME, 0);
  addmess ((method)rtcmix_okclose, "okclose", A_CANT, 0);
  addmess((method)rtcmix_write, "savescript", A_GIMME, 0);
  addmess((method)rtcmix_writeas, "savescriptas", A_GIMME, 0);*/

  // binbuf storage
  //addmess((method)rtcmix_save, "save", A_CANT, 0);
  //addmess((method)rtcmix_restore, "restore", A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_save, gensym("save"), A_CANT, 0);

  class_addmethod(rtcmix_class,(t_method)rtcmix_callback, gensym("callback"), A_SYMBOL, 0);
}


//this gets called when the object is created; everytime the user types in new args, this will get called
//void rtcmix_new(long num_inoutputs, long num_additional)
static void *rtcmix_tilde_new(t_symbol *s, int argc, t_atom *argv)
{
  // creates the object
  t_rtcmix *x = (t_rtcmix *)pd_new(rtcmix_class);
  load_dylib(x);
  int i;

  short num_inoutputs = 1;
  short num_additional = 0;
  switch(argc)
    {
    default:
    case 2:
      num_additional = atom_getint(argv+1);
    case 1:
      num_inoutputs = atom_getint(argv);
    }
  post("creating %d inlets and outlets and %d additional inlets",num_inoutputs,num_additional);

  if (num_inoutputs < 1) num_inoutputs = 1; // no args, use default of 1 channel in/out
  if ((num_inoutputs + num_additional) > MAX_INPUTS)
    {
      num_inoutputs = 1;
      num_additional = 0;
      error("sorry, only %d total inlets are allowed!", MAX_INPUTS);
      //return(NULL);
    }

  x->num_inputs = num_inoutputs;
  x->num_outputs = num_inoutputs;
  x->num_pinlets = num_additional;

  // setup up inputs and outputs, for audio inputs
  //dsp_setup((t_object *)x, x->num_inputs + x->num_pinlets);

  // JWM: hoo boy this looks ugly, but
  // I don't think you can specify an arbitrary number of inlets in Pd
  for (i=0; i < x->num_inputs-1; i++)
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

  for (i=0; i< x->num_pinlets; i++)
    floatinlet_new(&x->x_obj, &x->in[i]);

  for (i = 0; i < x->num_outputs; i++)
    {
      // outputs, right-to-left
      outlet_new(&x->x_obj, gensym("signal"));
    }

  // initialize some variables; important to do this!
  for (i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      x->in[i] = 0.;
      x->in_connected[i] = 0;
    }

  for (i=0; i<MAX_SCRIPTS; i++)
    {
      x->rtcmix_script[i]=binbuf_new();
    }
  x->current_script = 0;

  //ps_buffer = gensym("buffer~"); // for [buffer~]

  // ho ho!
  post("rtcmix~ -- RTcmix music language, v. %s (%s)", VERSION, RTcmixVERSION);

  // set up for the variable-substitution scheme
  for(i = 0; i < NVARS; i++)
    {
      x->var_set[i] = 0;
      x->var_array[i] = 0.0;
    }

  // the text editor
  x->current_script = 0;
  x->rw_flag = RTcmixREADFLAG;

  x->outpointer = outlet_new(&x->x_obj, &s_bang);

  // This nasty looking stuff is to bind the object's ID
  // to x_s to facilitate callbacks
  char buf[50];
  sprintf(buf, "d%lx", (t_int)x);
  x->x_s = gensym(buf);
  pd_bind(&x->x_obj.ob_pd, x->x_s);
  x->x_canvas = canvas_getcurrent();
  x->canvas_path = malloc(MAXPDSTRING);
  x->canvas_path = canvas_getdir(x->x_canvas);


  x->flushflag = 0; // [flush] sets flag for call to x->flush() in rtcmix_perform() (after pulltraverse completes)
  return (x);
}

static void load_dylib(t_rtcmix* x)
{
  // using Pd's open_via_path to find rtcmix~, and from there rtcmixdylib.so
  char *temp_path, *pathptr;
  temp_path = malloc(MAXPDSTRING);
  int fd = -1;
  fd = open_via_path(".","rtcmix~.pd_linux","",temp_path, &pathptr, MAXPDSTRING,1);
  if (fd < 0)
    error ("open_via_path() failed!");
  else
    {
      sprintf(mpathname,"%s/dylib/",temp_path);
    }
  free(temp_path);

  // BGG kept this in for the dlopen() stuff

  //zero out the struct, to be careful (takk to jkclayton)
  if (x)
    {
      unsigned int j;
      for(j=sizeof(t_object);j<sizeof(t_rtcmix);j++)
        ((char *)x)[j]=0;
    }

  // these are the entry function pointers in to the rtcmixdylib.so lib
  x->rtcmixmain = NULL;
  x->pd_rtsetparams = NULL;
  x->pullTraverse = NULL;
  x->parse_score = NULL;
  x->check_bang = NULL;
  x->check_vals = NULL;
  x->parse_dispatch = NULL;
  x->check_error = NULL;
  x->pfield_set = NULL;
  x->buffer_set = NULL;
  x->flush = NULL;

  //constrain number of inputs and outputs

  x->dylibincr = dylibincr++; // keep track of rtcmixdylibN.so for copy/load

  // full path to the rtcmixdylib.so file
  sprintf(x->pathname, "%srtcmixdylib%d.so", mpathname, x->dylibincr);


  char *cp_command = malloc(MAXPDSTRING); // should probably be malloc'd

  // ok, this is fairly insane.  To guarantee a fully-isolated namespace with dlopen(), we need
  // a totally *unique* dylib, so we copy this.  Deleted in rtcmix_free() below
  // RTLD_LOCAL doesn't do it all - probably the global vars in RTcmix
  sprintf(cp_command, "cp \"%srtcmixdylib.so\" \"%s\"",mpathname,x->pathname);
  if (system(cp_command)) error("error creating unique dylib copy");

  free(cp_command);

  // load the dylib
  x->rtcmixdylib = dlopen(x->pathname, RTLD_NOW | RTLD_LOCAL);
  // JWM: for safety (and debugging), added check for load error
  if (!x->rtcmixdylib)
    {
      error("dlopen error loading dylib");
    }

  // find the main entry to be sure we're cool...
  x->rtcmixmain = dlsym(x->rtcmixdylib, "rtcmixmain");
  if (x->rtcmixmain)	x->rtcmixmain();
  else error("rtcmix~ could not call rtcmixmain()");

  x->pd_rtsetparams = dlsym(x->rtcmixdylib, "pd_rtsetparams");
  if (!(x->pd_rtsetparams))
    error("rtcmix~ could not find pd_rtsetparams()");

  x->parse_score = dlsym(x->rtcmixdylib, "parse_score");
  if (!(x->parse_score))
    error("rtcmix~ could not find parse_score()");

  x->pullTraverse = dlsym(x->rtcmixdylib, "pullTraverse");
  if (!(x->pullTraverse))
    error("rtcmix~ could not find pullTraverse()");

  x->check_bang = dlsym(x->rtcmixdylib, "check_bang");
  if (!(x->check_bang))
    error("rtcmix~ could not find check_bang()");

  x->check_vals = dlsym(x->rtcmixdylib, "check_vals");
  if (!(x->check_vals))
    error("rtcmix~ could not find check_vals()");

  x->parse_dispatch = dlsym(x->rtcmixdylib, "parse_dispatch");
  if (!(x->parse_dispatch))
    error("rtcmix~ could not find parse_dispatch()");

  x->check_error = dlsym(x->rtcmixdylib, "check_error");
  if (!(x->check_error))
    error("rtcmix~ could not find check_error()");

  x->pfield_set = dlsym(x->rtcmixdylib, "pfield_set");
  if (!(x->pfield_set))
    error("rtcmix~ could not find pfield_set()");

  x->buffer_set = dlsym(x->rtcmixdylib, "buffer_set");
  if (!(x->buffer_set))
    error("rtcmix~ could not find buffer_set()");

  x->flush = dlsym(x->rtcmixdylib, "flush_sched");
  if (!(x->flush))
    error("rtcmix~ could not find flush_sched()");
}

//this gets called everytime audio is started; even when audio is running, if the user
//changes anything (like deletes a patch cord), audio will be turned off and
//then on again, calling this func.
//this adds the "perform" method to the DSP chain, and also tells us
//where the audio vectors are and how big they are
void rtcmix_dsp(t_rtcmix *x, t_signal **sp, short *count)
{
  t_int dsp_add_args [MAX_INPUTS + MAX_OUTPUTS + 2];
  int i;

  // RTcmix vars

  // these are the entry function pointers in to the rtcmixdylib.so lib
  x->rtcmixmain = NULL;
  x->pd_rtsetparams = NULL;
  x->pullTraverse = NULL;
  x->parse_score = NULL;
  x->check_bang = NULL;
  x->check_vals = NULL;
  x->parse_dispatch = NULL;
  x->check_error = NULL;
  x->pfield_set = NULL;
  x->buffer_set = NULL;
  x->flush = NULL;

  // set sample rate
  x->srate = sp[0]->s_sr;

  // check to see if there are signals connected to the various inputs
  for(i = 0; i < (x->num_inputs + x->num_pinlets); i++) x->in_connected[i] = count[i];

  // construct the array of vectors and stuff
  dsp_add_args[0] = (t_int)x; //the object itself
  for(i = 0; i < (x->num_inputs + x->num_pinlets + x->num_outputs); i++)
    { //pointers to the input and output vectors
      dsp_add_args[i+1] = (t_int)sp[i]->s_vec;
    }

  dsp_add_args[x->num_inputs + x->num_pinlets + x->num_outputs + 1] = (t_int)sp[0]->s_n; //pointer to the vector size

  dsp_addv(rtcmix_perform, (x->num_inputs + x->num_pinlets + x->num_outputs + 2), dsp_add_args); //add them to the signal chain

  // reload, this reinits the RTcmix queue, etc.
  dlclose(x->rtcmixdylib);

  // load the dylib
  x->rtcmixdylib = dlopen(x->pathname,  RTLD_NOW | RTLD_LOCAL);
  if (!x->rtcmixdylib)
    {
      error("dlopen error loading dylib");
    }

  // find the main entry to be sure we're cool...
  x->rtcmixmain = dlsym(x->rtcmixdylib, "rtcmixmain");
  if (x->rtcmixmain)	x->rtcmixmain();
  else error("rtcmix~ could not call rtcmixmain()");

  x->pd_rtsetparams = dlsym(x->rtcmixdylib, "pd_rtsetparams");
  if (!(x->pd_rtsetparams))
    error("rtcmix~ could not find pd_rtsetparams()");

  x->parse_score = dlsym(x->rtcmixdylib, "parse_score");
  if (!(x->parse_score))
    error("rtcmix~ could not find parse_score()");

  x->pullTraverse = dlsym(x->rtcmixdylib, "pullTraverse");
  if (!(x->pullTraverse))
    error("rtcmix~ could not find pullTraverse()");

  x->check_bang = dlsym(x->rtcmixdylib, "check_bang");
  if (!(x->check_bang))
    error("rtcmix~ could not find check_bang()");

  x->check_vals = dlsym(x->rtcmixdylib, "check_vals");
  if (!(x->check_vals))
    error("rtcmix~ could not find check_vals()");

  x->parse_dispatch = dlsym(x->rtcmixdylib, "parse_dispatch");
  if (!(x->parse_dispatch))
    error("rtcmix~ could not find parse_dispatch()");

  x->check_error = dlsym(x->rtcmixdylib, "check_error");
  if (!(x->check_error))
    error("rtcmix~ could not find check_error()");

  x->pfield_set = dlsym(x->rtcmixdylib, "pfield_set");
  if (!(x->pfield_set))
    error("rtcmix~ could not find pfield_set()");

  x->buffer_set = dlsym(x->rtcmixdylib, "buffer_set");
  if (!(x->buffer_set))
    error("rtcmix~ could not find buffer_set()");

  x->flush = dlsym(x->rtcmixdylib, "flush_sched");
  if (!(x->flush))
    error("rtcmix~ could not find flush_sched()");

  // allocate the RTcmix i/o transfer buffers
  x->pd_inbuf = malloc(sizeof(float) * sp[0]->s_n * x->num_inputs);
  x->pd_outbuf = malloc(sizeof(float) * sp[0]->s_n * x->num_outputs);

  // zero out these buffers for UB
  for (i = 0; i < (sp[0]->s_n * x->num_inputs); i++) x->pd_inbuf[i] = 0.0;
  for (i = 0; i < (sp[0]->s_n * x->num_outputs); i++) x->pd_outbuf[i] = 0.0;

  if (x->pd_rtsetparams)
    {
      x->pd_rtsetparams(x->srate, x->num_outputs, sp[0]->s_n, x->pd_inbuf, x->pd_outbuf, x->theerror);
    }
}


//this is where the action is
//we get vectors of samples (n samples per vector), process them and send them out
t_int *rtcmix_perform(t_int *w)
{
  t_rtcmix *x = (t_rtcmix *)(w[1]);

  float *in[MAX_INPUTS];          //pointers to the input vectors
  float *out[MAX_OUTPUTS];	//pointers to the output vectors

  long n = w[x->num_inputs + x->num_pinlets + x->num_outputs + 2];	//number of samples per vector

  //random local vars
  int i, j, k;

  // stuff for check_vals() and check_error()
  int valflag;
  int errflag;


  //check to see if we should skip this routine if the patcher is "muted"
  //i also setup of "power" messages for expensive objects, so that the
  //object itself can be turned off directly. this can be convenient sometimes.
  //in any case, all audio objects should have this
  // BGG -- need to set up to zero the buffers, though
  // JWM -- removed for Pd
  //if (x->x_obj.z_disabled) goto out;

  //check to see if we have a signal or float message connected to input
  //then assign the pointer accordingly
  for (i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      in[i] = x->in_connected[i] ? (float *)(w[i+2]) : &x->in[i];
    }

  //assign the output vectors
  for (i = 0; i < x->num_outputs; i++)
    {
      out[i] = (float *)(w[x->num_inputs+x->num_pinlets+i+2]);
    }

  j = 0;
  k = 0;
  while (n--)
    {	//this is where the action happens.....

      for(i = 0; i < x->num_inputs; i++)
        if (x->in_connected[i])
          (x->pd_inbuf)[k++] = *in[i]++;
        else
          (x->pd_inbuf)[k++] = *in[i];

      for(i = 0; i < x->num_outputs; i++)
        *out[i]++ = (x->pd_outbuf)[j++];

    }

  // RTcmix stuff
  // this drives the RTcmix sample-computing engine
  x->pullTraverse();

  // look for a pending bang from MAXBANG()
  if (x->check_bang() == 1) // JWM: no defer in Pd, and BGG says unnecessary anyway
    {
      //defer_low(x, (method)rtcmix_dobangout, (t_symbol *)NULL, 0, (t_atom *)NULL);
      outlet_bang(x->outpointer);
    }
  // look for pending vals from MAXMESSAGE()
  valflag = x->check_vals(x->thevals);

  // BGG -- I should probably defer this one and the error posts also.  So far not a problem...
  if (valflag > 0)
    {
      if (valflag == 1) outlet_float(x->outpointer, (double)(x->thevals[0]));
      else
        {
          for (i = 0; i < valflag; i++) SETFLOAT((x->valslist)+i, x->thevals[i]);
          outlet_list(x->outpointer, 0L, valflag, x->valslist);
        }
    }

  errflag = x->check_error();
  if (errflag != 0)
    {
      if (errflag == 1) post("RTcmix: %s", x->theerror);
      else error("RTcmix: %s", x->theerror);
    }

  // reset queue and heap if signalled
  if (x->flushflag == 1)
    {
      x->flush();
      x->flushflag = 0;
    }

  //return a pointer to the next object in the signal chain.
  return w + x->num_inputs + x->num_pinlets + x->num_outputs + 3;
}


// the deferred bang output
// JWM: obsolete since no defer; called directly
/*void rtcmix_dobangout(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
  {
  outlet_bang(x->outpointer);
  }*/

// here's my free function
static void rtcmix_free(t_rtcmix *x)
{
  // BGG kept dlopen() stuff in in case NSLoad gets dropped
  char rm_command[MAXPDSTRING];

    dlclose(x->rtcmixdylib);
    sprintf(rm_command, "rm -rf \"%s\" ", x->pathname);
    if (system(rm_command)) error("error deleting unique dylib");

    free(x->canvas_path);
    free(x->pd_inbuf);
    free(x->pd_outbuf);
    int i;
    for (i=0; i<MAX_SCRIPTS; i++)
      {
        binbuf_free(x->rtcmix_script[i]);
      }

    error ("rtcmix~ DESTROYED!");
}


//this gets called whenever a float is received at *any* input
// used for the PField control inlets
static void rtcmix_float(t_rtcmix *x, double f)
{
  /*
  int i;

  //check to see which input the float came in, then set the appropriate variable value
  for(i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      if (i == x->x_obj.z_in)
        {
          if (i < x->num_inputs)
            {
              x->in[i] = f;
              post("rtcmix~: setting in[%d] =  %f, but rtcmix~ doesn't use this", i, f);
            }
          else x->pfield_set(i - (x->num_inputs-1), f);
        }
    }
  */
}

// bang triggers the current working script
static void rtcmix_bang(t_rtcmix *x)
{
  t_atom a[1];

  if (x->flushflag == 1) return; // heap and queue being reset

  // JWM - FIXME - no A_LONG in Pd (doesn't seem to be a problem so far...)
  a[0].a_w.w_float = x->current_script;
  a[0].a_type = A_FLOAT;
  //defer_low(x, (method)rtcmix_dogoscript, NULL, 1, a);
  rtcmix_dogoscript(x, NULL, 1, a);
}


// print out the rtcmix~ version
static void rtcmix_version(t_rtcmix *x)
{
  post("rtcmix~, v. %s by Joel Matthys (%s)", VERSION, RTcmixVERSION);
  outlet_bang(x->outpointer);
}

// see the note for rtcmix_dotext() below
static void rtcmix_text(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  rtcmix_dotext(x, s, argc, argv); // always defer this message
}


// what to do when we get the message "text"
// Max rtcmix~ scores come from the [textedit] object this way
// JWM: In Pd, this comes from [entry] as a list
static void rtcmix_dotext(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i, varnum;
  char thebuf[8192]; // should #define these probably
  char xfer[8192];
  char *bptr;
  int nchars;
  int top;

  bptr = thebuf;
  nchars = 0;
  top = 0;

  for (i=0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          //JWM - no A_LONG in Pd
          //case A_LONG:
          //sprintf(xfer, " %ld", argv[i].a_w.w_long);
          //break;
        case A_FLOAT:
          sprintf(xfer, " %lf", argv[i].a_w.w_float);
          break;
        case A_DOLLAR:
          varnum = argv[i].a_w.w_float;
          if ( !(x->var_set[varnum-1]) ) error("variable $%d has not been set yet, using 0.0 as default",varnum);
          sprintf(xfer, " %lf", x->var_array[varnum-1]);
          break;
        case A_SYMBOL:
          if (top == 0) { sprintf(xfer, "%s", argv[i].a_w.w_symbol->s_name); top = 1;}
          else sprintf(xfer, " %s", argv[i].a_w.w_symbol->s_name);
          break;
        case A_SEMI:
          sprintf(xfer, ";");
          break;
        case A_COMMA:
          sprintf(xfer, ",");
        }
      strcpy(bptr, xfer);
      nchars = strlen(xfer);
      bptr += nchars;
    }


  // ok, here's the deal -- when cycling tokenizes the message stream, if a quoted param (like a pathname to a soundfile)
  // contains any spaces it will be split into to separate, unquoted Symbols.  The function below repairs this for
  // certain specific rtcmix commands.  Not really elegant, but I don't know of another solution at present
 // JWM - TODO - what if, upon getting quotes, we jump to a separate routine until we get close quotes? Then jump back.
  rtcmix_badquotes("rtinput", thebuf);
  rtcmix_badquotes("system", thebuf);
  rtcmix_badquotes("dataset", thebuf);

  if (x->parse_score(thebuf, strlen(thebuf)) != 0) error("problem parsing RTcmix script");
}


// see the note in rtcmix_dotext() about why I have to do this
static void rtcmix_badquotes(char *cmd, char *thebuf)
{
  int i;
  char *rtinputptr;
  int badquotes, checkon;
  int clen;
  char tbuf[8192];

  // jeez this just sucks big giant easter eggs
  badquotes = 0;
  rtinputptr = strstr(thebuf, cmd); // find (if it exists) the instance of the command that may have a split in the quoted param
  if (rtinputptr) {
    rtinputptr += strlen(cmd);
    clen = strlen(thebuf) - (rtinputptr-thebuf);
    checkon = 0;
    for (i = 0; i < clen; i++) { // start from the command and look for spaces, between ( )
      if (*rtinputptr++ == '(' ) checkon = 1;
      if (checkon) {
        if ((int)*rtinputptr == 34) { // we found a quote, so its cool and we can stop
          i = clen;
        } else if (*rtinputptr == ')' ) {  // uh oh, no quotes and this command expects them
          badquotes = 1;
          i = clen;
        }
      }
    }
  }

  // lordy, look at this code.  I wish cycling would come up with an unaltered buf-passing type
  if (badquotes) { // now we're gonna put in the missing quotes
    rtinputptr = strstr(thebuf, cmd);
    rtinputptr += strlen(cmd);
    checkon = 0;
    for (i = 0; i < clen; i++) {
      if (*rtinputptr++ == '(' ) checkon = 1;
      if (checkon)
        if (*rtinputptr != ' ') i = clen;
    } // at this point we're at the beginning of the should-be-quoted param in the buffer

    // so we copy it to a temporary buffer, insert a quote...
    strcpy(tbuf, rtinputptr);
    *rtinputptr++ = 34;
    strcpy(rtinputptr, tbuf);

    // and then put in the rest of the param, inserting a quote at the end of the should-be-quoted param
    clen = strlen(thebuf) - (rtinputptr-thebuf);
    for (i = 0; i < clen; i++)
      if (*(++rtinputptr) == ')' ) {
        while (*(--rtinputptr) == ' ') { }
        rtinputptr++;
        i = clen;
      }

    // and this splices the modified, happily-quoted-param buffer back to the buf we give to rtcmix
    strcpy(tbuf, rtinputptr);
    *rtinputptr++ = 34;
    strcpy(rtinputptr, tbuf);
  }
}


// see the note for rtcmix_dortcmix() below
static void rtcmix_rtcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  //defer_low(x, (method)rtcmix_dortcmix, s, argc, argv); // always defer this message
  rtcmix_dortcmix(x,s,argc,argv);
}


// what to do when we get the message "rtcmix"
// used for single-shot RTcmix scorefile commands
static void rtcmix_dortcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i;
  double p[1024]; // should #define this probably
  char *cmd = NULL;

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          // JWM: no A_LONG in Pd
          //case A_LONG:
          //p[i-1] = (double)(argv[i].a_w.w_long);
          //break;
        case A_FLOAT:
          p[i-1] = (double)(argv[i].a_w.w_float);
          break;
        case A_SYMBOL:
          cmd = argv[i].a_w.w_symbol->s_name;
        }
    }

  x->parse_dispatch(cmd, p, argc-1, NULL);
}


// the "var" message allows us to set $n variables imbedded in a scorefile with varnum value messages
static void rtcmix_var(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i, varnum;

  for (i = 0; i < argc; i += 2)
    {
      varnum = argv[i].a_w.w_float;
      if ( (varnum < 1) || (varnum > NVARS) )
        {
          error("only vars $1 - $9 are allowed");
          return;
        }
      x->var_set[varnum-1] = 1;
      switch (argv[i+1].a_type)
        {
          // JWM - no A_LONG in Pd
          //case A_LONG:
          //x->var_array[varnum-1] = (float)(argv[i+1].a_w.w_long);
          //break;
        case A_FLOAT:
          x->var_array[varnum-1] = argv[i+1].a_w.w_float;
        }
    }
}


// the "varlist" message allows us to set $n variables imbedded in a scorefile with a list of positional vars
static void rtcmix_varlist(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i;

  if (argc > NVARS)
    {
      error("asking for too many variables, only setting the first 9 ($1-$9)");
      argc = NVARS;
    }

  for (i = 0; i < argc; i++)
    {
      x->var_set[i] = 1;
      switch (argv[i].a_type)
        {
          // JWM - No A_LONG in Pd
          //case A_LONG:
          //x->var_array[i] = (float)(argv[i].a_w.w_long);
          //break;
        case A_FLOAT:
          x->var_array[i] = argv[i].a_w.w_float;
        }
    }
}


// the "bufset" message allows access to a [buffer~] object.  The only argument is the name of the [buffer~]
static void rtcmix_bufset(t_rtcmix *x, t_symbol *s)
{
  /*
  t_buffer *b;

  if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer)
    {
      x->buffer_set(s->s_name, b->b_samples, b->b_frames, b->b_nchans, b->b_modtime);
    }
  else
    {
      error("rtcmix~: no buffer~ %s", s->s_name);
    }
  */
}

// the "flush" message
static void rtcmix_flush(t_rtcmix *x)
{
  x->flushflag = 1; // set a flag, the flush will happen in perform after pulltraverse()
}


// here is the text-editor buffer stuff, go dan trueman go!
// used for rtcmix~ internal buffers
static void rtcmix_edclose (t_rtcmix *x, char **text, long size)
{
  /*
  if (x->rtcmix_script[x->current_script])
    {
      //sysmem_freeptr((void *)x->rtcmix_script[x->current_script]);
      freebytes((void *)x->rtcmix_script[x->current_script],sizeof(x->rtcmix_script[x->current_script]));
      x->rtcmix_script[x->current_script] = 0;
    }
  x->rtcmix_script_len[x->current_script] = size;
  x->rtcmix_script[x->current_script] = (char *)sysmem_newptr((size+1) * sizeof(char)); // size+1 so we can add '\0' at end
  x->rtcmix_script[x->current_script] = getbytes(
  strncpy(x->rtcmix_script[x->current_script], *text, size);
  x->rtcmix_script[x->current_script][size] = '\0'; // add the terminating '\0'
  x->m_editor = NULL;
  */
}


static void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result)
{
  //*result = 3; //don't put up dialog box
  //return;
}


// open up an ed window on the current buffer
static void rtcmix_dblclick(t_rtcmix *x)
{
  post("DOUBLE CLICK!!!");
  // JWM - eventually open an editor here
  /*
  char title[80];

  if (x->m_editor)
    {
      if(x->rtcmix_script[x->current_script])
        object_method(x->m_editor, gensym("settext"), x->rtcmix_script[x->current_script], gensym("utf-8"));
    }
  else
    {
      x->m_editor = object_new(CLASS_NOBOX, gensym("jed"), (t_object *)x, 0);
      sprintf(title,"script_%d", x->current_script);
      object_attr_setsym(x->m_editor, gensym("title"), gensym(title));
      if(x->rtcmix_script[x->current_script])
        object_method(x->m_editor, gensym("settext"), x->rtcmix_script[x->current_script], gensym("utf-8"));
    }

  object_attr_setchar(x->m_editor, gensym("visible"), 1);
  */
}


// see the note for rtcmix_goscript() below
static void rtcmix_goscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  //defer_low(x, (method)rtcmix_dogoscript, s, argc, argv); // always defer this message
  rtcmix_dogoscript(x, s, argc, argv);
}


// the [goscript N] message will cause buffer N to be sent to the RTcmix parser
static void rtcmix_dogoscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short temp,i;
  if (argc == 0)
    {
      error("rtcmix~: goscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      if (argv[i].a_type == A_FLOAT)
        temp = (short)argv[i].a_w.w_float;
    }

  if (temp > MAX_SCRIPTS)
    {
      error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
      temp = MAX_SCRIPTS-1;
    }
  if (temp < 0)
    {
      error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
      temp = 0;
    }
  x->current_script = temp;


  short j;
  int tval;
  char *thebuf = malloc(MAXSCRIPTSIZE);
  int natoms = binbuf_getnatom(x->rtcmix_script[x->current_script]);

  binbuf_gettext(x->rtcmix_script[x->current_script], &thebuf, &natoms);

  if (natoms==0)
    error("rtcmix~: you are triggering a 0-length script!");

  int buflen = strlen(thebuf);

  // probably don't need to transfer to a new buffer, but I want to be sure there's room for the \0,
  // plus the substitution of \n for those annoying ^M thingies
  for (i = 0, j = 0; i < buflen; i++)
    {
      if ((int)thebuf[j] == 13) thebuf[j] = '\n'; // RTcmix wants newlines, not <cr>'s
      // ok, here's where we substitute the $vars
      if (thebuf[j] == '$')
        {
          sscanf(thebuf+i+1, "%d", &tval);
          if ( !(x->var_set[tval-1]) ) error("variable $%d has not been set yet, using 0.0 as default", tval);
          sprintf(thebuf+j, "%f", x->var_array[tval-1]);
          j = strlen(thebuf)-1;
          i++; // skip over the var number in input
        }
      j++;
    }
  thebuf[j] = '\0';

  if ( (canvas_dspstate == 1) || (strncmp(thebuf, "system", 6) == 0) )
    { // don't send if the dacs aren't turned on, unless it is a system() <------- HACK HACK HACK!

     if (x->parse_score(thebuf, j) != 0) error("problem parsing RTcmix script");

      }
  free(thebuf);
}


// [openscript N] will open a buffer N
static void rtcmix_openscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i,temp = 0;

  if (argc == 0)
    {
      error("rtcmix~: openscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      if (argv[i].a_type == A_FLOAT)
        temp = (short)argv[i].a_w.w_float;
    }

  if (temp > MAX_SCRIPTS)
    {
      error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
      temp = MAX_SCRIPTS-1;
    }
  if (temp < 0)
    {
      error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
      temp = 0;
    }

  x->current_script = temp;
  rtcmix_dblclick(x);
}


// [setscript N] will set the currently active script to N
static void rtcmix_setscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i,temp = 0;

  if (argc == 0)
    {
      error("rtcmix~: setscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      if (argv[i].a_type == A_FLOAT)
          temp = (short)argv[i].a_w.w_float;
    }

  if (temp > MAX_SCRIPTS)
    {
      error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
      temp = MAX_SCRIPTS-1;
    }
  if (temp < 0)
    {
      error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
      temp = 0;
    }

  x->current_script = temp;
}


// the [savescript] message triggers this
static void rtcmix_write(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i, temp = 0;

  for (i = 0; i < argc; i++)
    {
      if (argv[i].a_type)
        temp = (short)argv[i].a_w.w_float;
    }

  if (temp > MAX_SCRIPTS)
    {
      error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
      temp = MAX_SCRIPTS-1;
    }
  if (temp < 0)
    {
      error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
      temp = 0;
    }

  x->current_script = temp;
  post("rtcmix: current script is %d", temp);

  //defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
  rtcmix_dowrite(x,s,argc,argv);
}


// the [savescriptas] message triggers this
static void rtcmix_writeas(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  /*
  short i, temp = 0;

  for (i=0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
        case A_FLOAT:
          temp = (short)argv[i].a_w.w_float;
          if (temp > MAX_SCRIPTS)
            {
              error("rtcmix~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
              temp = MAX_SCRIPTS-1;
            }
          if (temp < 0)
            {
              error("rtcmix~: the script number should be > 0!  Resetting to script number 0");
              temp = 0;
            }
          x->current_script = temp;
          //x->s_name[x->current_script][0] = 0;
          break;
        case A_SYMBOL://this doesn't work yet
          //strcpy(x->s_name[x->current_script], argv[i].a_w.w_symbol->s_name);
          post("rtcmix~: writing file %s",x->s_name[x->current_script]);
        }
    }
  post("rtcmix: current script is %d", temp);

  //defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
  rtcmix_dowrite(x,s,argc,argv);
  */
}


// deferred from the [save*] messages
static void rtcmix_dowrite(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  /*
  char filename[256];
  t_handle script_handle;
  short err;
  long type_chosen, thistype = 'TEXT';
  t_filehandle fh;

  if(!x->s_name[x->current_script][0])
    {
      //if (saveas_dialog(&x->s_name[0][x->current_script], &x->path[x->current_script], &type))
      if (saveasdialog_extended(x->s_name[x->current_script], &x->path[x->current_script], &type_chosen, &thistype, 1))
        return; //user cancelled
    }
  strcpy(filename, x->s_name[x->current_script]);

  err = path_createsysfile(filename, x->path[x->current_script], thistype, &fh);
  if (err)
    {
      fh = 0;
      error("rtcmix~: error %d creating file", err);
      return;
    }

  script_handle = sysmem_newhandle(0);
  sysmem_ptrandhand (x->rtcmix_script[x->current_script], script_handle, x->rtcmix_script_len[x->current_script]);

  err = sysfile_writetextfile(fh, script_handle, TEXT_LB_UNIX);
  if (err)
    {
      fh = 0;
      error("rtcmix~: error %d writing file", err);
      return;
    }

  // BGG for some reason mach-o doesn't like this one... the memory hit should be small
  //	sysmem_freehandle(script_handle);
  sysfile_seteof(fh, x->rtcmix_script_len[x->current_script]);
  sysfile_close(fh);

  return;
  */
}

// the [read ...] message triggers this
static void rtcmix_read(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  int i;
  int temp = 0;
  t_symbol *filename = gensym("foobar");
  post("filename: %s",filename->s_name);
  int fnflag = 0;
  for (i=0; i<argc; i++)
    {
      switch (argv[i].a_type)
        {
        case A_FLOAT:
          temp = (short)argv[i].a_w.w_float;
          break;
        case A_SYMBOL:
          filename = argv[i].a_w.w_symbol;
          fnflag = 1;
          break;
        }
    }
  if (temp<0)
    {
      error("rtcmix~: the script number must be [0-19]. Setting to 0.");
      temp = 0;
    }
  if (temp>(MAX_SCRIPTS-1))
    {
      error("rtcmix~: the script number must be [0-19]. Setting to 19.");
      temp = (int)(MAX_SCRIPTS-1);
    }
  x->current_script = temp;

  if (!fnflag)
    {
      //post("openpanel signaled");
      x->rw_flag = RTcmixREADFLAG;
      //post("x->x_s->s_name: %s, x->canvas_path->s_name: %s",x->x_s->s_name, x->canvas_path->s_name);
      sys_vgui("pdtk_openpanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
    }
  else
    {
  rtcmix_callback(x,filename);
    }
}

static void rtcmix_save(t_rtcmix *x, void *w)
{
  x->rw_flag = RTcmixWRITEFLAG;
  sys_vgui("pdtk_savepanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
}

static void rtcmix_callback(t_rtcmix *x, t_symbol *filename)
{
  post("callback! flag = %d",x->rw_flag);

  char *buf = malloc(MAXPDSTRING);
  switch (x->rw_flag)
    {
    case RTcmixWRITEFLAG:
      canvas_makefilename(x->x_canvas, filename->s_name,
                          buf, MAXPDSTRING);
      if (binbuf_write(x->rtcmix_script[x->current_script], buf, "", CRFLAG))
        error("%s: write failed", buf);
      break;
    case RTcmixREADFLAG:
    default:
      if (binbuf_read_via_canvas(x->rtcmix_script[x->current_script], filename->s_name, x->x_canvas, CRFLAG))
        error("%s: read failed", filename->s_name);
    }
  free(buf);
}
