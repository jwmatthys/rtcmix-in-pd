/* RTcmix  - Copyright (C) 2004  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rtcmix_types.h>
#include <PField.h>
#include <utils.h>   // in ../../rtcmix
#include <RTPitchPField.h>
#include <ugens.h>		// for warn, die


// ------------------------------------------------------ helmholtz connection ---
//
// JWM
//
// use this to connect inlets from the HELMHOLTZ pitch tracker
//
//    create_handle calls create_pfield:
//
//    pitch = create_pfield();
//
//    score to use this might be:
//
//    rtinput("mySoundFile.wav")
//    HELMHOLTZ(0,0,120)
//    value = makeconnection("pitch")
//    INSTRUMENT(p1, p2, value, p3)
//

static RTNumberPField *
_inlet_usage()
{
  die("makeconnection (pitch)",
      "Usage: makeconnection(\"pitch\")");
  return NULL;
}

static RTNumberPField *
create_pfield(const Arg args[], const int nargs)
{
  if (nargs>0)
    return _inlet_usage();
  
  return new RTPitchPField();
}

// The following functions are the publically-visible ones called by the
// system.

extern "C" {
  Handle create_handle(const Arg args[], const int nargs);
};

Handle
create_handle(const Arg args[], const int nargs)
{
  PField *pField = create_pfield(args, nargs);
  Handle handle = NULL;
  if (pField != NULL) {
    handle = createPFieldHandle(pField);
  }
  return handle;
}
