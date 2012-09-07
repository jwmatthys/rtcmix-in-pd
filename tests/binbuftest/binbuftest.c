/* code for "binbuftest" pd class. This opens a binbuf and allows the user
   to read a file, append to it, and bang to output it. An array of binbufs
   will run internally in rtcmix~ to store scorefiles.
   TODO: implement save routine */

#include "m_pd.h"
#include <stdio.h>
#include <string.h>

#define BINBUFREADFLAG 1
#define BINBUFWRITEFLAG 2
#define CRFLAG 1 // 0 for no CR, 1 for semis?

#define NUMBUFFERS 10

//#define DEBUG(x)
#define DEBUG(x) x

    /* the data structure for each copy of "binbuftest".  In this case we
    on;y need pd's obligatory header (of type t_object). */
typedef struct binbuftest
{
  t_object x_ob;
  t_binbuf *mybuf[NUMBUFFERS];
  t_outlet *textout;
  t_canvas *x_canvas;
  t_symbol *canvas_path;
  t_symbol *x_s;
  unsigned short rw_flag;
  unsigned short current_buffer;
} t_binbuftest;

static void binbuftest_float(t_binbuftest *x, t_floatarg f);
static void binbuftest_bang(t_binbuftest *x);
static void binbuftest_read(t_binbuftest* x, t_symbol *filename);
static void binbuftest_write(t_binbuftest* x, t_symbol *filename);
static void binbuftest_open(t_binbuftest* x,  t_symbol *s, int argc, t_atom *argv);
static void binbuftest_save(t_binbuftest* x,  t_symbol *s, int argc, t_atom *argv);
static void binbuftest_callback(t_binbuftest* x, t_symbol *filename);

    /* this is called back when binbuftest gets a "float" message (i.e., a
    number.) */
static void binbuftest_float(t_binbuftest *x, t_floatarg f)
{
  DEBUG(post("binbuftest: switching to buffer %f", f););
  x->current_buffer = (unsigned int)(f-1);
  // JWM: don't lose this! method to overwrite text to binbuf:
  //char * temp = "Joel is awesome.";
  //binbuf_text(x->mybuf[x->current_buffer],temp,strlen(temp));
}

static void binbuftest_bang(t_binbuftest *x)
{
  DEBUG(post("binbuftest_bang"););
  char* result = malloc(MAXPDSTRING);
  int n = binbuf_getnatom(x->mybuf[x->current_buffer]);
  DEBUG(post("natom: %d",n););
  binbuf_gettext(x->mybuf[x->current_buffer], &result,&n);
  DEBUG(post("result: %s",result););
  // OK, so the text going to the outlet gets truncated at MAXPDSTRING and
  // characters escaped, but I *think* that it remains uncorrupted in the
  // binbuf. I hope!
  outlet_symbol(x->textout,gensym(result));
}

    /* this is called when binbuftest gets the message, "append". */
void binbuftest_append(t_binbuftest *x, t_symbol *s, int argc, t_atom *argv)
{
  DEBUG(post("argc: %d",argc););
  binbuf_add(x->mybuf[x->current_buffer], argc, argv);
  DEBUG(binbuf_print(x->mybuf[x->current_buffer]););
}

static void binbuftest_clear(t_binbuftest *x, t_float f)
{
  DEBUG(post ("binbuf clear"););
  binbuf_clear(x->mybuf[x->current_buffer]);
}

static void binbuftest_callback(t_binbuftest* x, t_symbol *filename)
{
  switch (x->rw_flag)
    {
    case BINBUFREADFLAG:
      binbuftest_read(x,filename);
      break;
    case BINBUFWRITEFLAG:
      binbuftest_write(x,filename);
      break;
    default:
      error("invalid READ/WRITE flag");
    }
}

static void binbuftest_read(t_binbuftest* x, t_symbol *filename)
{
  if (binbuf_read_via_canvas(x->mybuf[x->current_buffer], filename->s_name, x->x_canvas, CRFLAG))
    error("%s: read failed", filename->s_name);
}

static void binbuftest_write(t_binbuftest* x, t_symbol *filename)
{
  char buf[MAXPDSTRING];
  canvas_makefilename(x->x_canvas, filename->s_name,
                      buf, MAXPDSTRING);
  DEBUG(post("filename: %s",buf););
  if (binbuf_write(x->mybuf[x->current_buffer], buf, "", CRFLAG))
    error("%s: write failed", buf);
}

static void binbuftest_open(t_binbuftest* x,  t_symbol *s, int argc, t_atom *argv)
{
  DEBUG(post("binbuftest_read"););
  if (argc==0)
    {
      x->rw_flag = BINBUFREADFLAG;
      sys_vgui("pdtk_openpanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
    }
  else
    {
      t_symbol *filename = atom_getsymbolarg(0,argc,argv);
      binbuftest_read(x,filename);
    }
}

static void binbuftest_save(t_binbuftest* x,  t_symbol *s, int argc, t_atom *argv)
{
  DEBUG(post("binbuftest_save"););
  if (argc==0)
    {
      x->rw_flag = BINBUFWRITEFLAG;
      sys_vgui("pdtk_savepanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
    }
  else
    {
      t_symbol *filename = atom_getsymbolarg(0,argc,argv);
      binbuftest_write(x,filename);
    }
}


    /* this is a pointer to the class for "binbuftest", which is created in the
    "setup" routine below and used to create new ones in the "new" routine. */
t_class *binbuftest_class;

    /* this is called when a new "binbuftest" object is created. */
void *binbuftest_new(void)
{
    t_binbuftest *x = (t_binbuftest *)pd_new(binbuftest_class);

    x->mybuf[x->current_buffer] = binbuf_new();
    int i;
    for (i=0; i<NUMBUFFERS; i++)
      {
        x->mybuf[i] = binbuf_new();
      }
    DEBUG(post("binbuftest_new"););
    // store ref to this object in x->x_s
    char buf[50];
    sprintf(buf, "d%lx", (t_int)x);
    x->x_s = gensym(buf);
    pd_bind(&x->x_ob.ob_pd, x->x_s);
    x->rw_flag = 0;
    x->current_buffer = 0;
    x->x_canvas = canvas_getcurrent();
    x->canvas_path = malloc(MAXPDSTRING);
    x->canvas_path = canvas_getdir(x->x_canvas);
    x->textout = outlet_new(&x->x_ob, &s_symbol);
    return (void *)x;
}

void binbuftest_free(t_binbuftest *x)
{
  binbuf_free(x->mybuf[x->current_buffer]);
  outlet_free(x->textout);
}

    /* this is called once at setup time, when this code is loaded into Pd. */
void binbuftest_setup(void)
{
  DEBUG(post("binbuftest_setup"););
  binbuftest_class = class_new(gensym("binbuftest"), (t_newmethod)binbuftest_new, (t_method)binbuftest_free, sizeof(t_binbuftest), 0, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_append, gensym("append"), A_GIMME, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_open, gensym("read"), A_GIMME, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_save, gensym("save"), A_GIMME, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_clear, gensym("clear"), 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_bang, gensym("click"), 0);
  class_addfloat(binbuftest_class, binbuftest_float);
  class_addbang(binbuftest_class, binbuftest_bang);
  class_addmethod(binbuftest_class, (t_method)binbuftest_callback, gensym("callback"), A_SYMBOL, 0);
}
