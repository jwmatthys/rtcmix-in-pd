/* Copyright (c) 2002-2005 krzYszcz and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "m_pd.h"
#include "loud.h"
#include "file.h"

#define MAXBUFSIZE 32768

typedef struct _capture
{
  t_object       x_ob;
  t_canvas      *x_canvas;
  char           x_intmode;  /* if nonzero ('x' or 'm') floats are ignored */
  float         *x_buffer;
  int            x_bufsize;
  int            x_count;
  int            x_head;
  t_hammerfile  *x_filehandle;
  char          *x_text;
} t_capture;

static t_class *capture_class;

static void capture_doread(t_capture *x, t_symbol *fn)
{
  post("doread");
  //hammerpanel_save(x->x_filehandle, 0, 0);
}

static void capture_dowrite(t_capture *x, t_symbol *fn)
{
  post("dowrite");
  //hammerpanel_save(x->x_filehandle, 0, 0);
}

static void capture_dosave(t_capture *x, t_symbol *fn)
{
  post("dosave");

  t_binbuf *temp = hammereditor_getbinbuf(x->x_filehandle);
  t_atom *xxx = binbuf_getvec(temp);
  char *buf = malloc(MAXBUFSIZE);
  atom_string(xxx,buf,MAXBUFSIZE);
  post("buf: %s",buf);
  //binbuf_write(temp, "tempscript.sco", ".", 0);

  /*

  char* tempbuf;
  int n = binbuf_getnatom(temp);
  binbuf_gettext(temp, &tempbuf, &n);
  binbuf_free(temp);
  post("buffer: %s",tempbuf);
  unsigned int i;
  for (i=0; i<strlen(tempbuf); i++)
    {
      *(x->x_text+i) = *(tempbuf+i);
    }
  while (i<MAXBUFSIZE)
    *(x->x_text+i++) = 0;
    */
}

static void capture_writehook(t_pd *z, t_symbol *fn, int ac, t_atom *av)
{
  post("writehook");
  capture_dowrite((t_capture *)z, fn);
}

static void capture_readhook(t_pd *z, t_symbol *fn, int ac, t_atom *av)
{
  post("readhook");
  capture_doread((t_capture *)z, fn);
}

static void capture_editorhook(t_pd *z, t_symbol *fn, int ac, t_atom *av)
{
  post("editorhook");
  capture_dosave((t_capture *)z, fn);
}

static void capture_open(t_capture *x)
{
  hammereditor_open(x->x_filehandle, "Joel is Awesome", "");
  //hammereditor_append(x->x_filehandle, "textscript\n");
  hammereditor_append(x->x_filehandle, x->x_text);
}

static void capture_click(t_capture *x, t_floatarg xpos, t_floatarg ypos,
                          t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
  capture_open(x);
}

static void capture_bang(t_capture *x)
{
  post ("result: %s",x->x_text);
}

static void capture_free(t_capture *x)
{
  hammerfile_free(x->x_filehandle);
  free(x->x_text);
}

static void *capture_new(t_symbol *s, t_floatarg f)
{
  t_capture *x = (t_capture *)pd_new(capture_class);
  x->x_canvas = canvas_getcurrent();
  x->x_filehandle = hammerfile_new((t_pd *)x, 0, capture_readhook, capture_writehook, capture_editorhook);
  x->x_text = malloc(MAXBUFSIZE);
  return (x);
}

void capture_setup(void)
{
    capture_class = class_new(gensym("capture"),
                              (t_newmethod)capture_new,
                              (t_method)capture_free,
                              sizeof(t_capture), 0, A_DEFFLOAT, A_DEFSYM, 0);
    class_addmethod(capture_class, (t_method)capture_click,
                    gensym("click"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addbang(capture_class, capture_bang);
    hammerfile_setup(capture_class, 0);
}
