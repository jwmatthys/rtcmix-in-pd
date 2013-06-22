/* RTcmix - Copyright (C) 2004  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/
#ifndef _RTPITCHPFIELD_H_
#define _RTPITCHPFIELD_H_

#include <PField.h>

class RTPitchPField : public RTNumberPField {
 public:
  RTPitchPField();
  
  virtual double doubleValue(double dummy) const;

 protected:
  virtual ~RTPitchPField();
};

#endif // _RTPITCHPFIELD_H_
