// chuck~ v 0.11, Joel Matthys (8/2012) (Linux, Pd support), based on:
// chuck~ v 1.81, Brad Garton (2/2011) (OS 10.5/6, Max5 support)
// uses the Chuck bundled executable lib, now based on Chuck-4.0.1.6
// see http://music.columbia.edu/cmc/Chuck for more info

#define VERSION "0.11"
#define ChuckVERSION "ChucK-pd-4.0.1.6"

// JWM - Pd headers
#include "chuck~.h"
#include "m_pd.h"
#include "m_imp.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dlfcn.h>

          //#define DEBUG(x) // debug off
#define DEBUG(x) x

/*** PD EXTERNAL SETUP ---------------------------------------------------------------------------***/
void chuck_tilde_setup(void)
{
  chuck_class = class_new (gensym("chuck~"),
                            (t_newmethod)chuck_tilde_new,
                            (t_method)chuck_free,
                            sizeof(t_chuck),
                            0, A_GIMME, 0);

  //standard messages; don't change these
  CLASS_MAINSIGNALIN(chuck_class, t_chuck, f);
  class_addmethod(chuck_class, (t_method)chuck_dsp, gensym("dsp"), 0);

  //our own messages
  class_addmethod(chuck_class,(t_method)chuck_version, gensym("version"), 0);
  class_addmethod(chuck_class,(t_method)chuck_chuck, gensym("chuck"), A_GIMME, 0);
  class_addmethod(chuck_class,(t_method)chuck_var, gensym("var"), A_GIMME, 0);
  class_addmethod(chuck_class,(t_method)chuck_varlist, gensym("varlist"), A_GIMME, 0);
  class_addmethod(chuck_class,(t_method)chuck_flush, gensym("flush"), 0);

  //so we know what to do with floats that we receive at the inputs
  class_addlist(chuck_class, chuck_text); // text from [entry] comes in as list
  class_addfloat(chuck_class, chuck_float);
  class_addbang(chuck_class, chuck_bang); // trigger scripts
  class_addmethod(chuck_class,(t_method)chuck_goscript, gensym("goscript"), A_FLOAT, 0);
  class_addmethod(chuck_class,(t_method)chuck_openeditor, gensym("click"), 0);
  class_addmethod(chuck_class,(t_method)chuck_setscript, gensym("setscript"), A_GIMME, 0);
  class_addmethod(chuck_class,(t_method)chuck_read, gensym("read"), A_GIMME, 0);
  class_addmethod(chuck_class,(t_method)chuck_read, gensym("load"), A_GIMME, 0);
  class_addmethod(chuck_class,(t_method)chuck_save, gensym("save"), 0);
  class_addmethod(chuck_class,(t_method)chuck_saveas, gensym("saveas"), 0);
  class_addmethod(chuck_class,(t_method)chuck_save, gensym("write"), 0);
  // openpanel and savepanel return their messages through "callback"
  class_addmethod(chuck_class,(t_method)chuck_callback, gensym("callback"), A_SYMBOL, 0);

  class_addmethod(chuck_class, (t_method)chuck_inletp9, gensym("pinlet9"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp8, gensym("pinlet8"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp7, gensym("pinlet7"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp6, gensym("pinlet6"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp5, gensym("pinlet5"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp4, gensym("pinlet4"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp3, gensym("pinlet3"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp2, gensym("pinlet2"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp1, gensym("pinlet1"), A_FLOAT, 0);
  class_addmethod(chuck_class, (t_method)chuck_inletp0, gensym("pinlet0"), A_FLOAT, 0);
}

//this gets called when the object is created; everytime the user types in new args, this will get called
//void chuck_new(long num_inoutputs, long num_additional)
void *chuck_tilde_new(t_symbol *s, int argc, t_atom *argv)
{
  // creates the object
  t_chuck *x = (t_chuck *)pd_new(chuck_class);

  // using Pd's open_via_path to find chuck~, and from there chuckdylib.so
  //char temp_path[MAXPDSTRING]; //, *pathptr;
  char *externdir = chuck_class->c_externdir->s_name;
  DEBUG(post("dir: %s %i %i",externdir, strlen(externdir),MAXPDSTRING););
  sprintf(mpathname, "%s/dylib/", externdir);
  //int fd = -1;
  //fd = open_via_path(".", CHUCKEXTERNALNAME, "", temp_path, &pathptr, MAXPDSTRING,0);

  DEBUG(post("external path: %s",mpathname););
  /*
  if(x)
    {
      unsigned int j;
      for(j=sizeof(t_object);j<sizeof(t_chuck);j++)
        ((char *)x)[j]=0;
    }
  */
  x->dylibincr = dylibincr++; // keep track of chuckdylibN.so for copy/load

  // full path to the chuckdylib.so file
  sprintf(x->pathname, "%schuckdylib%d.so", mpathname, x->dylibincr);

  char cp_command[MAXPDSTRING];

  // ok, this is fairly insane.  To guarantee a fully-isolated namespace with dlopen(), we need
  // a totally *unique* dylib, so we copy this.  Deleted in chuck_free() below
  // RTLD_LOCAL doesn't do it all - probably the global vars in Chuck
  sprintf(cp_command, "cp \"%schuckdylib.so\" \"%s\"",mpathname,x->pathname);
  if (system(cp_command)) error("error creating unique dylib copy");

  chuck_dlopen_and_errorcheck(x);

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
  x->current_script = 0;
  x->rw_flag = ChuckREADFLAG;

  x->chuck_script = malloc(MAX_SCRIPTS * MAXSCRIPTSIZE);

  for (i=0; i<MAX_SCRIPTS; i++)
    {
      x->chuck_script[i] = malloc(MAXSCRIPTSIZE);
      sprintf(x->script_name[i],"temp script %i",i);
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

  x->flushflag = 0; // [flush] sets flag for call to x->flush() in chuck_perform() (after pulltraverse completes)

  // JWM: since Pd has no decent text editor, I created a simple Text GUI object in
  // Python. It reads temp.sco and rewrites when it's altered. [chuck~] reads that
  // temp.sco file, so we need to be sure it exists.

  x->editor_flag = UNEDITED_SCRIPT;
  sprintf(x->tempfile_path,"%stemp.sco",mpathname);
  for (i=0; i<MAX_SCRIPTS; i++)
    x->script_size[i] = 0;

  post("chuck~ -- Chuck music language, v. %s (%s)", VERSION, ChuckVERSION);

  return (x);
}

static void load_dylib(t_chuck* x)
{
  // using Pd's open_via_path to find chuck~, and from there chuckdylib.so
  //char temp_path[MAXPDSTRING]; //, *pathptr;
  /*
  char *temp_path;//, *pathptr;
  temp_path = malloc(MAXPDSTRING);
  temp_path = chuck_class->c_externdir->s_name;
  sprintf(mpathname,"%s/dylib/",temp_path);
  //int fd = -1;
  //fd = open_via_path(".", CHUCKEXTERNALNAME, "", temp_path, &pathptr, MAXPDSTRING,0);

  DEBUG(post("external path: %s",temp_path););
  free(temp_path);
  if (fd < 0)
    error ("open_via_path() failed!");
  // JWM: TODO: add backup which uses canvas path
  else
    {
      sys_close(fd);
      sprintf(mpathname,"%s/dylib/",temp_path);
    }
  // BGG kept this in for the dlopen() stuff

  //zero out the struct, to be careful (takk to jkclayton)
  // JWM - no need for null check; just zero it out anyway.
  if(x)
    {
      unsigned int j;
      for(j=sizeof(t_object);j<sizeof(t_chuck);j++)
        ((char *)x)[j]=0;
    }

  x->dylibincr = dylibincr++; // keep track of chuckdylibN.so for copy/load

  x->pathname = malloc(MAXPDSTRING);
  // full path to the chuckdylib.so file
  sprintf(x->pathname, "%schuckdylib%d.so", mpathname, x->dylibincr);

  char cp_command[MAXPDSTRING];

  // ok, this is fairly insane.  To guarantee a fully-isolated namespace with dlopen(), we need
  // a totally *unique* dylib, so we copy this.  Deleted in chuck_free() below
  // RTLD_LOCAL doesn't do it all - probably the global vars in Chuck
  sprintf(cp_command, "cp \"%schuckdylib.so\" \"%s\"",mpathname,x->pathname);
  if (system(cp_command)) error("error creating unique dylib copy");

  chuck_dlopen_and_errorcheck(x);
  */

}

//this gets called everytime audio is started; even when audio is running, if the user
//changes anything (like deletes a patch cord), audio will be turned off and
//then on again, calling this func.
//this adds the "perform" method to the DSP chain, and also tells us
//where the audio vectors are and how big they are
void chuck_dsp(t_chuck *x, t_signal **sp)
{
  // This is 2 because (for some totally crazy reason) the
  // signal vector starts at 1, not 0
  t_int dsp_add_args [x->num_inputs + x->num_outputs + 2];
  t_int vector_size = sp[0]->s_n;
  int i;
  unsigned int j;

  // Chuck vars
  // these are the entry function pointers in to the chuckdylib.so lib
  x->chuckmain = NULL;
  x->pull_cb2 = NULL;
  x->parse_score = NULL;
  x->check_bang = NULL;
  x->check_vals = NULL;
  x->inlet_set = NULL;

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

  dsp_addv(chuck_perform, (x->num_inputs  + x->num_outputs + 2),(t_int*)dsp_add_args); //add them to the signal chain

  // reload, this reinits the Chuck queue, etc.
  if (x->chuckdylib)
    dlclose(x->chuckdylib);

  // load the dylib
  chuck_dlopen_and_errorcheck(x);

  // allocate the Chuck i/o transfer buffers
  x->pd_inbuf = malloc(sizeof(float) * vector_size * x->num_inputs);
  x->pd_outbuf = malloc(sizeof(float) * vector_size * x->num_outputs);

  // zero out these buffers for UB
  for (i = 0; i < (vector_size * x->num_inputs); i++) x->pd_inbuf[i] = 0.0;
  for (i = 0; i < (vector_size * x->num_outputs); i++) x->pd_outbuf[i] = 0.0;
}


//this is where the action is
//we get vectors of samples (n samples per vector), process them and send them out
t_int *chuck_perform(t_int *w)
{
  t_chuck *x = (t_chuck *)(w[1]);

  float *in[MAX_INPUTS];   //pointers to the input vectors
  float *out[MAX_OUTPUTS]; //pointers to the output vectors

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

  // ChucK stuff
  // this drives the sample-computing input and output in chuck
  x->pull_cb2(x->pd_outbuf, n);

  // look for a pending bang from MAXBANG()
  if (x->check_bang() == 1) // JWM: no defer in Pd, and BGG says unnecessary anyway
    {
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

  /*
  errflag = x->check_error();
  if (errflag != 0)
    {
      if (errflag == 1) post("Chuck: %s", x->theerror);
      else error("Chuck: %s", x->theerror);
    }

  // reset queue and heap if signalled
  if (x->flushflag == 1)
    {
      x->flush();
      x->flushflag = 0;
    }
  */

  //return a pointer to the next object in the signal chain.
  return w + x->num_inputs + x->num_outputs + 3;
}

// here's my free function
void chuck_free(t_chuck *x)
{
  // BGG kept dlopen() stuff in in case NSLoad gets dropped
  char rm_command[MAXPDSTRING];

    dlclose(x->chuckdylib);
    sprintf(rm_command, "rm -rf \"%s\" ", x->pathname);
    if (system(rm_command))
      error("error deleting unique dylib");

    free(x->pd_inbuf);
    free(x->pd_outbuf);
    free(x->var_array);
    free(x->var_set);

    int i;
    for (i=0; i<MAX_SCRIPTS; i++)
      {
        free(x->chuck_script[i]);
        sprintf(x->script_name[i],"0");
      }

    free(x->chuck_script);

    DEBUG(post ("chuck~ DESTROYED!"););
}

static void chuck_openeditor(t_chuck *x)
{
  x->editor_flag = EDITED_SCRIPT;
  char *editor_cmd = malloc(MAXPDSTRING);
  sprintf(editor_cmd, "python %schuck_scripteditor.py %s &",mpathname,x->tempfile_path);
  DEBUG(post("cmd: %s",editor_cmd););
  if (system(editor_cmd))
    error("chuck~: can't open chuck script editor");
  free(editor_cmd);
}

// bang triggers the current working script
void chuck_bang(t_chuck *x)
{
  DEBUG(post("chuck~: received bang"););

  if (x->flushflag == 1) return; // heap and queue being reset

  t_atom *argv = getbytes(0);
  chuck_goscript(x, x->current_script);
}

void chuck_float(t_chuck *x, t_float scriptnum)
{
  DEBUG(post("received float %f",scriptnum););
  chuck_goscript(x, scriptnum);
}


// print out the chuck~ version
void chuck_version(t_chuck *x)
{
  post("chuck~, v. %s by Joel Matthys (%s)", VERSION, ChuckVERSION);
  outlet_bang(x->outpointer);
}

// JWM: In Pd-extended, one-shot commands from [flatgui/entry] as a list
void chuck_text(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  short i, varnum;
  char thebuf[MAXPDSTRING];
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
          if ( x->var_set[varnum-1]==0 ) error("variable $%d has not been set yet, using 0.0 as default",varnum);
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
  // certain specific chuck commands.  Not really elegant, but I don't know of another solution at present
 // JWM - TODO - what if, upon getting quotes, we jump to a separate routine until we get close quotes? Then jump back.
  chuck_badquotes("rtinput", thebuf);
  chuck_badquotes("system", thebuf);
  chuck_badquotes("dataset", thebuf);

  if (x->parse_score(thebuf) != 0) error("problem parsing Chuck script");
}


// see the note in chuck_dotext() about why I have to do this
void chuck_badquotes(char *cmd, char *thebuf)
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

    // and this splices the modified, happily-quoted-param buffer back to the buf we give to chuck
    strcpy(tbuf, rtinputptr);
    *rtinputptr++ = 34;
    strcpy(rtinputptr, tbuf);
  }
}


// see the note for chuck_dochuck() below
void chuck_chuck(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  chuck_dochuck(x,s,argc,argv);
}


// what to do when we get the message "chuck"
// used for single-shot Chuck scorefile commands
void chuck_dochuck(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
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

  // JWM: not sure what's supposed to happen here...
  //chuck_text(cmd, p, argc-1, NULL);
}


// the "var" message allows us to set $n variables imbedded in a scorefile with varnum value messages
void chuck_var(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
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
void chuck_varlist(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
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

// the "flush" message
void chuck_flush(t_chuck *x)
{
  x->flushflag = 1; // set a flag, the flush will happen in perform after pulltraverse()
}

void chuck_goscript(t_chuck *x, t_float f)
{
  DEBUG(post("goscript"););
  // JWM reload temp.sco if editor has been open
  if (x->editor_flag == EDITED_SCRIPT)
    {
      chuck_doread(x,x->tempfile_path);
      x->editor_flag = UNEDITED_SCRIPT;
    }
  short i;
  short temp =   temp = (short)f;

  if (temp > MAX_SCRIPTS)
    {
      error("chuck~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
      temp = MAX_SCRIPTS-1;
    }
  if (temp < 0)
    {
      error("chuck~: the script number should be > 0!  Resetting to script number 0");
      temp = 0;
    }

  x->current_script = (t_int)temp;
  DEBUG(post("current script = %i",x->current_script););

  short j;
  int tval;

  int buflen = x->script_size[x->current_script];

  for (j = buflen-1; j>=0; j--)
    {
      if ((int)x->chuck_script[x->current_script][j]==0)
        buflen--;
      else
        break;
    }
  char *thebuf = malloc(buflen);

  DEBUG(post("buflen: %i",buflen););

      // JWM: HEAVY DUTY BUFFER DEBUGGER
  DEBUG(
        j = buflen;
        i = 0;
        int linenum = 1;
        while (i < j)
          {
            if (j - i >= 10)
              {
                int foo;
                for (foo=0; foo<10; foo++)
                  {
                    if ((int)x->chuck_script[x->current_script][i+foo] == 10)
                      linenum++;
                  }
                //post("orig: %i: chars: %c %c %c %c %c %c %c %c %c %c",
                     post("orig: %i: chars: %i %i %i %i %i %i %i %i %i %i",
                     linenum, x->chuck_script[x->current_script][i], x->chuck_script[x->current_script][i+1], x->chuck_script[x->current_script][i+2],
                     x->chuck_script[x->current_script][i+3], x->chuck_script[x->current_script][i+4], x->chuck_script[x->current_script][i+5],
                     x->chuck_script[x->current_script][i+6], x->chuck_script[x->current_script][i+7], x->chuck_script[x->current_script][i+8],
                     x->chuck_script[x->current_script][i+9]);
                post("line: %i: chars: %c %c %c %c %c %c %c %c %c %c",
                     //post("line: %i: chars: %i %i %i %i %i %i %i %i %i %i",
                     linenum, x->chuck_script[x->current_script][i], x->chuck_script[x->current_script][i+1], x->chuck_script[x->current_script][i+2],
                     x->chuck_script[x->current_script][i+3], x->chuck_script[x->current_script][i+4], x->chuck_script[x->current_script][i+5],
                     x->chuck_script[x->current_script][i+6], x->chuck_script[x->current_script][i+7], x->chuck_script[x->current_script][i+8],
                     x->chuck_script[x->current_script][i+9]);
                i += 10;
              }
            else
              {
                if ((int)x->chuck_script[x->current_script][i] == 10)
                  linenum++;
                post("orig: %i chars: %i", linenum, x->chuck_script[x->current_script][i++]);
                post("line: %i chars: %c", linenum, x->chuck_script[x->current_script][i++]);
              }
          });


  // probably don't need to transfer to a new buffer, but I want to be sure there's room for the \0,
  // plus the substitution of \n for those annoying ^M thingies
  if (buflen<=0)
    {
      error("chuck~: you are triggering a 0-length script!");
    }
  else
    {
      for (i = 0, j = 0; i < buflen; i++)
        {
          thebuf[j] = *(x->chuck_script[x->current_script]+i);
          if ((int)thebuf[j] == 13) thebuf[j] = 10; // Chuck wants newlines, not <cr>'s

          // ok, here's where we substitute the $vars
          if ((int)thebuf[j] == 36)
            {
              sscanf(x->chuck_script[x->current_script]+i+1, "%d", &tval);
              if ( !(x->var_set[tval-1]) )
                error("variable $%d has not been set yet, using 0.0 as default", tval);
              sprintf(thebuf+j, "%f", x->var_array[tval-1]);
              j = strlen(thebuf)-1;
              i++; // skip over the var number in input
            }
          j++;
        }

      // JWM: HEAVY DUTY BUFFER DEBUGGER

      DEBUG(
      i = 0;
      linenum = 1;
      while (i < j)
        {
          if (j - i >= 10)
            {
              int foo;
              for (foo=0; foo<10; foo++)
                {
                  if ((int)thebuf[i+foo] == 10)
                    linenum++;
                }
              //post("fix: %i: chars: %c %c %c %c %c %c %c %c %c %c",
                   post("fix: %i: chars: %i %i %i %i %i %i %i %i %i %i",
                   linenum, thebuf[i], thebuf[i+1], thebuf[i+2],
                   thebuf[i+3], thebuf[i+4], thebuf[i+5],
                   thebuf[i+6], thebuf[i+7], thebuf[i+8],
                   thebuf[i+9]);
              i += 10;
            }
          else
            {
              if ((int)thebuf[i] == 10)
                linenum++;
              post("line: %i chars: %i", linenum, thebuf[i++]);
            }
        });

      if ( (canvas_dspstate == 1) || (strncmp(thebuf, "system", 6) == 0) )
        { // don't send if the dacs aren't turned on, unless it is a system() <------- HACK HACK HACK!

          post ("chuck~: parsing script %i: \"%s\"",x->current_script,x->script_name[x->current_script]);

          if (x->parse_score(thebuf) != 0)
            error("possible problem parsing Chuck script");
        }
      else
        error ("chuck~: can't parse score with DSP off");
    }
  free(thebuf);
}

// [setscript N] will set the currently active script to N
void chuck_setscript(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
{
  short i,temp = 0;

  if (argc == 0)
    {
      error("chuck~: setscript needs a buffer number [0-19]");
      return;
    }

  for (i = 0; i < argc; i++)
    {
      if (argv[i].a_type == A_FLOAT)
          temp = (short)argv[i].a_w.w_float;
    }

  if (temp > MAX_SCRIPTS)
    {
      error("chuck~: only %d scripts available, setting to script number %d", MAX_SCRIPTS, MAX_SCRIPTS-1);
      temp = MAX_SCRIPTS-1;
    }
  if (temp < 0)
    {
      error("chuck~: the script number should be > 0!  Resetting to script number 0");
      temp = 0;
    }

  x->editor_flag==EDITED_SCRIPT;
  //chuck_doread(x, x->tempfile_path);
  x->current_script = (t_int)temp;
  post("chuck~: set current script to %i: \"%s\"", x->current_script, x->script_name[x->current_script]);
  chuck_dosave(x,x->tempfile_path);
}

// the [read ...] message triggers this
void chuck_read(t_chuck *x, t_symbol *s, short argc, t_atom *argv)
{
  DEBUG(post("read"););
  int i;
  int temp = x->current_script;
  x->editor_flag = UNEDITED_SCRIPT;
  t_symbol *filename;
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
      error("chuck~: the script number must be [0-19]. Setting to 0.");
      temp = 0;
    }
  if (temp>(MAX_SCRIPTS-1))
    {
      error("chuck~: the script number must be [0-19]. Setting to 19.");
      temp = (int)(MAX_SCRIPTS-1);
    }
  x->current_script = (t_int)temp;

  for (i=0; i<MAXSCRIPTSIZE; i++)
    x->chuck_script[x->current_script][i] = 0;

  if (!fnflag)
    {
      DEBUG(post("openpanel signaled"););
      x->rw_flag = ChuckREADFLAG;
      DEBUG(post("x->x_s->s_name: %s, x->canvas_path->s_name: %s",x->x_s->s_name, x->canvas_path->s_name););
      sys_vgui("pdtk_openpanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
    }
  else
    {
      char fullpath[MAXPDSTRING];
      sprintf(fullpath,"%s/%s",x->canvas_path->s_name, filename->s_name);
      chuck_callback(x,gensym(fullpath));
  chuck_callback(x,filename);
    }
}

void chuck_save(t_chuck *x)
{
  DEBUG(post("save signaled"););
  chuck_dosave(x, x->script_name[x->current_script]);
}

void chuck_saveas(t_chuck *x)
{
  DEBUG(post("savepanel signaled"););
  x->rw_flag = ChuckWRITEFLAG;
  sys_vgui("pdtk_savepanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
}

void chuck_callback(t_chuck *x, t_symbol *filename)
{
  DEBUG(post("callback! flag = %d",x->rw_flag););
  DEBUG(post("current script: %i",x->current_script););
  char *buf = malloc(MAXPDSTRING);
  sprintf(x->script_name[x->current_script], "%s",filename->s_name);
  switch (x->rw_flag)
    {
    case ChuckWRITEFLAG:
      canvas_makefilename(x->x_canvas, filename->s_name,
                          buf, MAXPDSTRING);
      chuck_dosave(x,buf);
      break;
    case ChuckREADFLAG:
    default:
      chuck_doread(x,filename->s_name);
    }
  free(buf);
}

static void chuck_doread(t_chuck *x, char* filename)
{
  DEBUG(post("doread %s",filename););
  FILE *fp = fopen ( filename , "rb" );

  long lSize = 0;
  char *buffer = malloc(MAXSCRIPTSIZE);
  if( fp == NULL )
    {
      // JWM: if user opens an empty editor and then closes it empty,
      // it would trigger an error message, so this surpresses it
      if (x->editor_flag==UNEDITED_SCRIPT)
        error("chuck~: error reading \"%s\"",filename);
      goto out;
    }

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  if (lSize>MAXSCRIPTSIZE)
    {
      error("chuck~: error: file is longer than MAXSCRIPTSIZE");
      fclose(fp);
      goto out;
    }

  post("rtcmix~: read \"%s\"", filename);

  if( fread( buffer , lSize, 1, fp) != 1 )
    {
      // error if file is empty; this is not necessary an error
      // if you close the editor with an empty file
      if (x->editor_flag==UNEDITED_SCRIPT)
        error("chuck~: failed to read file");
      fclose(fp);
      goto out;
    }
  if (lSize>0)
    sprintf(x->chuck_script[x->current_script], "%s",buffer);
  fclose(fp);
  // if it's not a temp file already, save to temp.sco
  if (x->editor_flag==UNEDITED_SCRIPT)
    chuck_dosave(x, x->tempfile_path);

  out:
  x->script_size[x->current_script] = lSize;
  x->editor_flag = UNEDITED_SCRIPT;
  free(buffer);
}

static void chuck_dosave(t_chuck *x, char* filename)
{
  DEBUG(post("dosave %s",filename););
  FILE *pfile;
  pfile = fopen(filename,"w");
  if(pfile == NULL)
    {
      error("chuck~: error opening \"%s\" for writing",filename);
      return;
    }

  unsigned int i;
  for (i=0; i<sizeof(*x->chuck_script[x->current_script]); i++)
    {
      fwrite(x->chuck_script[x->current_script],
             1,
             strlen(x->chuck_script[x->current_script]),
             pfile);
    }
  fclose(pfile);
  if (x->editor_flag == EDITED_SCRIPT)
    post("chuck~: wrote script %i to %s",x->current_script,filename);
}

static void chuck_inletp0(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 0",f););
  chuck_float_inlet(x,0,f);
}

static void chuck_inletp1(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 1",f););
  chuck_float_inlet(x,1,f);
}
static void chuck_inletp2(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 2",f););
  chuck_float_inlet(x,2,f);
}
static void chuck_inletp3(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 3",f););
  chuck_float_inlet(x,3,f);
}
static void chuck_inletp4(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 4",f););
  chuck_float_inlet(x,4,f);
}
static void chuck_inletp5(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 5",f););
  chuck_float_inlet(x,5,f);
}
static void chuck_inletp6(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 6",f););
  chuck_float_inlet(x,6,f);
}
static void chuck_inletp7(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 7",f););
  chuck_float_inlet(x,7,f);
}
static void chuck_inletp8(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 8",f););
  chuck_float_inlet(x,8,f);
}
static void chuck_inletp9(t_chuck *x, t_float f)
{
  DEBUG(post("received %f at pinlet 9",f););
  chuck_float_inlet(x,9,f);
}

static void chuck_float_inlet(t_chuck *x, short inlet, t_float f)
{
  //check to see which input the float came in, then set the appropriate variable value
  if (inlet >= x->num_inputs)
    {
      x->in[inlet] = f;
      post("chuck~: setting in[%d] =  %f, but chuck~ doesn't use this", inlet, f);
    }
  else x->inlet_set(inlet+1, f);
}

static void chuck_dlopen_and_errorcheck(t_chuck *x)
{
  x->chuckdylib = dlopen(x->pathname,  RTLD_NOW | RTLD_LOCAL);
  if (!x->chuckdylib)
    {
      error("dlopen error loading dylib");
    }

  // find the main entry to be sure we're cool...
  x->chuckmain = dlsym(x->chuckdylib, "_chuckmain");
  if (x->chuckmain)	x->chuckmain(x->outbuf_size, x->srate);
  else error("chuck~ could not call chuckmain()");

  x->pull_cb2 = dlsym(x->chuckdylib, "_pull_cb2");
  if (!(x->pull_cb2))
    error("chuck~ could not find pull_cb2()");

  x->parse_score = dlsym(x->chuckdylib, "_parse_score");
  if (!(x->parse_score))
    error("chuck~ could not find parse_score()");

  x->check_bang = dlsym(x->chuckdylib, "_check_bang");
  if (!(x->check_bang))
    error("chuck~ could not find check_bang()");

  x->check_vals = dlsym(x->chuckdylib, "_check_vals");
  if (!(x->check_vals))
    error("chuck~ could not find check_vals()");

  x->inlet_set = dlsym(x->chuckdylib, "_inlet_set");
  if (!(x->inlet_set))
    error("chuck~ could not find inlet_set()");
}
