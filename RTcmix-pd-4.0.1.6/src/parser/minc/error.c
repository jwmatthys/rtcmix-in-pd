/* RTcmix  - Copyright (C) 2004  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "minc_internal.h"

#define BUFSIZE 1024

#ifndef vsnprintf
#define vsnprintf(str, sz, fmt, args)  vsprintf(str, fmt, args)
#endif

extern int yylineno;
// BGG mm -- don't exit
//static int exit_on_die = 1;
static int exit_on_die = 0;

// BGG mm -- these are to communicate with max/msp (from src/rtcmix/message.cpp)
extern void rtcmix_advise(const char *inst_name, const char *format, ...);
extern void warn(const char *inst_name, const char *format, ...);
extern void rterror(const char *inst_name, const char *format, ...);
extern int die(const char *inst_name, const char *format, ...);


void
sys_error(char *msg)
{
//   fprintf(stderr, "FATAL SYSTEM ERROR: %s\n", msg);

// BGG mm -- to communicate to max/msp
	die("parser", "%s\n", msg);

   if (exit_on_die)
      exit(EXIT_FAILURE);
}

int
configure_minc_error_handler(int exit)
{
   exit_on_die = exit;
   return 0;
}

void
minc_advise(const char *msg, ...)
{
   char buf[BUFSIZE];
   va_list args;

   va_start(args, msg);
   vsnprintf(buf, BUFSIZE, msg, args);
   va_end(args);

// BGG mm -- to communicate with max/msp
	rtcmix_advise("parser", buf);

//   fprintf(stderr, "%s\n", buf);
}

void
minc_warn(const char *msg, ...)
{
   char buf[BUFSIZE];
   va_list args;

   va_start(args, msg);
   vsnprintf(buf, BUFSIZE, msg, args);
   va_end(args);

// BGG mm -- to communicate with max/msp
	warn("parser", "%s (near line %d)\n", buf, yylineno); 

//   fprintf(stderr, "WARNING: %s (near line %d)\n", buf, yylineno);
}

void
minc_die(const char *msg, ...)
{
   char buf[BUFSIZE];
   va_list args;

   va_start(args, msg);
   vsnprintf(buf, BUFSIZE, msg, args);
   va_end(args);

// BGG mm -- to communicate with max/msp
	rterror("parser", "%s (near line %d)\n", buf, yylineno);

//   fprintf(stderr, "ERROR: %s (near line %d)\n", buf, yylineno);
   if (exit_on_die)
      exit(EXIT_FAILURE);
}

void
minc_internal_error(const char *msg, ...)
{
   char buf[BUFSIZE];
   va_list args;

   va_start(args, msg);
   vsnprintf(buf, BUFSIZE, msg, args);
   va_end(args);

// BGG mm -- to communicate with max/msp
	rterror("parser-program", "%s (near line %d)\n", buf, yylineno);

//   fprintf(stderr, "PROGRAM ERROR: %s (near line %d)\n", buf, yylineno);
   if (exit_on_die)
      exit(EXIT_FAILURE);
}

void
yyerror(char *msg)
{

//BGG mm -- to communicate with max/msp
	rterror("parser-yyerror", "near line %d: %s\n", yylineno, msg);

//   fprintf(stderr, "near line %d: %s\n", yylineno, msg);
}

