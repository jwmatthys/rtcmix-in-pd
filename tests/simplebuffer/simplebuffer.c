/* code for "simplebuffer" pd class.  This takes two messages: floating-point
   numbers, and "rats", and just prints something out for each message. */

#include "m_pd.h"
#include "g_canvas.h"
#include <stdio.h>
#include <string.h>
#include "loud.h"
#include "file.h"

#define loudbug_bug post

/* the data structure for each copy of "simplebuffer".  In this case we
   on;y need pd's obligatory header (of type t_object). */

enum { SB_HEADRESET,
       SB_HEADDELETED };

/*typedef struct _collelem
{
    int                e_hasnumkey;
    int                e_numkey;
    t_symbol          *e_symkey;
    struct _collelem  *e_prev;
    struct _collelem  *e_next;
    int                e_size;
    t_atom            *e_data;
    } t_collelem;
*/

typedef struct _sharedtextbuffer
{
    t_pd           c_pd;
    struct simplebuffer *c_refs;  /* used in read-banging and dirty flag handling */
    int            c_increation;
    int            c_volatile;
    int            c_selfmodified;
    int            c_entered;    /* a counter, LATER rethink */
    int            c_embedflag;  /* common field (CHECKED in 'TEXT' files) */
    t_symbol      *c_filename;   /* CHECKED common for all, read and write */
    t_canvas      *c_lastcanvas;
    t_hammerfile  *c_filehandle;
  //t_collelem    *c_first;
  //t_collelem    *c_last;
  //t_collelem    *c_head;
    int            c_headstate;
} t_sharedtextbuffer;

typedef struct simplebuffer
{
  t_object x_ob;
  t_canvas      *x_canvas;
  t_symbol      *x_name;
  t_sharedtextbuffer  *x_common;
  t_hammerfile  *x_filehandle;
  t_outlet      *x_keyout;
  t_outlet      *x_filebangout;
  t_outlet      *x_dumpbangout;
  struct simplebuffer  *x_next;
} t_simplebuffer;

static void simplebuffer_open(t_simplebuffer *x);
static void simplebuffer_dump(t_simplebuffer *x);
static void simplebuffer_bang(t_simplebuffer *x);
static void simplebuffer_dooutput(t_simplebuffer *x, int ac, t_atom *av);
//static void simplebuffer_keyoutput(t_simplebuffer *x, t_collelem *ep);
static void simplebuffer_read(t_simplebuffer *x, t_symbol *s);
static void simplebuffer_readagain(t_simplebuffer *x);
static void simplebuffer_write(t_simplebuffer *x, t_symbol *s);
static void simplebuffer_writeagain(t_simplebuffer *x);
static int sharedtextbuffer_frombinbuf(t_sharedtextbuffer *cc, t_binbuf *bb);
static void sharedtextbuffer_tobinbuf(t_sharedtextbuffer *cc, t_binbuf *bb);
static void simplebuffer_embedhook(t_pd *z, t_binbuf *bb, t_symbol *bindsym);
static void simplebuffer_unbind(t_simplebuffer *x);
static void simplebuffer_bind(t_simplebuffer *x, t_symbol *name);
static void sharedtextbuffer_free(t_sharedtextbuffer *cc);
static void *sharedtextbuffer_new(void);
static void sharedtextbuffer_readhook(t_pd *z, t_symbol *fn, int ac, t_atom *av);
static void sharedtextbuffer_writehook(t_pd *z, t_symbol *fn, int ac, t_atom *av);
static void sharedtextbuffer_editorhook(t_pd *z, t_symbol *s, int ac, t_atom *av);
static void sharedtextbuffer_dowrite(t_sharedtextbuffer *cc, t_symbol *fn, t_canvas *cv);
static void sharedtextbuffer_doread(t_sharedtextbuffer *cc, t_symbol *fn, t_canvas *cv);
static int sharedtextbuffer_fromatoms(t_sharedtextbuffer *cc, int ac, t_atom *av);
/*static void sharedtextbuffer_putafter(t_sharedtextbuffer *cc,
  t_collelem *ep, t_collelem *prev);*/
static void sharedtextbuffer_modified(t_sharedtextbuffer *cc, int relinked);
static void sharedtextbuffer_clearall(t_sharedtextbuffer *cc);
//static t_collelem *collelem_new(int ac, t_atom *av, int *np, t_symbol *s);
//static void collelem_free(t_collelem *ep);

/* this is called back when simplebuffer gets a "float" message (i.e., a
   number.) */
void simplebuffer_float(t_simplebuffer *x, t_floatarg f)
{
  post("simplebuffer: %f", f);
  x=NULL; /* don't warn about unused variables */
}

static void simplebuffer_bang(t_simplebuffer *x)
{
  simplebuffer_dump(x);
}

static void simplebuffer_write(t_simplebuffer *x, t_symbol *s)
{
    t_sharedtextbuffer *cc = x->x_common;
    if (s && s != &s_)
        sharedtextbuffer_dowrite(cc, s, x->x_canvas);
    else
        hammerpanel_save(cc->c_filehandle, 0, 0);  /* CHECKED no default name */
}

static void simplebuffer_writeagain(t_simplebuffer *x)
{
    t_sharedtextbuffer *cc = x->x_common;
    if (cc->c_filename)
        sharedtextbuffer_dowrite(cc, 0, 0);
    else
        hammerpanel_save(cc->c_filehandle, 0, 0);  /* CHECKED no default name */
}

static void simplebuffer_click(t_simplebuffer *x, t_floatarg xpos, t_floatarg ypos,
                               t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
  post("simplebuffer: clicked");
  simplebuffer_open(x);
}

static void simplebuffer_dump(t_simplebuffer *x)
{
  t_sharedtextbuffer *cc = x->x_common;
  /*
     t_collelem *ep;
     for (ep = cc->c_first; ep; ep = ep->e_next)
     {
     simplebuffer_keyoutput(x, ep);
     if (cc->c_selfmodified)
     break;
     simplebuffer_dooutput(x, ep->e_size, ep->e_data);
     // FIXME dooutput() may invalidate ep as well as keyoutput()...
     }
  */
  outlet_bang(x->x_dumpbangout);
}

static void simplebuffer_dooutput(t_simplebuffer *x, int ac, t_atom *av)
{
    if (ac > 1)
    {
        if (av->a_type == A_FLOAT)
            outlet_list(((t_object *)x)->ob_outlet, &s_list, ac, av);
        else if (av->a_type == A_SYMBOL)
            outlet_anything(((t_object *)x)->ob_outlet,
                            av->a_w.w_symbol, ac-1, av+1);
    }
    else if (ac)
    {
        if (av->a_type == A_FLOAT)
            outlet_float(((t_object *)x)->ob_outlet, av->a_w.w_float);
        else if (av->a_type == A_SYMBOL)
            outlet_symbol(((t_object *)x)->ob_outlet, av->a_w.w_symbol);
    }
}

/*
static void simplebuffer_keyoutput(t_simplebuffer *x, t_collelem *ep)
{
    t_sharedtextbuffer *cc = x->x_common;
    if (!cc->c_entered++) cc->c_selfmodified = 0;
    cc->c_volatile = 0;
    if (ep->e_hasnumkey)
        outlet_float(x->x_keyout, ep->e_numkey);
    else if (ep->e_symkey)
        outlet_symbol(x->x_keyout, ep->e_symkey);
    else
        outlet_float(x->x_keyout, 0);
    if (cc->c_volatile) cc->c_selfmodified = 1;
    cc->c_entered--;
}
*/

static void simplebuffer_open(t_simplebuffer *x)
{
  t_sharedtextbuffer *cc = x->x_common;
  t_binbuf *bb = binbuf_new();
  int natoms, newline;
  t_atom *ap;
  char buf[MAXPDSTRING];
  hammereditor_open(cc->c_filehandle,
                    (x->x_name ? x->x_name->s_name : "RTCMIX script"), "simplebuffer");
  sharedtextbuffer_tobinbuf(cc, bb);
  natoms = binbuf_getnatom(bb);
  ap = binbuf_getvec(bb);
  newline = 1;
  while (natoms--)
    {
      char *ptr = buf;
      if (ap->a_type != A_SEMI && ap->a_type != A_COMMA && !newline)
        *ptr++ = ' ';
      atom_string(ap, ptr, MAXPDSTRING);
      if (ap->a_type == A_SEMI)
        {
          strcat(buf, "\n");
          newline = 1;
        }
      else newline = 0;
      hammereditor_append(cc->c_filehandle, buf);
      ap++;
    }
  hammereditor_setdirty(cc->c_filehandle, 0);
  binbuf_free(bb);
}

static void simplebuffer_read(t_simplebuffer *x, t_symbol *s)
{
    t_sharedtextbuffer *cc = x->x_common;
    if (s && s != &s_)
        sharedtextbuffer_doread(cc, s, x->x_canvas);
    else
        hammerpanel_open(cc->c_filehandle, 0);
}

static void simplebuffer_readagain(t_simplebuffer *x)
{
    t_sharedtextbuffer *cc = x->x_common;
    if (cc->c_filename)
        sharedtextbuffer_doread(cc, 0, 0);
    else
        hammerpanel_open(cc->c_filehandle, 0);
}

/*
static t_collelem *collelem_new(int ac, t_atom *av, int *np, t_symbol *s)
{
    t_collelem *ep = (t_collelem *)getbytes(sizeof(*ep));
    if (ep->e_hasnumkey = (np != 0))
        ep->e_numkey = *np;
    ep->e_symkey = s;
    ep->e_prev = ep->e_next = 0;
    if (ep->e_size = ac)
    {
        t_atom *ap = getbytes(ac * sizeof(*ap));
        ep->e_data = ap;
        if (av) while (ac--)
            *ap++ = *av++;
        else while (ac--)
        {
            SETFLOAT(ap, 0);
            ap++;
        }
    }
    else ep->e_data = 0;
    return (ep);
}

static void collelem_free(t_collelem *ep)
{
    if (ep->e_data)
        freebytes(ep->e_data, ep->e_size * sizeof(*ep->e_data));
    freebytes(ep, sizeof(*ep));
}
*/

static void sharedtextbuffer_modified(t_sharedtextbuffer *cc, int relinked)
{
    if (cc->c_increation)
        return;
    if (relinked)
    {
        cc->c_volatile = 1;
    }
    if (cc->c_embedflag)
    {
        t_simplebuffer *x;
        for (x = cc->c_refs; x; x = x->x_next)
            if (x->x_canvas && glist_isvisible(x->x_canvas))
                canvas_dirty(x->x_canvas, 1);
    }
}

static void sharedtextbuffer_clearall(t_sharedtextbuffer *cc)
{
    if (cc->c_first)
    {
      /*
        t_collelem *ep1 = cc->c_first, *ep2;
        do
        {
            ep2 = ep1->e_next;
            collelem_free(ep1);
        }
        while (ep1 = ep2);
      */
        cc->c_first = cc->c_last = 0;
        cc->c_head = 0;
        cc->c_headstate = SB_HEADRESET;
        sharedtextbuffer_modified(cc, 1);
    }
}

static int sharedtextbuffer_fromatoms(t_sharedtextbuffer *cc, int ac, t_atom *av)
{
    int hasnumkey = 0, numkey;
    t_symbol *symkey = 0;
    int size = 0;
    t_atom *data = 0;
    int nlines = 0;
    cc->c_increation = 1;
    sharedtextbuffer_clearall(cc);
    while (ac--)
    {
        if (data)
        {
            if (av->a_type == A_SEMI)
            {
              /*
                t_collelem *ep = collelem_new(size, data,
                                              hasnumkey ? &numkey : 0, symkey);
                sharedtextbuffer_putafter(cc, ep, cc->c_last);
              */
                hasnumkey = 0;
                symkey = 0;
                data = 0;
                nlines++;
            }
            if (av->a_type == A_COMMA)
            {
                /* CHECKED rejecting a comma */
                sharedtextbuffer_clearall(cc);  /* LATER rethink */
                cc->c_increation = 0;
                return (-nlines);
            }
            else size++;
        }
        else if (av->a_type == A_COMMA)
        {
            size = 0;
            data = av + 1;
        }
        else if (av->a_type == A_SYMBOL)
            symkey = av->a_w.w_symbol;
        else if (av->a_type == A_FLOAT &&
                 loud_checkint(0, av->a_w.w_float, &numkey, 0))
            hasnumkey = 1;
        else
        {
            loud_error(0, "coll: bad atom");
            sharedtextbuffer_clearall(cc);  /* LATER rethink */
            cc->c_increation = 0;
            return (-nlines);
        }
        av++;
    }
    if (data)
    {
        loud_error(0, "coll: incomplete");
        sharedtextbuffer_clearall(cc);  /* LATER rethink */
        cc->c_increation = 0;
        return (-nlines);
    }
    cc->c_increation = 0;
    return (nlines);
}

/*
static void sharedtextbuffer_putafter(t_sharedtextbuffer *cc,
                                t_collelem *ep, t_collelem *prev)
{
    if (prev)
    {
        ep->e_prev = prev;
        if (ep->e_next = prev->e_next)
            ep->e_next->e_prev = ep;
        else
            cc->c_last = ep;
        prev->e_next = ep;
    }
    else if (cc->c_first || cc->c_last)
        loudbug_bug("sharedtextbuffer_putafter");
    else
        cc->c_first = cc->c_last = ep;
    sharedtextbuffer_modified(cc, 1);
}
*/

static int sharedtextbuffer_frombinbuf(t_sharedtextbuffer *cc, t_binbuf *bb)
{
    return (sharedtextbuffer_fromatoms(cc, binbuf_getnatom(bb), binbuf_getvec(bb)));
}

static void sharedtextbuffer_tobinbuf(t_sharedtextbuffer *cc, t_binbuf *bb)
{
  /*
    t_collelem *ep;
    t_atom at[3];
    for (ep = cc->c_first; ep; ep = ep->e_next)
    {
        t_atom *ap = at;
        int cnt = 1;
        if (ep->e_hasnumkey)
        {
            SETFLOAT(ap, ep->e_numkey);
            ap++; cnt++;
        }
        if (ep->e_symkey)
        {
            SETSYMBOL(ap, ep->e_symkey);
            ap++; cnt++;
        }
        SETCOMMA(ap);
        binbuf_add(bb, cnt, at);
        binbuf_add(bb, ep->e_size, ep->e_data);
        binbuf_addsemi(bb);
    }
  */
          binbuf_add(bb, cc);
}

/* this is a pointer to the class for "simplebuffer", which is created in the
   "setup" routine below and used to create new ones in the "new" routine. */
static t_class *simplebuffer_class;
static t_class *sharedtextbuffer_class;

/* this is called when a new "simplebuffer" object is created. */
static void *simplebuffer_new(t_symbol *s)
{
  t_simplebuffer *x = (t_simplebuffer *)pd_new(simplebuffer_class);
  x->x_canvas = canvas_getcurrent();
  outlet_new((t_object *)x, &s_);
  x->x_keyout = outlet_new((t_object *)x, &s_);
  x->x_filebangout = outlet_new((t_object *)x, &s_bang);
  x->x_dumpbangout = outlet_new((t_object *)x, &s_bang);
  x->x_filehandle = hammerfile_new((t_pd *)x, simplebuffer_embedhook, 0, 0, 0);
  simplebuffer_bind(x, s);
  post("simplebuffer_new");
  return (x);
}

static void sharedtextbuffer_dowrite(t_sharedtextbuffer *cc, t_symbol *fn, t_canvas *cv)
{
    t_binbuf *bb;
    int ac;
    t_atom *av;
    char buf[MAXPDSTRING];
    if (!fn && !(fn = cc->c_filename))  /* !fn: 'writeagain' */
        return;
    if (cv || (cv = cc->c_lastcanvas))  /* !cv: 'write' w/o arg, 'writeagain' */
        canvas_makefilename(cv, fn->s_name, buf, MAXPDSTRING);
    else
    {
        strncpy(buf, fn->s_name, MAXPDSTRING);
        buf[MAXPDSTRING-1] = 0;
    }
    bb = binbuf_new();
    sharedtextbuffer_tobinbuf(cc, bb);
    if (binbuf_write(bb, buf, "", 0))
        loud_error(0, "coll: error writing text file '%s'", fn->s_name);
    else
    {
        cc->c_lastcanvas = cv;
        cc->c_filename = fn;
    }
    binbuf_free(bb);
}

static void sharedtextbuffer_doread(t_sharedtextbuffer *cc, t_symbol *fn, t_canvas *cv)
{
    t_binbuf *bb;
    char buf[MAXPDSTRING];
    if (!fn && !(fn = cc->c_filename))  /* !fn: 'readagain' */
        return;
    /* FIXME use open_via_path() */
    if (cv || (cv = cc->c_lastcanvas))  /* !cv: 'read' w/o arg, 'readagain' */
        canvas_makefilename(cv, fn->s_name, buf, MAXPDSTRING);
    else
    {
        strncpy(buf, fn->s_name, MAXPDSTRING);
        buf[MAXPDSTRING-1] = 0;
    }
    if (!cc->c_refs)
    {
        /* loading during object creation --
           avoid binbuf_read()'s complaints, LATER rethink */
        FILE *fp;
        char fname[MAXPDSTRING];
        sys_bashfilename(buf, fname);
        if (!(fp = fopen(fname, "r")))
        {
            loud_warning(&simplebuffer_class, 0, "no coll file '%s'", fname);
            return;
        }
        fclose(fp);
    }
    bb = binbuf_new();
    if (binbuf_read(bb, buf, "", 0))
        loud_error(0, "coll: error reading text file '%s'", fn->s_name);
    else
    {
        int nlines = sharedtextbuffer_frombinbuf(cc, bb);
        if (nlines > 0)
        {
            t_simplebuffer *x;
            /* LATER consider making this more robust */
            for (x = cc->c_refs; x; x = x->x_next)
              outlet_bang(x->x_filebangout);
            cc->c_lastcanvas = cv;
            cc->c_filename = fn;
            post("coll: finished reading %d lines from text file '%s'",
                 nlines, fn->s_name);
        }
        else if (nlines < 0)
            loud_error(0, "coll: error in line %d of text file '%s'",
                       1 - nlines, fn->s_name);
        else
            loud_error(0, "coll: error reading text file '%s'", fn->s_name);
        if (cc->c_refs)
            sharedtextbuffer_modified(cc, 1);
    }
    binbuf_free(bb);
}

static void sharedtextbuffer_editorhook(t_pd *z, t_symbol *s, int ac, t_atom *av)
{
    int nlines = sharedtextbuffer_fromatoms((t_sharedtextbuffer *)z, ac, av);
    if (nlines < 0)
        loud_error(0, "coll: editing error in line %d", 1 - nlines);
}

static void sharedtextbuffer_readhook(t_pd *z, t_symbol *fn, int ac, t_atom *av)
{
    sharedtextbuffer_doread((t_sharedtextbuffer *)z, fn, 0);
}

static void sharedtextbuffer_writehook(t_pd *z, t_symbol *fn, int ac, t_atom *av)
{
    sharedtextbuffer_dowrite((t_sharedtextbuffer *)z, fn, 0);
}

static void simplebuffer_embedhook(t_pd *z, t_binbuf *bb, t_symbol *bindsym)
{
  t_simplebuffer *x = (t_simplebuffer *)z;
  t_sharedtextbuffer *cc = x->x_common;
  if (cc->c_embedflag)
    {
      t_collelem *ep;
      t_atom at[6];
      binbuf_addv(bb, "ssii;", bindsym, gensym("flags"), 1, 0);
      SETSYMBOL(at, bindsym);
      for (ep = cc->c_first; ep; ep = ep->e_next)
        {
          t_atom *ap = at + 1;
          int cnt;
          if (ep->e_hasnumkey && ep->e_symkey)
            {
              SETSYMBOL(ap, gensym("nstore"));
              ap++;
              SETSYMBOL(ap, ep->e_symkey);
              ap++;
              SETFLOAT(ap, ep->e_numkey);
              cnt = 4;
            }
          else if (ep->e_symkey)
            {
              SETSYMBOL(ap, gensym("store"));
              ap++;
              SETSYMBOL(ap, ep->e_symkey);
              cnt = 3;
            }
          else
            {
              SETFLOAT(ap, ep->e_numkey);
              cnt = 2;
            }
          binbuf_add(bb, cnt, at);
          binbuf_add(bb, ep->e_size, ep->e_data);
          binbuf_addsemi(bb);
        }
    }
}

static void simplebuffer_bind(t_simplebuffer *x, t_symbol *name)
{
  t_sharedtextbuffer *cc = 0;
  if (name == &s_)
    name = 0;
  else if (name)
    cc = (t_sharedtextbuffer *)pd_findbyclass(name, sharedtextbuffer_class);
  if (!cc)
    {
      cc = (t_sharedtextbuffer *)sharedtextbuffer_new();
      cc->c_refs = 0;
      cc->c_increation = 0;
      if (name)
        {
          pd_bind(&cc->c_pd, name);
          /* LATER rethink canvas unpredictability */
          sharedtextbuffer_doread(cc, name, x->x_canvas);
        }
      else
        {
          cc->c_filename = 0;
          cc->c_lastcanvas = 0;
        }
      cc->c_filehandle = hammerfile_new((t_pd *)cc, 0, sharedtextbuffer_readhook,
                                        sharedtextbuffer_writehook,
                                        sharedtextbuffer_editorhook);
    }
  x->x_common = cc;
  x->x_name = name;
  x->x_next = cc->c_refs;
  cc->c_refs = x;
}

void simplebuffer_free(t_simplebuffer *x)
{
  hammerfile_free(x->x_filehandle);
  simplebuffer_unbind(x);
}

static void simplebuffer_unbind(t_simplebuffer *x)
{
    t_sharedtextbuffer *cc = x->x_common;
    t_simplebuffer *prev, *next;
    if ((prev = cc->c_refs) == x)
      {
        if (!(cc->c_refs = x->x_next))
          {
            hammerfile_free(cc->c_filehandle);
            sharedtextbuffer_free(cc);
            if (x->x_name) pd_unbind(&cc->c_pd, x->x_name);
            pd_free(&cc->c_pd);
          }
      }
    else if (prev)
      {
        while (next = prev->x_next)
        {
            if (next == x)
            {
                prev->x_next = next->x_next;
                break;
            }
            prev = next;
        }
    }
    x->x_common = 0;
    x->x_name = 0;
    x->x_next = 0;
}

static void *sharedtextbuffer_new(void)
{
    t_sharedtextbuffer *cc = (t_sharedtextbuffer *)pd_new(sharedtextbuffer_class);
    cc->c_embedflag = 0;
    cc->c_first = cc->c_last = 0;
    cc->c_head = 0;
    cc->c_headstate = SB_HEADRESET;
    return (cc);
}

static void sharedtextbuffer_free(t_sharedtextbuffer *cc)
{
  t_collelem *ep1, *ep2 = cc->c_first;
    while (ep1 = ep2)
      {
      ep2 = ep1->e_next;
        collelem_free(ep1);
    }
}


/* this is called once at setup time, when this code is loaded into Pd. */
void simplebuffer_setup(void)
{
  post("simplebuffer_setup");
  simplebuffer_class = class_new(gensym("simplebuffer"), (t_newmethod)simplebuffer_new, 0,
                                 sizeof(t_simplebuffer), 0, 0);
  class_addfloat(simplebuffer_class, simplebuffer_float);
  class_addbang(simplebuffer_class, simplebuffer_bang);
  class_addmethod(simplebuffer_class, (t_method)simplebuffer_click,
                  gensym("click"),
                  A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(simplebuffer_class, (t_method)simplebuffer_write,
                  gensym("write"), A_DEFSYM, 0);
  class_addmethod(simplebuffer_class, (t_method)simplebuffer_writeagain,
                  gensym("writeagain"), 0);
  sharedtextbuffer_class = class_new(gensym("simplebuffer"), 0, 0,
                               sizeof(t_sharedtextbuffer), CLASS_PD, 0);
  class_addmethod(simplebuffer_class, (t_method)simplebuffer_read,
                  gensym("read"), A_DEFSYM, 0);
  class_addmethod(simplebuffer_class, (t_method)simplebuffer_readagain,
                  gensym("readagain"), 0);
    /* this call is a nop (sharedtextbuffer does not embed, and the hammerfile
       class itself has been already set up above), but it is better to
       have it around, just in case... */
    hammerfile_setup(sharedtextbuffer_class, 0);
}
