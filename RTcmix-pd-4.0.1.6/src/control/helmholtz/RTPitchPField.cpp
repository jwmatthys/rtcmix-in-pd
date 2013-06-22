/* RTcmix - Copyright (C) 2004  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
  the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/

#include <RTPitchPField.h>
#include <PField.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

extern double hh_pitch;

RTPitchPField::RTPitchPField()
  : RTNumberPField(0)
{
}

RTPitchPField::~RTPitchPField() {}

double RTPitchPField::doubleValue(double dummy) const
{
  return hh_pitch ? hh_pitch : -1;
}

