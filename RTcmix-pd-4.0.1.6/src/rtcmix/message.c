/* message functions, designed for use by instruments and others.
                                                        -JGG, 5/16/00
*/
/* modified to allow die() to continue (i.e. no exit) if ERROR_ON_EXIT
   is undef'd in makefile.conf
   -- BGG 1/2004
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <globals.h>
#include <rtdefs.h>
#include <prototypes.h>
#include <ugens.h>
#include <Option.h>
#include <m_pd.h> // print directly to Pd console

#define PREFIX  "***"       /* print before WARNING and ERROR */
#define BUFSIZE 1024

#ifdef SGI  /* not as safe, but what can ya do */
#define vsnprintf(str, sz, fmt, args)  vsprintf(str, fmt, args)
#endif

/* These functions are wrappers for printf that do a little extra work.
   The first arg is the name of the instrument they're called from.
   Non-instrument code can call these and pass NULL for <inst_name>.
   The remaining args are just like printf (format string and a variable
   number of arguments to fill it).  advise() and warn() take into account
   the current state of the print option, while die() prints no matter what,
   and then exits after some cleanup.
*/

/* --------------------------------------------------------------- advise --- */
// BGG mm -- changed "advise" to "rtcmix_advise"; rtcmix~ conflict in OSX 10.5
void
rtcmix_advise(const char *inst_name, const char *format, ...)
{
  if (get_print_option())
    {
      char     buf[BUFSIZE];
      va_list  args;
      
      va_start(args, format);
      vsnprintf(buf, BUFSIZE, format, args);
      va_end(args);
      
      // use Pd's built-in post() to print to console
      if (inst_name)
	post("%s:  %s", inst_name, buf);
      else
	post("%s", buf);
      //fprintf(stdout,"Advise: %s",buf);
    }
}


/* --------------------------------------------------------------- warn --- */
void
warn(const char *inst_name, const char *format, ...)
{
  if (get_print_option())
    {
      char     buf[BUFSIZE];
      va_list  args;
      
      va_start(args, format);
      vsnprintf(buf, BUFSIZE, format, args);
      va_end(args);
      
      // JWM: use Pd's post() command to print to console
      if (inst_name)
	post("%s %s %s", PREFIX, inst_name, buf);
      else
	post("%s  %s", PREFIX, buf);
      //fprintf(stdout,"Warn: %s",buf);
    }
}

/* -------------------------------------------------------------- rterror --- */
void
rterror(const char *inst_name, const char *format, ...)
{
  char     buf[BUFSIZE];
  va_list  args;
  
  va_start(args, format);
  vsnprintf(buf, BUFSIZE, format, args);
  va_end(args);
  
  if (inst_name)
    error("%s ERROR [%s]: %s", PREFIX, inst_name, buf);
  else
    error("%s ERROR: %s", PREFIX, buf);
  //fprintf(stdout,"Error: %s",buf);
}

/* ------------------------------------------------------------------ die --- */
int
die(const char *inst_name, const char *format, ...)
{
  char     buf[BUFSIZE];
  va_list  args;
  
  va_start(args, format);
  vsnprintf(buf, BUFSIZE, format, args);
  va_end(args);
  
  if (inst_name)
    error("%s FATAL ERROR [%s]:  %s", PREFIX, inst_name, buf);
  else
    error("%s FATAL ERROR:  %s", PREFIX, buf);
  //fprintf(stdout,"Fatal Error: %s",buf);
}
