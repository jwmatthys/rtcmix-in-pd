/* RTcmix  - Copyright (C) 2000  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/
/* rev'd for v2.3, JGG, 20-Feb-00 */
/* rev'd for v3.7 (converted to C++ source), DS, April-04 */

#include <RTcmix.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <prototypes.h>
#include <buffers.h>
#include "rtdefs.h"
#include <AudioDevice.h>
#include "Option.h"

// BGG mm
extern float *maxmsp_inbuf; // set in mm_rtsetparams()
// BGG mm -- use older nomalizing since we're not using AudioDevice
#define IN_GAIN_FACTOR 32768.0

/* ----------------------------------------------------------- rtgetsamps --- */
void
RTcmix::rtgetsamps(AudioDevice *inputDevice)
{
   float *in;
   int i,j;
   int bsamps, nchans;

	assert(Option::record() == true);

// BGG mm
// maxmsp_inbuf is a pointer to a max/msp buffer, passed in via
// maxmsp_rtsetparams

   bsamps = bufsamps();
   nchans = chans();
   in = maxmsp_inbuf;
   for(i = 0; i < bsamps; i++)
      for (j = 0; j < nchans; j++)
         audioin_buffer[j][i] = *in++ * IN_GAIN_FACTOR;

   return;

// BGG mm -- this isn't used in max/msp
	if (inputDevice->getFrames(audioin_buffer, RTBUFSAMPS) < 0)
	{
		fprintf(stderr, "rtgetsamps error: %s\n", inputDevice->getLastError());
	}
}


