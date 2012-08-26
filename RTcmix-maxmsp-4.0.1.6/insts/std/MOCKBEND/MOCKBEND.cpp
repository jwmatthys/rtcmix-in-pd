/* MOCKBEND - perform a time-varying transpose on a mono input signal
   using cubic spline interpolation

   NOTE: This is a version of TRANSBEND that's designed to work with
   real-time input sources, such as microphone or aux bus.

   p0 = output start time
   p1 = input start time
   p2 = output duration (time to end if negative)
   p3 = amplitude multiplier
   p4 = gen index for pitch curve
   p5 = input channel [optional, default is 0]
   p6 = percent to left [optional, default is .5]

   Processes only one channel at a time.

   Assumes function table 1 is amplitude curve for the note.
   You can call setline for this.

   Uses the function table specified by p4 as the transposition curve
   for the note.  The interval values in this table are expressed in
   linear octaves.  You can call makegen(2, ...) for this.  So if you
   want to gliss from a perfect 5th above to a perfect 5th below, use:
      makegen(-2, 18, 1000, 0,octpch(.07), 1,octpch(-.07))

   Derived from Doug Scott's TRANSBEND by Ivica Ico Bukvic <ico at fuse.net>
   (2001-2)
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ugens.h>
#include "MOCKBEND.h"
#include <rt.h>

//#define DEBUG
//#define DEBUG_FULL


inline float interp(float y0, float y1, float y2, float t)
{
   float hy2, hy0, a, b, c;

   a = y0;
   hy0 = y0 / 2.0f;
   hy2 = y2 / 2.0f;
   b = (-3.0f * hy0) + (2.0f * y1) - hy2;
   c = hy0 - y1 + hy2;

   return (a + b * t + c * t * t);
}

MOCKBEND :: MOCKBEND() : Instrument()
{
   in = NULL;

   incount = 1;
   counter = 0.0;
   get_frame = 1;

   /* clear sample history */
   oldersig = 0.0;
   oldsig = 0.0;
   newsig = 0.0;

   //BUFAMP INIT
   //bufamp = 0.0;

   branch = 0;
}


MOCKBEND :: ~MOCKBEND()
{
   delete [] in;
}


int MOCKBEND :: init(double p[], int n_args)
{
   if (n_args < 5)
      return die("MOCKBEND", "Wrong number of args.");

   float outskip = p[0];
   float inskip = p[1];
   float dur = p[2];
   amp = p[3];
   int pgen = (int) p[4];
   inchan = (n_args > 5) ? (int) p[5] : 0;
   pctleft = (n_args > 6) ? p[6] : 0.5;

   if (dur < 0.0)
      dur = -dur - inskip;

   if (rtsetoutput(outskip, dur, this) == -1)
		return DONT_SCHEDULE;
   if (rtsetinput(inskip, this) == -1)
		return DONT_SCHEDULE;   // no input

   if (inchan >= inputChannels())
      return die("MOCKBEND", "You asked for channel %d of a %d-channel file.",
                                                    inchan, inputChannels());

   float interval = 0;
   pitchtable = floc(pgen);
   if (pitchtable) {
      int plen = fsize(pgen);
      float isum = 0;
      for (int loc = 0; loc < plen; loc++) {
          float pch = pitchtable[loc];
	      isum += octpch(pch);
      }
      interval = isum / plen;
#ifdef DEBUG
      printf("average interval: %g\n", interval);
#endif
      tableset(SR, dur, plen, ptabs);
   }
   else
      return die("MOCKBEND", "Unable to load pitch curve!");

   float averageInc = (double) cpsoct(10.0 + interval) / cpsoct(10.0);

#ifdef NOTYET
   float total_indur = (float) m_DUR(NULL, 0);
   float dur_to_read = dur * averageInc;
   if (inskip + dur_to_read > total_indur) {
      warn("MOCKBEND", "This note will read off the end of the input file.\n"
                    "You might not get the envelope decay you "
                    "expect from setline.\nReduce output duration.");
      /* no exit() */
   }
#endif

   /* total number of frames to read during life of inst */
   in_frames_left = (int) (nSamps() * averageInc + 0.5);

   /* to trigger first read in run() */
   inframe = RTBUFSAMPS;

   amptable = floc(1);
   if (amptable) {
      int amplen = fsize(1);
      tableset(SR, dur, amplen, tabs);
   }
   else {
      rtcmix_advise("MOCKBEND", "Setting phrase curve to all 1's.");
      aamp = amp;
   }

   skip = (int) (SR / (float) resetval);

   return nSamps();
}


int MOCKBEND :: configure()
{
   in = new float[inputChannels() * RTBUFSAMPS];
   return in ? 0 : -1;
}


int MOCKBEND :: run()
{
   const int out_frames = framesToRun();
   int       ibranch = 0;
   float     *outp;
   const float cpsoct10 = cpsoct(10.0);

   //UGLY HACK
   float bufampABC;
   int rampSize;
   float rampMultiplier;

   bufampABC = 0.0;
   rampSize = 100;
   rampMultiplier = 1.0 / rampSize;

   //printf(" rampSize %d rampMultiplier %g\n", rampSize, rampMultiplier);

#ifdef DEBUG
   printf("out_frames: %d  in_frames_left: %d\n", out_frames, in_frames_left);
#endif

   outp = outbuf;               /* point to inst private out buffer */

   for (int i = 0; i < out_frames; i++) {
      if (--branch <= 0) {
         if (amptable)
            aamp = rtcmix_table(currentFrame(), amptable, tabs) * amp;
         branch = skip;
      }
      while (get_frame) {
         if (inframe >= RTBUFSAMPS) {
            rtgetin(in, this, RTBUFSAMPS * inputChannels());

            in_frames_left -= RTBUFSAMPS;
#ifdef DEBUG
            printf("READ %d frames, in_frames_left: %d\n",
                                                  RTBUFSAMPS, in_frames_left);
#endif
            inframe = 0;
         }
         oldersig = oldsig;
         oldsig = newsig;

         newsig = in[(inframe * inputChannels()) + inchan];

         inframe++;
         incount++;

         if (counter - (double) incount < 0.5)
            get_frame = 0;
      }
/*
     if ( inframe == 0) { bufampABC = 0.0; }
     if ( inframe < rampSize && inframe != 0 ) { bufampABC = bufampABC + rampMultiplier; }
     if ( inframe >= RTBUFSAMPS - rampSize && inframe!=RTBUFSAMPS ) { bufampABC = bufampABC - rampMultiplier; }
     if ( inframe == RTBUFSAMPS ) { bufampABC = 0; }
     if ( inframe >=rampSize && inframe < (RTBUFSAMPS - rampSize) ) { bufampABC = 1; }
*/

/*
     if ( i == 0) { bufampABC = 0.0; }
     if ( i < rampSize && i != 0 ) { bufampABC = bufampABC + rampMultiplier; }
     if ( i >= RTBUFSAMPS - rampSize && i!=RTBUFSAMPS ) { bufampABC = bufampABC - rampMultiplier; }
     if ( i == RTBUFSAMPS ) { bufampABC = 0; }
     if ( i >=rampSize && i < (RTBUFSAMPS - rampSize) ) { bufampABC = 1; }
*/
     if ( inframe == 0) { bufampABC = 0.0; }
     if ( inframe < rampSize && inframe != 0 ) { bufampABC = rampMultiplier * inframe; }
     if ( inframe >= RTBUFSAMPS - rampSize && inframe!=RTBUFSAMPS ) { bufampABC = rampMultiplier * (RTBUFSAMPS - inframe); }
     if ( inframe == RTBUFSAMPS ) { bufampABC = 0; }
     if ( inframe >=rampSize && inframe < (RTBUFSAMPS - rampSize) ) { bufampABC = 1; }

/*     printf("i: %d inframe: %d aamp: %g bufamp %g  ",
             i, inframe, aamp, bufampABC);
     printf("interping %g, %g, %g => %g\n", oldersig, oldsig, newsig, outp[0]);
*/
      double frac = (counter - (double) incount) + 2.0;
      outp[0] = interp(oldersig, oldsig, newsig, frac) * (aamp * bufampABC); //* BUFFER ENVELOPE
      //outp[0] = interp(oldersig, oldsig, newsig, frac) * aamp;

#ifdef DEBUG_FULL
      printf("i: %d counter: %g incount: %d frac: %g inframe: %d cursamp: %d\n",
             i, counter, incount, frac, inframe, currentFrame());
      printf("interping %g, %g, %g => %g\n", oldersig, oldsig, newsig, outp[0]);
#endif

      if (outputChannels() == 2) {
         outp[1] = outp[0] * (1.0 - pctleft);
         outp[0] *= pctleft;
      }

      outp += outputChannels();
      increment();

      if (--ibranch <= 0) {
		   float interval = rtcmix_table(currentFrame(), pitchtable, ptabs);
	      incr = (double) cpsoct(10.0 + interval) / cpsoct10;
         ibranch = 20;
      }

      counter += incr;         /* keeps track of interp pointer */
      if (counter - (double) incount >= -0.5)
         get_frame = 1;
   }

#ifdef DEBUG
   printf("OUT %d frames\n\n", i);
#endif

   return framesToRun();
}


Instrument *makeMOCKBEND()
{
   MOCKBEND *inst;

   inst = new MOCKBEND();
   inst->set_bus_config("MOCKBEND");

   return inst;
}

/* BGG mm -- consolidates in src/rtcmix/rtprofile.cpp
void rtprofile()
{
   RT_INTRO("MOCKBEND", makeMOCKBEND);
}
*/

