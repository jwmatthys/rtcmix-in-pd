/* Copyright (c) 2002-2005 krzYszcz and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include <stdio.h>
#include <string.h>
#include "m_pd.h"
#include "loud.h"
#include "file.h"

#define DEBUG(x) x;
//#define DEBUG(x)

#define CAPTURE_DEFSIZE  512

typedef struct _capturecommon
{
  t_pd             c_pd;
  struct _capture *c_refs;
  t_canvas        *c_lastcanvas;
  t_hammerfile    *c_filehandle;
  int              c_embedflag;  /* common field (CHECKED in 'TEXT' files) */
  t_symbol        *c_filename;   /* CHECKED common for all, read and write */

} t_capturecommon;

typedef struct _capture
{
  t_object       x_ob;
  t_canvas      *x_canvas;
  t_symbol      *x_name;
  t_hammerfile  *x_filehandle;
  t_capturecommon *x_common;
  struct _capture *x_next;
} t_capture;

static t_class *capture_class;
static t_class *capturecommon_class;

static void capture_open(t_capture *x)
{
  DEBUG(post("open"););
  hammereditor_open(x->x_filehandle, "Joel Editor", 0);
  hammereditor_setdirty(x->x_filehandle, 0);
}

/* CHECKED without asking and storing the changes */
static void capture_wclose(t_capture *x)
{
  DEBUG(post("close"););
  hammereditor_close(x->x_filehandle, 1);
}

static void capture_save(t_capture *x)
{
  DEBUG(post("save"););
}

static void capture_click(t_capture *x, t_floatarg xpos, t_floatarg ypos,
                          t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
  capture_open(x);
}

static void capture_dowrite(t_capture *x, t_symbol *fn)
{
  DEBUG(post("dowrite"););

}

static void capture_writehook(t_pd *z, t_symbol *fn, int ac, t_atom *av)
{
    capture_dowrite((t_capture *)z, fn);
}

static void capture_editorhook(t_pd *z, t_symbol *s, int ac, t_atom *av)
{
  DEBUG(post("save stuff"););
  capture_save((t_capture *)z);
}

static void capture_free(t_capture *x)
{
  DEBUG(post("free"););
    hammerfile_free(x->x_filehandle);
}

static void capture_bind(t_capture *x, t_symbol *name)
{
  t_capturecommon *cc = 0;
  if (name == &s_)
    name = 0;
  else if (name)
    cc = (t_capturecommon *)pd_findbyclass(name, capturecommon_class);
  if (!cc)
    {
      t_capturecommon *cc = (t_capturecommon *)pd_new(capturecommon_class);

      if (name)
        {
          pd_bind(&cc->c_pd, name);
          /* LATER rethink canvas unpredictability */
          //capturecommon_doread(cc, name, x->x_canvas);
        }
      else
        {
          cc->c_lastcanvas = 0;
        }
      cc->c_filehandle = hammerfile_new((t_pd *)cc,
                                        0, 0, 0,
                                        capture_editorhook);
    }
  x->x_common = cc;
  x->x_name = name;
  x->x_next = cc->c_refs;
  cc->c_refs = x;
}

static void *capture_new(t_symbol *s)
{
  DEBUG(post("new"););

    t_capture *x = 0;
    x = (t_capture *)pd_new(capture_class);
    x->x_canvas = canvas_getcurrent();
    x->x_filehandle = hammerfile_new((t_pd *)x, 0, 0,
                                     capture_writehook,
                                     capture_editorhook);
    capture_bind(x,s);
    return (x);
}

void capture_setup(void)
{
    capture_class = class_new(gensym("capture"),
                              (t_newmethod)capture_new,
                              (t_method)capture_free,
                              sizeof(t_capture), 0,
                              A_DEFFLOAT, A_DEFSYM, 0);
    class_addmethod(capture_class, (t_method)capture_open,
                    gensym("open"), 0);
    class_addmethod(capture_class, (t_method)capture_wclose,
                    gensym("wclose"), 0);
    class_addmethod(capture_class, (t_method)capture_click,
                    gensym("click"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    hammerfile_setup(capture_class, 0);
}
