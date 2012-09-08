// rtcmix~ v 0.01, Joel Matthys (8/2012) (Linux, Pd support), based on:
// rtcmix~ v 1.81, Brad Garton (2/2011) (OS 10.5/6, Max5 support)
// uses the RTcmix bundled executable lib, now based on RTcmix-4.0.1.6
// see http://music.columbia.edu/cmc/RTcmix for more info
//
// see rtcmix~.c in BGG's rtcmix~ for Max/MSP thank yous & changelog

// JWM - Pd headers
#include "rtcmix~.h"
#include "m_pd.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dlfcn.h>

//#define DEBUG(x) // debug off
#define DEBUG(x) x

/*** PD EXTERNAL SETUP ---------------------------------------------------------------------------***/
void rtcmix_tilde_setup(void)
{
  rtcmix_class = class_new (gensym("rtcmix~"),
                            (t_newmethod)rtcmix_tilde_new,
                            (t_method)rtcmix_free,
                            sizeof(t_rtcmix),
                            0, A_GIMME, 0);

  //standard messages; don't change these
  CLASS_MAINSIGNALIN(rtcmix_class, t_rtcmix, f);
  class_addmethod(rtcmix_class, (t_method)rtcmix_dsp, gensym("dsp"), 0);

  //our own messages
  class_addmethod(rtcmix_class,(t_method)rtcmix_version, gensym("version"), 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_rtcmix, gensym("rtcmix"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_var, gensym("var"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_varlist, gensym("varlist"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_bufset, gensym("bufset"), A_SYMBOL, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_flush, gensym("flush"), 0);

  //so we know what to do with floats that we receive at the inputs
  class_addlist(rtcmix_class, rtcmix_text); // text from [entry] comes in as list
  //class_addfloat(rtcmix_class, rtcmix_float);
  class_addbang(rtcmix_class, rtcmix_bang); // trigger scripts

  class_addmethod(rtcmix_class,(t_method)rtcmix_setscript, gensym("setscript"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_read, gensym("read"), A_GIMME, 0);
  class_addmethod(rtcmix_class,(t_method)rtcmix_save, gensym("save"), A_CANT, 0);
  // openpanel and savepanel return their messages through "callback"
  class_addmethod(rtcmix_class,(t_method)rtcmix_callback, gensym("callback"), A_SYMBOL, 0);

  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp9, gensym("pinlet9"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp8, gensym("pinlet8"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp7, gensym("pinlet7"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp6, gensym("pinlet6"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp5, gensym("pinlet5"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp4, gensym("pinlet4"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp3, gensym("pinlet3"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp2, gensym("pinlet2"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp1, gensym("pinlet1"), A_FLOAT, 0);
  class_addmethod(rtcmix_class, (t_method)rtcmix_inletp0, gensym("pinlet0"), A_FLOAT, 0);

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
}

//this gets called when the object is created; everytime the user types in new args, this will get called
//void rtcmix_new(long num_inoutputs, long num_additional)
void *rtcmix_tilde_new(t_symbol *s, int argc, t_atom *argv)
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
  DEBUG(post("creating %d inlets and outlets and %d additional inlets",num_inoutputs,num_additional););

  if (num_inoutputs < 1) num_inoutputs = 1; // no args, use default of 1 channel in/out
  if ((num_inoutputs + num_additional) > MAX_INPUTS)
    {
      num_inoutputs = 1;
      num_additional = 0;
      error("sorry, only %d total inlets are allowed!", MAX_INPUTS);
    }
  // JWM: limiting num_additional to 10
  if (num_additional > 10)
    num_additional = 10;

  x->num_inputs = num_inoutputs;
  x->num_outputs = num_inoutputs;
  x->num_pinlets = num_additional;

  // setup up inputs and outputs, for audio inputs

  // SIGNAL INLETS
  for (i=0; i < x->num_inputs-1; i++)
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

  // FLOAT INLETS (for pfields)
  // JWM: This is a hack, since I couldn't find a way to identify a float
  // passed to a single method by inlet id. So I'm limiting the number of
  // inlets to 10 (not sure why we'd need more anyway) and passing each to
  // gensym "pinlet0", "pinlet1", etc.
  char* inletname = malloc(8);
  for (i=0; i< x->num_pinlets; i++)
    {
      sprintf(inletname, "pinlet%d", i);
      inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym(inletname));
    }
  free(inletname);

  // SIGNAL OUTLETS
  for (i = 0; i < x->num_outputs; i++)
    {
      // outputs, right-to-left
      outlet_new(&x->x_obj, gensym("signal"));
    }

  // OUTLET FOR BANGS
  x->outpointer = outlet_new(&x->x_obj, &s_bang);

  // initialize some variables; important to do this!
  for (i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      x->in[i] = 0.;
      x->in_connected[i] = 0;
    }

  // set up for the variable-substitution scheme
  x->var_array = malloc(NVARS * sizeof(float));
  x->var_set = malloc(NVARS * sizeof(short));
  for(i = 0; i < NVARS; i++)
    {
      x->var_set[i] = 0;
      x->var_array[i] = 0.0;
    }

  // the text editor

  // JWM: internal script storage is handled with binbufs
  x->current_script = 0;
  x->rw_flag = RTcmixREADFLAG;
  for (i=0; i<MAX_SCRIPTS; i++)
    {
      x->rtcmix_script[i]=binbuf_new();
    }

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

  post("rtcmix~ -- RTcmix music language, v. %s (%s)", VERSION, RTcmixVERSION);

  return (x);
}

static void load_dylib(t_rtcmix* x)
{
  // using Pd's open_via_path to find rtcmix~, and from there rtcmixdylib.so
  char temp_path[MAXPDSTRING], *pathptr;
  int fd = -1;
  fd = open_via_path(".","rtcmix~.pd_linux","",temp_path, &pathptr, MAXPDSTRING,1);
  if (fd < 0)
    error ("open_via_path() failed!");
  // JWM: TODO: add backup which uses canvas path
  else
    {
      sprintf(mpathname,"%s/dylib/",temp_path);
    }

  // BGG kept this in for the dlopen() stuff

  //zero out the struct, to be careful (takk to jkclayton)
  // JWM: TROUBLE! somehow this is zeroing out num_output too
  // I suspect it's because not all of the objects are the
  // same size; I'm going to disable this and see if it
  // causes any trouble in the long run...

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

  x->pathname = malloc(MAXPDSTRING);
  // full path to the rtcmixdylib.so file
  sprintf(x->pathname, "%srtcmixdylib%d.so", mpathname, x->dylibincr);

  char cp_command[MAXPDSTRING];

  // ok, this is fairly insane.  To guarantee a fully-isolated namespace with dlopen(), we need
  // a totally *unique* dylib, so we copy this.  Deleted in rtcmix_free() below
  // RTLD_LOCAL doesn't do it all - probably the global vars in RTcmix
  sprintf(cp_command, "cp \"%srtcmixdylib.so\" \"%s\"",mpathname,x->pathname);
  if (system(cp_command)) error("error creating unique dylib copy");

  rtcmix_dlopen_and_errorcheck(x);

}

//this gets called everytime audio is started; even when audio is running, if the user
//changes anything (like deletes a patch cord), audio will be turned off and
//then on again, calling this func.
//this adds the "perform" method to the DSP chain, and also tells us
//where the audio vectors are and how big they are
void rtcmix_dsp(t_rtcmix *x, t_signal **sp)
{
  // This is 2 because (for some totally crazy reason) the
  // signal vector starts at 1, not 0
  t_int dsp_add_args [x->num_inputs + x->num_outputs + 2];
  t_int vector_size = sp[0]->s_n;
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
  x->srate = sys_getsr();

  // check to see if there are signals connected to the various inputs
  // JWM: AFAIK, this isn't possible with Pd.
  //for(i = 0; i < (x->num_inputs + x->num_pinlets); i++) x->in_connected[i] = count[i];

  // construct the array of vectors and stuff
  dsp_add_args[0] = (t_int)x; //the object itself
  for(i = 0; i < (x->num_inputs + x->num_outputs); i++)
    { //pointers to the input and output vectors
      dsp_add_args[i+1] = (t_int)sp[i]->s_vec;
    }

  dsp_add_args[x->num_inputs + x->num_outputs + 1] = vector_size; //pointer to the vector size

  DEBUG(post("vector size: %d",vector_size););
  //DEBUG(post("num inputs: %i, num outputs: %i", x->num_inputs, x->num_outputs););

  dsp_addv(rtcmix_perform, (x->num_inputs  + x->num_outputs + 2),(t_int*)dsp_add_args); //add them to the signal chain

  // reload, this reinits the RTcmix queue, etc.
  dlclose(x->rtcmixdylib);

  // load the dylib
  rtcmix_dlopen_and_errorcheck(x);

  // allocate the RTcmix i/o transfer buffers
  x->pd_inbuf = malloc(sizeof(float) * vector_size * x->num_inputs);
  x->pd_outbuf = malloc(sizeof(float) * vector_size * x->num_outputs);

  // zero out these buffers for UB
  for (i = 0; i < (vector_size * x->num_inputs); i++) x->pd_inbuf[i] = 0.0;
  for (i = 0; i < (vector_size * x->num_outputs); i++) x->pd_outbuf[i] = 0.0;

  if (x->pd_rtsetparams)
    {
      x->pd_rtsetparams(x->srate, x->num_outputs, vector_size, x->pd_inbuf, x->pd_outbuf, x->theerror);
    }
}


//this is where the action is
//we get vectors of samples (n samples per vector), process them and send them out
t_int *rtcmix_perform(t_int *w)
{
  t_rtcmix *x = (t_rtcmix *)(w[1]);

  float *in[MAX_INPUTS];          //pointers to the input vectors
  float *out[MAX_OUTPUTS];	//pointers to the output vectors

  t_int n = w[x->num_inputs + x->num_outputs + 2]; //number of samples per vector

  //random local vars
  int i, j, k;

  // stuff for check_vals() and check_error()
  int valflag;
  int errflag;

  //BGG: check to see if we have a signal or float message connected to input
  //then assign the pointer accordingly
  // JWM: for now at least, all sig inlets and only sig inlets are assigned
  for (i = 0; i < (x->num_inputs + x->num_pinlets); i++)
    {
      in[i] = (float *)(w[i+2]);
    }

  //assign the output vectors
  for (i = 0; i < x->num_outputs; i++)
    {
      // this results in reversed L-R image but I'm
      // guessing it's the same in Max
      out[i] = (float *)( w[x->num_inputs + i + 2 ]);
    }

  j = 0;
  k = 0;

  while (n--)
    {	//this is where the action happens.....
      for(i = 0; i < x->num_inputs; i++)
        (x->pd_inbuf)[k++] = *in[i]++;

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
  return w + x->num_inputs + x->num_outputs + 3;
}

// here's my free function
void rtcmix_free(t_rtcmix *x)
{
  // BGG kept dlopen() stuff in in case NSLoad gets dropped
  char *rm_command = malloc(MAXPDSTRING);

    dlclose(x->rtcmixdylib);
    sprintf(rm_command, "rm -rf \"%s\" ", x->pathname);
    if (system(rm_command)) error("error deleting unique dylib");

    free(x->canvas_path);
    free(x->pd_inbuf);
    free(x->pd_outbuf);
    free(x->var_array);
    free(x->var_set);

    int i;
    for (i=0; i<MAX_SCRIPTS; i++)
      {
        binbuf_free(x->rtcmix_script[i]);
      }

    free(x->pathname);
    free(rm_command);
    DEBUG(post ("rtcmix~ DESTROYED!"););
}

// bang triggers the current working script
void rtcmix_bang(t_rtcmix *x)
{
  t_atom a[1];

  if (x->flushflag == 1) return; // heap and queue being reset

  rtcmix_goscript(x, NULL, 1, a);
}


// print out the rtcmix~ version
void rtcmix_version(t_rtcmix *x)
{
  post("rtcmix~, v. %s by Joel Matthys (%s)", VERSION, RTcmixVERSION);
  outlet_bang(x->outpointer);
}

// JWM: In Pd, this comes from [entry] as a list
void rtcmix_text(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  short i, varnum;
  char thebuf[MAXPDSTRING]; // should #define these probably
  char xfer[MAXPDSTRING];
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
void rtcmix_badquotes(char *cmd, char *thebuf)
{
  int i;
  char *rtinputptr;
  int badquotes, checkon;
  int clen;
  char tbuf[MAXPDSTRING];

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
void rtcmix_rtcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  //defer_low(x, (method)rtcmix_dortcmix, s, argc, argv); // always defer this message
  rtcmix_dortcmix(x,s,argc,argv);
}


// what to do when we get the message "rtcmix"
// used for single-shot RTcmix scorefile commands
void rtcmix_dortcmix(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i;
  double p[1024]; // should #define this probably
  char *cmd = NULL;

  for (i = 0; i < argc; i++)
    {
      switch (argv[i].a_type)
        {
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
void rtcmix_var(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i, varnum;

  for (i = 0; i < argc; i += 2)
    {
      varnum = (short)argv[i].a_w.w_float;
      if ( (varnum < 1) || (varnum > NVARS) )
        {
          error("only vars $1 - $9 are allowed");
          return;
        }
      x->var_set[varnum-1] = 1;
      if (argv[i+1].a_type == A_FLOAT)
          x->var_array[varnum-1] = argv[i+1].a_w.w_float;
    }
  DEBUG( post("vars: %f %f %f %f %f %f %f %f %f",
              (float)(x->var_array[0]),
              (float)(x->var_array[1]),
              (float)(x->var_array[2]),
              (float)(x->var_array[3]),
              (float)(x->var_array[4]),
              (float)(x->var_array[5]),
              (float)(x->var_array[6]),
              (float)(x->var_array[7]),
              (float)(x->var_array[8])););

}


// the "varlist" message allows us to set $n variables imbedded in a scorefile with a list of positional vars
void rtcmix_varlist(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
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
      if (argv[i].a_type == A_FLOAT)
        x->var_array[i] = argv[i].a_w.w_float;
    }
}


// the "bufset" message allows access to a [buffer~] object.  The only argument is the name of the [buffer~]
void rtcmix_bufset(t_rtcmix *x, t_symbol *s)
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
void rtcmix_flush(t_rtcmix *x)
{
  x->flushflag = 1; // set a flag, the flush will happen in perform after pulltraverse()
}


// here is the text-editor buffer stuff, go dan trueman go!
// used for rtcmix~ internal buffers
void rtcmix_edclose (t_rtcmix *x, char **text, long size)
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


void rtcmix_okclose (t_rtcmix *x, char *prompt, short *result)
{
  //*result = 3; //don't put up dialog box
  //return;
}


// open up an ed window on the current buffer
void rtcmix_dblclick(t_rtcmix *x)
{
  DEBUG(post("DOUBLE CLICK!!!"););
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

// the [goscript N] message will cause buffer N to be sent to the RTcmix parser
void rtcmix_goscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  short i;
  short temp = 0;
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
  char *origbuf = malloc(MAXSCRIPTSIZE);
  int natoms = binbuf_getnatom(x->rtcmix_script[x->current_script]);

  binbuf_gettext(x->rtcmix_script[x->current_script], &origbuf, &natoms);

  if (natoms==0)
    error("rtcmix~: you are triggering a 0-length script!");

  int buflen = strlen(origbuf);
  DEBUG(post("buffer length: %i",buflen););

  // probably don't need to transfer to a new buffer, but I want to be sure there's room for the \0,
  // plus the substitution of \n for those annoying ^M thingies
  for (i = 0, j = 0; i < buflen; i++)
    {
      thebuf[j] = *(origbuf+i);
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
  free(origbuf);
}

// [openscript N] will open a buffer N
// JWM: this will requires the hammereditor
void rtcmix_openscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
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
void rtcmix_setscript(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
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
void rtcmix_write(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  /*
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
  DEBUG(post("rtcmix: current script is %d", temp););

  //defer(x, (method)rtcmix_dowrite, s, argc, argv); // always defer this message
  rtcmix_dowrite(x,s,argc,argv);
  */
}


// the [savescriptas] message triggers this
void rtcmix_writeas(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
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
void rtcmix_dowrite(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
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
void rtcmix_read(t_rtcmix *x, t_symbol *s, short argc, t_atom *argv)
{
  int i;
  int temp = 0;
  t_symbol *filename = malloc(MAXPDSTRING);
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

  x->rtcmix_script[x->current_script] = binbuf_new();

  if (!fnflag)
    {
      DEBUG(post("openpanel signaled"););
      x->rw_flag = RTcmixREADFLAG;
      DEBUG(post("x->x_s->s_name: %s, x->canvas_path->s_name: %s",x->x_s->s_name, x->canvas_path->s_name););
      sys_vgui("pdtk_openpanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
    }
  else
    {
  rtcmix_callback(x,filename);
    }
  free(filename);
}

void rtcmix_save(t_rtcmix *x, void *w)
{
  DEBUG(post("savepanel signaled"););
  x->rw_flag = RTcmixWRITEFLAG;
  sys_vgui("pdtk_savepanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
}

void rtcmix_callback(t_rtcmix *x, t_symbol *filename)
{
  DEBUG(post("callback! flag = %d",x->rw_flag););

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
      t_atom eof;
      t_symbol *eof_marker = gensym("\0");
      SETSYMBOL(&eof,eof_marker);
      binbuf_add(x->rtcmix_script[x->current_script], 1,&eof );
    }
  free(buf);
}

static void rtcmix_inletp0(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 0",f););
  rtcmix_float(x,0,f);
}

static void rtcmix_inletp1(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 1",f););
  rtcmix_float(x,1,f);
}
static void rtcmix_inletp2(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 2",f););
  rtcmix_float(x,2,f);
}
static void rtcmix_inletp3(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 3",f););
  rtcmix_float(x,3,f);
}
static void rtcmix_inletp4(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 4",f););
  rtcmix_float(x,4,f);
}
static void rtcmix_inletp5(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 5",f););
  rtcmix_float(x,5,f);
}
static void rtcmix_inletp6(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 6",f););
  rtcmix_float(x,6,f);
}
static void rtcmix_inletp7(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 7",f););
  rtcmix_float(x,7,f);
}
static void rtcmix_inletp8(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 8",f););
  rtcmix_float(x,8,f);
}
static void rtcmix_inletp9(t_rtcmix *x, t_float f)
{
  DEBUG(post("received %f at pinlet 9",f););
  rtcmix_float(x,9,f);
}

static void rtcmix_float(t_rtcmix *x, short inlet, t_float f)
{
  //check to see which input the float came in, then set the appropriate variable value
  if (inlet >= x->num_inputs)
    {
      x->in[inlet] = f;
      post("rtcmix~: setting in[%d] =  %f, but rtcmix~ doesn't use this", inlet, f);
    }
  else x->pfield_set(inlet+1, f);
}

static void rtcmix_dlopen_and_errorcheck(t_rtcmix *x)
{
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
}
