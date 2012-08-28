// textbuffer.h
// Joel W. Matthys, 08/27/2012
// rtcmix~ functions for parsing score files
// formerly lines 701-831 of rtcmix~.c, now separated for easier dev

#define VERSION "0.01"
#define RTcmixVERSION "RTcmix-maxmsp-4.0.1.6"

#include "m_pd.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dlfcn.h>

#define MAX_INPUTS 100  //arbitrary -- for Damon!
#define MAX_OUTPUTS 20	//also arbitrary
#define MAX_SCRIPTS 20	//how many scripts can we store internally

/*** PD FUNCTIONS ***/

static t_class *textbuffer_class;

typedef struct textbuffer
{
  t_object x_obj;
  t_inlet *x_inindex;
  t_int flushflag;
} t_textbuffer;

/****PROTOTYPES****/

//int dylibincr;
void rtcmix_text(t_textbuffer *x, t_symbol *s, int argc, t_atom *argv);
void rtcmix_dotext(t_textbuffer *x, t_symbol *s, int argc, t_atom *argv);
void rtcmix_badquotes(char *cmd, char *buf); // this is to check for 'split' quoted params, called in rtcmix_dotext

/*** TEXT BUFFER FUNCTIONS ***/

// see the note for rtcmix_dotext() below
void rtcmix_text(t_textbuffer *x, t_symbol *s, int argc, t_atom *argv)
{
  if (x->flushflag == 1) return; // heap and queue being reset
  // uh oh, no defer_low in Pd. Hope this doesn't cause trouble...
  //defer_low(x, (method)rtcmix_dotext, s, argc, argv); // always defer this message
  rtcmix_dotext(x, s, argc, argv);
}

// what to do when we get the message "text"
// rtcmix~ scores come from the [textedit] object this way
void rtcmix_dotext(t_textbuffer *x, t_symbol *s, int argc, t_atom *argv)
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

  for (i=0; i < argc; i++) {
    switch (argv[i].a_type) {
    case A_FLOAT:
      sprintf(xfer, " %lf", argv[i].a_w.w_float);
      break;
      /*case A_DOLLAR:
      varnum = argv[i].a_w.w_float;
      //if ( !(x->var_set[varnum-1]) ) error("variable $%d has not been set yet, using 0.0 as default",varnum);
      sprintf(xfer, " %lf", x->var_array[varnum-1]);
      break;*/
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

  // ok, here's the deal -- when cycling (and pd) tokenizes the message stream, if a quoted param (like a pathname to a soundfile)
  // contains any spaces it will be split into to separate, unquoted Symbols.  The function below repairs this for
  // certain specific rtcmix commands.  Not really elegant, but I don't know of another solution at present
  // JWM - what if, upon getting quotes, we jump to a separate routine until we get close quotes? Then jump back.
  rtcmix_badquotes("rtinput", thebuf);
  rtcmix_badquotes("system", thebuf);
  rtcmix_badquotes("dataset", thebuf);

  // JWM - this the RTcmix parse command; for now I'll edit it out and just post thebuf
  //if (x->parse_score(thebuf, strlen(thebuf)) != 0) error("problem parsing RTcmix script");
  post((char*)thebuf);
}

// see the note in rtcmix_dotext() about why I have to do this
void rtcmix_badquotes(char *cmd, char *thebuf)
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

void textbuffer_new(void)
{
  t_textbuffer *x = (t_textbuffer *)pd_new(textbuffer_class);
  x->flushflag = 0;
  //return (void *)x;
}

void textbuffer_setup(void)
{
  textbuffer_class = class_new(gensym("textbuffer"),
                               (t_newmethod)textbuffer_new,
                               0,sizeof(t_textbuffer),
                               CLASS_DEFAULT, 0);

  class_addlist(textbuffer_class, rtcmix_text);
}
