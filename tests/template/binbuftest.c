/* code for "binbuftest" pd class. This opens a binbuf and allows the user
   to read a file, append to it, and bang to output it. An array of binbufs
   will run internally in rtcmix~ to store scorefiles.
   TODO: implement save routine */

#include "m_pd.h"
#include <stdio.h>
#include <string.h>

#define DEBUG(x)
//#define DEBUG(x) x

    /* the data structure for each copy of "binbuftest".  In this case we
    on;y need pd's obligatory header (of type t_object). */
typedef struct binbuftest
{
  t_object x_ob;
  t_binbuf *mybuf;
  t_outlet *textout;
  t_canvas *x_canvas;
  t_symbol *canvas_path;
  t_symbol *x_s;
} t_binbuftest;

static void binbuftest_callback(t_binbuftest* x,  t_symbol *filename);

    /* this is called back when binbuftest gets a "float" message (i.e., a
    number.) */
void binbuftest_float(t_binbuftest *x, t_floatarg f)
{
  DEBUG(post("binbuftest: %f", f););
  // JWM: don't lose this! method to overwrite text to binbuf:
  //char * temp = "Joel is awesome.";
  //binbuf_text(x->mybuf,temp,strlen(temp));
}

void binbuftest_bang(t_binbuftest *x)
{
  DEBUG(post("binbuftest_bang"););
  char* result = malloc(MAXPDSTRING);
  int n = binbuf_getnatom(x->mybuf);
  DEBUG(post("natom: %d",n););
  binbuf_gettext(x->mybuf, &result,&n);
  DEBUG(post("result: %s",result););
}

    /* this is called when binbuftest gets the message, "append". */
void binbuftest_append(t_binbuftest *x, t_symbol *s, int argc, t_atom *argv)
{
  DEBUG(post("argc: %d",argc););
  binbuf_add(x->mybuf, argc, argv);
  DEBUG(binbuf_print(x->mybuf););
}

void binbuftest_clear(t_binbuftest *x, t_float f)
{
  DEBUG(post ("binbuf clear"););
  binbuf_clear(x->mybuf);
}

void binbuftest_read(t_binbuftest* x,  t_symbol *s, int argc, t_atom *argv)
{
  DEBUG(post("binbuftest_read"););
  if (argc==0)
    {
      sys_vgui("pdtk_openpanel {%s} {%s}\n", x->x_s->s_name, x->canvas_path->s_name);
    }
  else
    {
      char * filename = atom_getsymbolarg(0,argc,argv)->s_name;
      if (binbuf_read_via_canvas(x->mybuf, filename, x->x_canvas, 0))
        error("%s: read failed", filename);
    }
}

static void binbuftest_callback(t_binbuftest* x,  t_symbol *filename)
{
  if (binbuf_read_via_canvas(x->mybuf, filename->s_name, x->x_canvas, 0))
    error("%s: read failed", filename->s_name);
}


void binbuftest_save(t_binbuftest* x,  t_symbol *filename)
{
  DEBUG(post("binbuftest_save"););
  char buf[MAXPDSTRING];
  canvas_makefilename(x->x_canvas, filename->s_name,
                      buf, MAXPDSTRING);
  if (binbuf_write(x->mybuf, buf, "", 0))
    error("%s: save failed", filename->s_name);
}

    /* this is a pointer to the class for "binbuftest", which is created in the
    "setup" routine below and used to create new ones in the "new" routine. */
t_class *binbuftest_class;

    /* this is called when a new "binbuftest" object is created. */
void *binbuftest_new(void)
{
    t_binbuftest *x = (t_binbuftest *)pd_new(binbuftest_class);
    x->mybuf = binbuf_new();
    DEBUG(post("binbuftest_new"););
    // store ref to this object in x->x_s
    char buf[50];
    sprintf(buf, "d%lx", (t_int)x);
    x->x_s = gensym(buf);
    pd_bind(&x->x_ob.ob_pd, x->x_s);

    x->x_canvas = canvas_getcurrent();
    x->canvas_path = malloc(MAXPDSTRING);
    x->canvas_path = canvas_getdir(x->x_canvas);
    x->textout = outlet_new(&x->x_ob, &s_symbol);
    return (void *)x;
}

void binbuftest_free(t_binbuftest *x)
{
  binbuf_free(x->mybuf);
  outlet_free(x->textout);
}

    /* this is called once at setup time, when this code is loaded into Pd. */
void binbuftest_setup(void)
{
  DEBUG(post("binbuftest_setup"););
  binbuftest_class = class_new(gensym("binbuftest"), (t_newmethod)binbuftest_new, (t_method)binbuftest_free, sizeof(t_binbuftest), 0, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_append, gensym("append"), A_GIMME, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_read, gensym("read"), A_GIMME, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_save, gensym("save"), A_SYMBOL, 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_clear, gensym("clear"), 0);
  class_addmethod(binbuftest_class, (t_method)binbuftest_bang, gensym("click"), 0);
  class_addfloat(binbuftest_class, binbuftest_float);
  class_addbang(binbuftest_class, binbuftest_bang);
  class_addmethod(binbuftest_class, (t_method)binbuftest_callback, gensym("callback"), A_SYMBOL, 0);
}
