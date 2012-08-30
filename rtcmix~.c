// rtcmix~ v 0.01, Joel Matthys (8/2012) (Linux, Pd support), based on:
// rtcmix~ v 1.81, Brad Garton (2/2011) (OS 10.5/6, Max5 support)
// uses the RTcmix bundled executable lib, now based on RTcmix-4.0.1.6
// see http://music.columbia.edu/cmc/RTcmix for more info
//
// see rtcmix~.c in BGG's rtcmix~ for Max/MSP thank yous & changelog

#define VERSION "0.01"
#define RTcmixVERSION "RTcmix-maxmsp-4.0.1.6"

// JWM - Pd headers
#include "m_pd.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// JWM WHAT A ROCKSTAR Brad is! Reverting to dlopen for linux
// BGG kept the dlopen() stuff in for future use
// for the dlopen() loader
#include <dlfcn.h>
int dylibincr;


#define MAX_INPUTS 100  //arbitrary -- for Damon!
#define MAX_OUTPUTS 20	//also arbitrary
#define MAX_SCRIPTS 20	//how many scripts can we store internally


/******* RTcmix stuff *******/
typedef int (*rtcmixmainFunctionPtr)();
typedef int (*maxmsp_rtsetparamsFunctionPtr)(float sr, int nchans, int vecsize, float *maxmsp_inbuf, float *maxmsp_outbuf, char *theerror);
typedef int (*parse_scoreFunctionPtr)();
typedef void (*pullTraverseFunctionPtr)();
typedef int (*check_bangFunctionPtr)();
typedef int (*check_valsFunctionPtr)(float *thevals);
typedef double (*parse_dispatchFunctionPtr)(char *cmd, double *p, int n_args, void *retval);
typedef int (*check_errorFunctionPtr)();
typedef void (*pfield_setFunctionPtr)(int inlet, float pval);
typedef void (*buffer_setFunctionPtr)(char *bufname, float *bufstart, int nframes, int nchans, int modtime);
typedef int (*flushPtr)();

void *rtcmix_class;

typedef struct _rtcmix
{
  //header
  t_pxobject x_obj;

  //variables specific to this object
  float srate;                                        //sample rate
  long num_inputs, num_outputs;       //number of inputs and outputs
  long num_pinlets;				// number of inlets for dynamic PField control
  float in[MAX_INPUTS];			//values of input variables
  float in_connected[MAX_INPUTS]; //booleans: true if signals connected to the input in question
  //we use this "connected" boolean so that users can connect *either* signals or floats
  //to the various inputs; sometimes it's easier just to have floats, but other times
  //it's essential to have signals.... but we have to know.
  void *outpointer;

  /******* RTcmix stuff *******/
  rtcmixmainFunctionPtr rtcmixmain;
  maxmsp_rtsetparamsFunctionPtr maxmsp_rtsetparams;
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
  char pathname[1024]; // probably should be malloc'd

  // space for these malloc'd in rtcmix_dsp()
  float *maxmsp_outbuf;
  float *maxmsp_inbuf;

  // script buffer pointer for large binbuf restores
  char *restore_buf_ptr;

  // for the rtmix_var() and rtcmix_varlist() $n variable scheme
#define NVARS 9
  float var_array[NVARS];
  Boolean var_set[NVARS];

  // stuff for check_vals
#define MAXDISPARGS 1024 // from RTcmix H/maxdispargs.h
  float thevals[MAXDISPARGS];
  t_atom valslist[MAXDISPARGS];

  // buffer for error-reporting
  char theerror[1024];

  // editor stuff
  t_object m_obj;
  t_object *m_editor;
  char *rtcmix_script[MAX_SCRIPTS], s_name[MAX_SCRIPTS][256];
  long rtcmix_script_len[MAX_SCRIPTS];
  short current_script, path[MAX_SCRIPTS];

  // for flushing all events on the queue/heap (resets to new ones inside RTcmix)
  int flushflag;
} t_rtcmix;


// for where the rtcmix-dylibs folder is located
char *mpathptr;
char mpathname[1024]; // probably should be malloc'd


/****PROTOTYPES****/

//setup funcs; this probably won't change, unless you decide to change the number of
//args that the user can input, in which case rtcmix_new will have to change
void *rtcmix_new(long num_inoutputs, long num_additional);
void rtcmix_dsp(t_rtcmix *x, t_signal **sp, short *count);
t_int *rtcmix_perform(t_int *w);
void rtcmix_assist(t_rtcmix *x, void *b, long m, long a, char *s);
void rtcmix_free(t_rtcmix *x);

//for getting floats, ints or bangs at inputs
void rtcmix_float(t_rtcmix *x, double f);
void rtcmix_int(t_rtcmix *x, int i);
void rtcmix_bang(t_rtcmix *x);
void rtcmix_dobangout(t_rtcmix *x, Symbol *s, short argc, t_atom *argv); // for the defer

//for custom messages
void rtcmix_version(t_rtcmix *x);
void rtcmix_text(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_dotext(t_rtcmix *x, Symbol *s, short argc, Atom *argv); // for the defer
void rtcmix_badquotes(char *cmd, char *buf); // this is to check for 'split' quoted params, called in rtcmix_dotext
void rtcmix_rtcmix(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_dortcmix(t_rtcmix *x, Symbol *s, short argc, Atom *argv); // for the defer
void rtcmix_var(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_varlist(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_bufset(t_rtcmix *x, t_symbol *s);
void rtcmix_flush(t_rtcmix *x);

//for the text editor
void rtcmix_edclose (t_rtcmix *x, char **text, long size);
void rtcmix_dblclick(t_rtcmix *x);
void rtcmix_goscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_dogoscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv); // for the defer
void rtcmix_openscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_setscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_read(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_doread(t_rtcmix *x, Symbol *s, short argc, t_atom *argv);
void rtcmix_write(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_writeas(t_rtcmix *x, Symbol *s, short argc, Atom *argv);
void rtcmix_dowrite(t_rtcmix *x, Symbol *s, short argc, t_atom *argv);
void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result);

//for binbuf storage of scripts
void rtcmix_save(t_rtcmix *x, void *w);
void rtcmix_restore(t_rtcmix *x, Symbol *s, short argc, Atom *argv);

t_symbol *ps_buffer; // for [buffer~]


/****FUNCTIONS****/

//primary MSP funcs
int main(void)
{
  int i;
  short path, rval;

  //the A_DEFLONG arguments give us the object arguments for the user to set number of ins/outs, etc.
  //change these if you want different user args
  setup((struct messlist **)&rtcmix_class, (method)rtcmix_new, (method)rtcmix_free, (short)sizeof(t_rtcmix), 0L, A_DEFLONG, A_DEFLONG, 0);

  //standard messages; don't change these
  addmess((method)rtcmix_dsp, "dsp", A_CANT, 0);
  addmess((method)rtcmix_assist,"assist", A_CANT,0);

  //our own messages
  addmess((method)rtcmix_version, "version", 0);
  addmess((method)rtcmix_text, "text", A_GIMME, 0);
  addmess((method)rtcmix_rtcmix, "rtcmix", A_GIMME, 0);
  addmess((method)rtcmix_var, "var", A_GIMME, 0);
  addmess((method)rtcmix_varlist, "varlist", A_GIMME, 0);
  addmess((method)rtcmix_bufset, "bufset", A_SYMBOL, 0);
  addmess((method)rtcmix_flush, "flush", 0);

  //so we know what to do with floats that we receive at the inputs
  addfloat((method)rtcmix_float);

  // this for ints...
  addint((method)rtcmix_int);

  // trigger scripts
  addbang((method)rtcmix_bang);

  //for the text editor and scripts
  addmess ((method)rtcmix_edclose, "edclose", A_CANT, 0);
  addmess((method)rtcmix_dblclick,	"dblclick",	A_CANT, 0);
  addmess((method)rtcmix_goscript, "goscript", A_GIMME, 0);
  addmess((method)rtcmix_openscript, "openscript", A_GIMME, 0);
  addmess((method)rtcmix_setscript, "setscript", A_GIMME, 0);
  addmess((method)rtcmix_read, "read", A_GIMME, 0);
  addmess ((method)rtcmix_okclose, "okclose", A_CANT, 0);
  addmess((method)rtcmix_write, "savescript", A_GIMME, 0);
  addmess((method)rtcmix_writeas, "savescriptas", A_GIMME, 0);

  // binbuf storage
  addmess((method)rtcmix_save, "save", A_CANT, 0);
  addmess((method)rtcmix_restore, "restore", A_GIMME, 0);

  //gotta have this one....
  dsp_initclass();

  // find the rtcmix-dylibs folder location
  nameinpath("rtcmix-dylibs", &path);
  rval = path_topathname(path, "", mpathname);
  if (rval != 0) error("couldn't find the rtcmix-dylibs folder!");
  else
    { // this is to find the beginning "/" for root
      for (i = 0; i < 1000; i++)
        if (mpathname[i] == '/') break;
      mpathptr = mpathname+i;
    }

  ps_buffer = gensym("buffer~"); // for [buffer~]

  // ho ho!
  post("rtcmix~ -- RTcmix music language, v. %s (%s)", VERSION, RTcmixVERSION);
  return(1);
}


//this gets called when the object is created; everytime the user types in new args, this will get called
void *rtcmix_new(long num_inoutputs, long num_additional)
{
  int i;
  t_rtcmix *x;

  // for the full path to the rtcmixdylib.so file
  char pathname[1000]; // probably should be malloc'd
  // BGG kept this in for the dlopen() stuff
  char cp_command[1024]; // should probably be malloc'd


  // creates the object
  x = (t_rtcmix *)newobject(rtcmix_class);

  //zero out the struct, to be careful (takk to jkclayton)
  if (x)
    {
      for(i=sizeof(t_pxobject);i<sizeof(t_rtcmix);i++)
        ((char *)x)[i]=0;
    }

  // binbuf storage
  // this sends the 'restore' message (rtcmix_restore() below)
  gensym("#X")->s_thing = (struct object*)x;

  // these are the entry function pointers in to the rtcmixdylib.so lib
  x->rtcmixmain = NULL;
  x->maxmsp_rtsetparams = NULL;
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
  if (num_inoutputs < 1) num_inoutputs = 1; // no args, use default of 1 channel in/out
  if ((num_inoutputs + num_additional) > MAX_INPUTS)
    {
      error("sorry, only %d total inlets are allowed!", MAX_INPUTS);
      return(NULL);
    }

  x->num_inputs = num_inoutputs;
  x->num_outputs = num_inoutputs;
  x->num_pinlets = num_additional;

  // setup up inputs and outputs, for audio inputs
  dsp_setup((t_pxobject *)x, x->num_inputs + x->num_pinlets);

  // outputs, right-to-left
  x->outpointer = outlet_new((t_object *)x, 0); // for bangs (from MAXBANG), values + value lists (from MAXMESSAGE)

  for (i = 0; i < x->num_outputs; i++)
    {
      outlet_new((t_object *)x, "signal");
    }

  // initialize some variables; important to do this!
  for (i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      x->in[i] = 0.;
      x->in_connected[i] = 0;
    }

  //occasionally this line is necessary if you are doing weird asynchronous things with the in/out vectors
  x->x_obj.z_misc = Z_NO_INPLACE;


  // RTcmix stuff
  // full path to the rtcmixdylib.so file
  sprintf(pathname, "%s/rtcmixdylib.so", mpathptr);

  // JWM: This is a lifesaver.
  // BGG kept this in for future use of dlopen() (if NSLoad gets dropped)

  x->dylibincr = dylibincr++; // keep track of rtcmixdylibN.so for copy/load

  // full path to the rtcmixdylib.so file
  sprintf(x->pathname, "%s/rtcmixdylib%d.so", mpathptr, x->dylibincr);

  // ok, this is fairly insane.  To guarantee a fully-isolated namespace with dlopen(), we need
  // a totally *unique* dylib, so we copy this.  Deleted in rtcmix_free() below
  // RTLD_LOCAL doesn't do it all - probably the global vars in RTcmix
  sprintf(cp_command, "cp \"%s/BASE_rtcmixdylib.so\" \"%s\"", mpathptr,x->pathname);
  system(cp_command);

  // load the dylib
  x->rtcmixdylib = dlopen(x->pathname, RTLD_NOW | RTLD_LOCAL);

  // find the main entry to be sure we're cool...
  x->rtcmixmain = dlsym(x->rtcmixdylib, "rtcmixmain");
  if (x->rtcmixmain)	x->rtcmixmain();
  else error("rtcmix~ could not call rtcmixmain()");

  x->maxmsp_rtsetparams = dlsym(x->rtcmixdylib, "maxmsp_rtsetparams");
  if (!(x->maxmsp_rtsetparams))
    error("rtcmix~ could not find maxmsp_rtsetparams()");

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

  // set up for the variable-substitution scheme
  for(i = 0; i < NVARS; i++)
    {
      x->var_set[i] = false;
      x->var_array[i] = 0.0;
    }

  // the text editor
  x->m_editor = NULL;
  x->current_script = 0;
  for (i = 0; i < MAX_SCRIPTS; i++)
    {
      x->s_name[i][0] = 0;
    }

  x->flushflag = 0; // [flush] sets flag for call to x->flush() in rtcmix_perform() (after pulltraverse completes)

  return (x);
}

//this gets called everytime audio is started; even when audio is running, if the user
//changes anything (like deletes a patch cord), audio will be turned off and
//then on again, calling this func.
//this adds the "perform" method to the DSP chain, and also tells us
//where the audio vectors are and how big they are
void rtcmix_dsp(t_rtcmix *x, t_signal **sp, short *count)
{
  void *dsp_add_args[MAX_INPUTS + MAX_OUTPUTS + 2];
  int i;

  // RTcmix vars
  // for the full path to the rtcmixdylib.so file
  char pathname[1000]; // probably should be malloc'd

  // these are the entry function pointers in to the rtcmixdylib.so lib
  x->rtcmixmain = NULL;
  x->maxmsp_rtsetparams = NULL;
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
  dsp_add_args[0] = x; //the object itself
  for(i = 0; i < (x->num_inputs + x->num_pinlets + x->num_outputs); i++)
    { //pointers to the input and output vectors
      dsp_add_args[i+1] = sp[i]->s_vec;
    }
  dsp_add_args[x->num_inputs + x->num_pinlets + x->num_outputs + 1] = (void *)sp[0]->s_n; //pointer to the vector size
  dsp_addv(rtcmix_perform, (x->num_inputs + x->num_pinlets + x->num_outputs + 2), dsp_add_args); //add them to the signal chain

  // RTcmix stuff
  // full path to the rtcmixdylib.so file
  sprintf(pathname, "%s/rtcmixdylib.so", mpathptr);

  // BGG kept this in for dlopen() stuff, future if NSLoad gets dropped

  // reload, this reinits the RTcmix queue, etc.
  dlclose(x->rtcmixdylib);

  // load the dylib
  x->rtcmixdylib = dlopen(x->pathname,  RTLD_NOW | RTLD_LOCAL);

  // find the main entry to be sure we're cool...
  x->rtcmixmain = dlsym(x->rtcmixdylib, "rtcmixmain");
  if (x->rtcmixmain)	x->rtcmixmain();
  else error("rtcmix~ could not call rtcmixmain()");

  x->maxmsp_rtsetparams = dlsym(x->rtcmixdylib, "maxmsp_rtsetparams");
  if (!(x->maxmsp_rtsetparams))
    error("rtcmix~ could not find maxmsp_rtsetparams()");

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
  x->maxmsp_inbuf = malloc(sizeof(float) * sp[0]->s_n * x->num_inputs);
  x->maxmsp_outbuf = malloc(sizeof(float) * sp[0]->s_n * x->num_outputs);

  // zero out these buffers for UB
  for (i = 0; i < (sp[0]->s_n * x->num_inputs); i++) x->maxmsp_inbuf[i] = 0.0;
  for (i = 0; i < (sp[0]->s_n * x->num_outputs); i++) x->maxmsp_outbuf[i] = 0.0;

  if (x->maxmsp_rtsetparams)
    {
      x->maxmsp_rtsetparams(x->srate, x->num_outputs, sp[0]->s_n, x->maxmsp_inbuf, x->maxmsp_outbuf, x->theerror);
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
  if (x->x_obj.z_disabled) goto out;

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
          (x->maxmsp_inbuf)[k++] = *in[i]++;
        else
          (x->maxmsp_inbuf)[k++] = *in[i];

      for(i = 0; i < x->num_outputs; i++)
        *out[i]++ = (x->maxmsp_outbuf)[j++];

    }

  // RTcmix stuff
  // this drives the RTcmix sample-computing engine
  x->pullTraverse();

  // look for a pending bang from MAXBANG()
  if (x->check_bang() == 1) // I don't think I really need this defer, but what the heck.
    defer_low(x, (method)rtcmix_dobangout, (Symbol *)NULL, 0, (Atom *)NULL);

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
 out:
  return w + x->num_inputs + x->num_pinlets + x->num_outputs + 3;
}


// the deferred bang output
void rtcmix_dobangout(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  outlet_bang(x->outpointer);
}


//tells the user about the inputs/outputs when mousing over them
void rtcmix_assist(t_rtcmix *x, void *b, long m, long a, char *s)
{

  if (m == 1)
    {
      if (a == 0) sprintf(s, "signal/text (score commands) in");
      else sprintf(s, "signal/pfieldvals in");
    }
  if (m == 2)
    {
      if (a < x->num_inputs) sprintf(s, "signal out");
      else sprintf(s, "bang, float or float-list out");
    }
}


// here's my free function
void rtcmix_free(t_rtcmix *x)
{
  // BGG kept dlopen() stuff in in case NSLoad gets dropped
    char rm_command[1024]; // should probably be malloc'd

    dlclose(x->rtcmixdylib);
    sprintf(rm_command, "rm -rf \"%s\" ", x->pathname);
    system(rm_command);

    // close any open editor windows
    if (x->m_editor)
      freeobject((t_object *)x->m_editor);
    x->m_editor = NULL;
    dsp_free((t_pxobject *)x);
}


//this gets called whenever a float is received at *any* input
// used for the PField control inlets
void rtcmix_float(t_rtcmix *x, double f)
{
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
}


//this gets called whenever an int is received at *any* input
// used for the PField control inlets
void rtcmix_int(t_rtcmix *x, int ival)
{
  int i;

  //check to see which input the float came in, then set the appropriate variable value
  for(i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      if (i == x->x_obj.z_in)
        {
          if (i < x->num_inputs)
            {
              x->in[i] = (float)ival;
              post("rtcmix~: setting in[%d] =  %f, but rtcmix~ doesn't use this", i, (float)ival);
            }
          else
            {
              x->pfield_set(i - (x->num_inputs-1), (float)ival);
            }
        }
    }
}


// bang triggers the current working script
void rtcmix_bang(t_rtcmix *x)
{
  Atom a[1];

  if (x->flushflag == 1) return; // heap and queue being reset

  // JWM - FIXME - no A_LONG in Pd
  a[0].a_w.w_long = x->current_script;
  a[0].a_type = A_LONG;
  defer_low(x, (method)rtcmix_dogoscript, NULL, 1, a);
}


// print out the rtcmix~ version
void rtcmix_version(t_rtcmix *x)
{
  post("rtcmix~, v. %s by Brad Garton (%s)", VERSION, RTcmixVERSION);
}


// see the note for rtcmix_dotext() below
void rtcmix_text(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  defer_low(x, (method)rtcmix_dotext, s, argc, argv); // always defer this message
}


// what to do when we get the message "text"
// rtcmix~ scores come from the [textedit] object this way
void rtcmix_dotext(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
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
          varnum = argv[i].a_w.w_long;
          if ( !(x->var_set[varnum-1]) ) error("variable $%d has not been set yet, using 0.0 as default",varnum);
          sprintf(xfer, " %lf", x->var_array[varnum-1]);
          break;
        case A_SYMBOLBOL:
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
  rtcmix_badquotes("rtinput", thebuf);
  rtcmix_badquotes("system", thebuf);
  rtcmix_badquotes("dataset", thebuf);

  if (x->parse_score(thebuf, strlen(thebuf)) != 0) error("problem parsing RTcmix script");
}


// see the note in rtcmix_dotext() about why I have to do this
void rtcmix_badquotes(char *cmd, char *thebuf)
{
  int i;
  char *rtinputptr;
  Boolean badquotes, checkon;
  int clen;
  char tbuf[8192];

  // jeez this just sucks big giant easter eggs
  badquotes = false;
  rtinputptr = strstr(thebuf, cmd); // find (if it exists) the instance of the command that may have a split in the quoted param
  if (rtinputptr) {
    rtinputptr += strlen(cmd);
    clen = strlen(thebuf) - (rtinputptr-thebuf);
    checkon = false;
    for (i = 0; i < clen; i++) { // start from the command and look for spaces, between ( )
      if (*rtinputptr++ == '(' ) checkon = true;
      if (checkon) {
        if ((int)*rtinputptr == 34) { // we found a quote, so its cool and we can stop
          i = clen;
        } else if (*rtinputptr == ')' ) {  // uh oh, no quotes and this command expects them
          badquotes = true;
          i = clen;
        }
      }
    }
  }

  // lordy, look at this code.  I wish cycling would come up with an unaltered buf-passing type
  if (badquotes) { // now we're gonna put in the missing quotes
    rtinputptr = strstr(thebuf, cmd);
    rtinputptr += strlen(cmd);
    checkon = false;
    for (i = 0; i < clen; i++) {
      if (*rtinputptr++ == '(' ) checkon = true;
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
void rtcmix_rtcmix(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  defer_low(x, (method)rtcmix_dortcmix, s, argc, argv); // always defer this message
}


// what to do when we get the message "rtcmix"
// used for single-shot RTcmix scorefile commands
void rtcmix_dortcmix(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
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
void rtcmix_var(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i, varnum;

  for (i = 0; i < argc; i += 2)
    {
      varnum = argv[i].a_w.w_long;
      if ( (varnum < 1) || (varnum > NVARS) )
        {
          error("only vars $1 - $9 are allowed");
          return;
        }
      x->var_set[varnum-1] = true;
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
void rtcmix_varlist(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i;

  if (argc > NVARS)
    {
      error("asking for too many variables, only setting the first 9 ($1-$9)");
      argc = NVARS;
    }

  for (i = 0; i < argc; i++)
    {
      x->var_set[i] = true;
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
void rtcmix_bufset(t_rtcmix *x, t_symbol *s)
{
  t_buffer *b;

  if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer)
    {
      x->buffer_set(s->s_name, b->b_samples, b->b_frames, b->b_nchans, b->b_modtime);
    }
  else
    {
      error("rtcmix~: no buffer~ %s", s->s_name);
    }
}

// the "flush" message
void rtcmix_flush(t_rtcmix *x)
{
  x->flushflag = 1; // set a flag, the flush will happen in perform after pulltraverse()
}


// here is the text-editor buffer stuff, go dan trueman go!
// used for rtcmix~ internal buffers
void rtcmix_edclose (t_rtcmix *x, char **text, long size)
{
  if (x->rtcmix_script[x->current_script])
    {
      sysmem_freeptr((void *)x->rtcmix_script[x->current_script]);
      x->rtcmix_script[x->current_script] = 0;
    }
  x->rtcmix_script_len[x->current_script] = size;
  x->rtcmix_script[x->current_script] = (char *)sysmem_newptr((size+1) * sizeof(char)); // size+1 so we can add '\0' at end
  strncpy(x->rtcmix_script[x->current_script], *text, size);
  x->rtcmix_script[x->current_script][size] = '\0'; // add the terminating '\0'
  x->m_editor = NULL;
}


void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result)
{
  *result = 3; //don't put up dialog box
  return;
}


// open up an ed window on the current buffer
void rtcmix_dblclick(t_rtcmix *x)
{
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
}


// see the note for rtcmix_goscript() below
void rtcmix_goscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  defer_low(x, (method)rtcmix_dogoscript, s, argc, argv); // always defer this message
}


// the [goscript N] message will cause buffer N to be sent to the RTcmix parser
void rtcmix_dogoscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i,j,temp = 0;
  int tval;
  int buflen;
#define MAXSCRIPTSIZE 16384
  char thebuf[MAXSCRIPTSIZE]; // should probably by dyn-alloced, or at least set to coordinate with RTcmix (if necessary)

  if (argc == 0)
    {
      error("rtcmix~: goscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          // JWM: No A_LONG in Pd
          //case A_LONG:
          //temp = (short)argv[i].a_w.w_long;
          //break;
        case A_FLOAT:
          temp = (short)argv[i].a_w.w_float;
        }
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

  if (x->rtcmix_script_len[x->current_script] == 0)
    post("rtcmix~: you are triggering a 0-length script!");

  buflen = x->rtcmix_script_len[x->current_script];
  if (buflen >= MAXSCRIPTSIZE)
    {
      error("rtcmix~ script %d is too large!", x->current_script);
      return;
    }

  // probably don't need to transfer to a new buffer, but I want to be sure there's room for the \0,
  // plus the substitution of \n for those annoying ^M thingies
  for (i = 0, j = 0; i < buflen; i++)
    {
      thebuf[j] = *(x->rtcmix_script[x->current_script]+i);
      if ((int)thebuf[j] == 13) thebuf[j] = '\n'; // RTcmix wants newlines, not <cr>'s

      // ok, here's where we substitute the $vars
      if (thebuf[j] == '$')
        {
          sscanf(x->rtcmix_script[x->current_script]+i+1, "%d", &tval);
          if ( !(x->var_set[tval-1]) ) error("variable $%d has not been set yet, using 0.0 as default", tval);
          sprintf(thebuf+j, "%f", x->var_array[tval-1]);
          j = strlen(thebuf)-1;
          i++; // skip over the var number in input
        }
      j++;
    }
  thebuf[j] = '\0';

  if ( (sys_getdspstate() == 1) || (strncmp(thebuf, "system", 6) == 0) )
    { // don't send if the dacs aren't turned on, unless it is a system() <------- HACK HACK HACK!
      if (x->parse_score(thebuf, j) != 0) error("problem parsing RTcmix script");
    }
}


// [openscript N] will open a buffer N
void rtcmix_openscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i,temp = 0;

  if (argc == 0)
    {
      error("rtcmix~: openscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          // JWM: No A_LONG in Pd
          //case A_LONG:
          //temp = (short)argv[i].a_w.w_long;
          //break;
        case A_FLOAT:
          temp = (short)argv[i].a_w.w_float;
        }
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
void rtcmix_setscript(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i,temp = 0;

  if (argc == 0)
    {
      error("rtcmix~: setscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          // JWM - No A_LONG in Pd
          //case A_LONG:
          //temp = (short)argv[i].a_w.w_long;
          //break;
        case A_FLOAT:
          temp = (short)argv[i].a_w.w_float;
        }
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
void rtcmix_write(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i, temp = 0;

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          //JWM: No A_LONG in Pd
          //case A_LONG:
          //temp = (short)argv[i].a_w.w_long;
          //break;
        case A_FLOAT:
          temp = (short)argv[i].a_w.w_float;
        }
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

  defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
}


// the [savescriptas] message triggers this
void rtcmix_writeas(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  short i, temp = 0;

  for (i=0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          // JWM: FIXME - No A_LONG in PD
        case A_LONG:
          temp = (short)argv[i].a_w.w_long;
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
          x->s_name[x->current_script][0] = 0;
          break;
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
          x->s_name[x->current_script][0] = 0;
          break;
        case A_SYMBOL://this doesn't work yet
          strcpy(x->s_name[x->current_script], argv[i].a_w.w_symbol->s_name);
          post("rtcmix~: writing file %s",x->s_name[x->current_script]);
        }
    }
  post("rtcmix: current script is %d", temp);

  defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
}


// deferred from the [save*] messages
void rtcmix_dowrite(t_rtcmix *x, Symbol *s, short argc, t_atom *argv)
{
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
}

// the [read ...] message triggers this
void rtcmix_read(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  defer(x, (method)rtcmix_doread, s, argc, argv); // always defer this message
}

// the deferred read
void rtcmix_doread(t_rtcmix *x, Symbol *s, short argc, t_atom *argv)
{
  char filename[256];
  short err, i, temp = 0;
  long type = 'TEXT';
  long size;
  long outtype;
  t_filehandle fh;
  t_handle script_handle;

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
          // JWM: FIXME - No A_LONG in Pd
        case A_LONG:
          temp = (short)argv[i].a_w.w_long;
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
          x->s_name[x->current_script][0] = 0;
          break;
        case A_FLOAT:
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
          temp = (short)argv[i].a_w.w_float;
          x->s_name[x->current_script][0] = 0;
          break;
        case A_SYMBOL:
          strcpy(filename, argv[i].a_w.w_symbol->s_name);
          strcpy(x->s_name[x->current_script], filename);
        }
    }


  if(!x->s_name[x->current_script][0])
    {
      //		if (open_dialog(filename, &path, &outtype, &type, 1))
      if (open_dialog(filename,  &x->path[x->current_script], &outtype, 0L, 0)) // allow all types of files

        return; //user cancelled
    }
  else
    {
      if (locatefile_extended(filename, &x->path[x->current_script], &outtype, &type, 1))
        {
          error("rtcmix~: error opening file: can't find file");
          x->s_name[x->current_script][0] = 0;
          return; //not found
        }
    }

  //we should have a valid filename at this point
  err = path_opensysfile(filename, x->path[x->current_script], &fh, READ_PERM);
  if (err)
    {
      fh = 0;
      error("error %d opening file", err);
      return;
    }

  strcpy(x->s_name[x->current_script], filename);

  sysfile_geteof(fh, &size);
  if (x->rtcmix_script[x->current_script])
    {
      sysmem_freeptr((void *)x->rtcmix_script[x->current_script]);
      x->rtcmix_script[x->current_script] = 0;
    }
  // BGG size+1 in max5 to add the terminating '\0'
  if (!(x->rtcmix_script[x->current_script] = (char *)sysmem_newptr(size+1)) || !(script_handle = sysmem_newhandle(size+1)))
    {
      error("rtcmix~: %s too big to read", filename);
      return;
    }
  else
    {
      x->rtcmix_script_len[x->current_script] = size;
      sysfile_readtextfile(fh, script_handle, size, TEXT_LB_NATIVE);
      strcpy(x->rtcmix_script[x->current_script], *script_handle);
    }
  x->rtcmix_script[x->current_script][size] = '\0'; // the max5 text editor apparently needs this
  // BGG for some reason mach-o doesn't like this one... the memory hit should be small
  //	sysmem_freehandle(*script_handle);
  sysfile_close(fh);

  return;
}


// this converts the current script to a binbuf format and sets it up to be saved and then restored
// via the rtcmix_restore() method below
void rtcmix_save(t_rtcmix *x, void *w)
{
  char *fptr, *tptr;
  char tbuf[5000]; // max 5's limit on symbol size is 32k, this is totally arbitrary on my part
  //	char *tbuf;
  int i,j,k;


  // insert the command to recreate the rtcmix~ object, with any additional vars
  binbuf_vinsert(w, "ssll", gensym("#N"), gensym("rtcmix~"), x->num_inputs, x->num_pinlets);

  for (i = 0; i < MAX_SCRIPTS; i++)
    {
      if (x->rtcmix_script[i] && (x->rtcmix_script_len[i] > 0))
        { // there is a script...
          // the reason I do this 'chunking' of restore messages is because of the 32k limit
          // I still wish Cycling had a generic, *untouched* buffer type.
          fptr = x->rtcmix_script[i];
          tptr = tbuf;
          k = 0;
          for (j = 0; j < x->rtcmix_script_len[i]; j++)
            {
              *tptr++ = *fptr++;
              if (++k >= 5000) { // 'serialize' the script
                // the 'restore' message contains script #, current buffer length, final buffer length, symbol with buffer contents
                *tptr = '\0';
                binbuf_vinsert(w, "ssllls", gensym("#X"), gensym("restore"), i, k, x->rtcmix_script_len[i], gensym(tbuf));
                tptr = tbuf;
                k = 0;
              }
            }
          // do the final one (or the only one in cases where scripts < 5000)
          *tptr = '\0';
          binbuf_vinsert(w, "ssllls", gensym("#X"), gensym("restore"), i, k, x->rtcmix_script_len[i], gensym(tbuf));
        }
    }
}


// and this gets the message set up by rtcmix_save()
void rtcmix_restore(t_rtcmix *x, Symbol *s, short argc, Atom *argv)
{
  int i;
  int bsize, fsize;
  char *fptr; // restore buf pointer is in the struct for repeated calls necessary for larger scripts (symbol size limit, see rtcmix_save())

  // script #, current buffer size, final script size, script data
  x->current_script = argv[0].a_w.w_long;
  bsize = argv[1].a_w.w_long;
  if (argv[2].a_type == A_SYMBOL)
    { // this is for v1.399 scripts that had the symbol size limit bug
      fsize = bsize;
      fptr = argv[2].a_w.w_symbol->s_name;
    }
  else
    {
      fsize = argv[2].a_w.w_long;
      fptr = argv[3].a_w.w_symbol->s_name;
    }

  if (!x->rtcmix_script[x->current_script])
    { // if the script isn't being restored already
      if (!(x->rtcmix_script[x->current_script] = (char *)sysmem_newptr(fsize+1)))
        { // fsize+1 for the '\0
          error("rtcmix~: problem allocating memory for restored script");
          return;
        }
      x->rtcmix_script_len[x->current_script] = fsize;
      x->restore_buf_ptr = x->rtcmix_script[x->current_script];
    }

  // this happy little for-loop is for older (max 4.x) rtcmix scripts.  The older version of max had some
  // serious parsing issues for saved text.  Now it all seems fixed in 5 -- yay!
  // convert the xRTCMIX_XXx tokens to their real equivalents
  for (i = 0; i < bsize; i++)
    {
      switch (*fptr)
        {
        case 'x':
          if (strncmp(fptr, "xRTCMIX_CRx", 11) == 0)
            {
              sprintf(x->restore_buf_ptr, "\r");
              fptr += 11;
              x->restore_buf_ptr++;
              break;
            }
          else if (strncmp(fptr, "xRTCMIX_DQx", 11) == 0)
            {
              sprintf(x->restore_buf_ptr, "\"");
              fptr += 11;
              x->restore_buf_ptr++;
              break;
            }
          else
            {
              *x->restore_buf_ptr++ = *fptr++;
              break;
            }
        default:
          *x->restore_buf_ptr++ = *fptr++;
        }
    }

  x->rtcmix_script[x->current_script][fsize] = '\0'; // the final '\0'

  x->current_script = 0; // do this to set script 0 as default
}
