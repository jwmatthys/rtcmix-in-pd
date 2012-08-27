// textbuffer.h
// Joel W. Matthys, 08/27/2012
// rtcmix~ functions for parsing score files
// formerly lines 701-831 of rtcmix~.c, now separated for easier dev

#define VERSION "0.01"
#define RTcmixVERSION "RTcmix-maxmsp-4.0.1.6"

// we'll see how many of these we need
#include "ext.h"
#include "z_dsp.h"
#include "string.h"
#include "ext_strings.h"
#include "edit.h"
#include "ext_wind.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "buffer.h"
#include "ext_obex.h"
#include <dlfcn.h>
int dylibincr;

#define MAX_INPUTS 100  //arbitrary -- for Damon!
#define MAX_OUTPUTS 20	//also arbitrary
#define MAX_SCRIPTS 20	//how many scripts can we store internally

// JWM - might not need these...
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

/****PROTOTYPES****/

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
