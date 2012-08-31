#include <ugens.h>
#include <Instrument.h>
#include "FRET1.h"
#include <rt.h>
#include <rtdefs.h>

extern strumq *curstrumq[6];
extern delayq *curdelayq;

extern "C" {
	void sset(float, float, float, float, strumq*);
	float strum(float, strumq*);
	void delayset(float, float, delayq*);
	float dist(float);
	float delay(float, delayq*);
}

FRET1::FRET1() : Instrument()
{
	branch = 0;
}

int FRET1::init(double p[], int n_args)
{
// p0 = start; p1 = dur; p2 = pitch (oct.pc); p3 = fundamental decay time
// p4 = nyquist decay time; p5 = distortion gain; p6 = feedback gain
// p7 = feedback pitch (oct.pc); p8 = clean signal level
// p9 = distortion signal level; p10 = amp; p11 = stereo spread [optional]

	float	dur = p[1];

	if (rtsetoutput(p[0], dur, this) == -1)
		return DONT_SCHEDULE;

	strumq1 = curstrumq[0];
	freq = cpspch(p[2]);
	tf0 = p[3];
	tfN = p[4];

	dq = curdelayq;
	fbpitch = cpspch(p[7]);

	dgain = p[5];
	fbgain = p[6]/dgain;
	cleanlevel = p[8];
	distlevel = p[9];
	amp = p[10];
	spread = p[11];

	d = 0.0;

   amptable = floc(1);
	if (amptable) {
		int amplen = fsize(1);
		tableset(SR, dur, amplen, amptabs);
	}
	else {
		rtcmix_advise("FRET1", "Setting phrase curve to all 1's.");
		aamp = amp;
	}

	skip = (int)(SR / (float)resetval);

	firsttime = 0;

	return nSamps();
}

int FRET1::configure()
{
	return 0;
}

int FRET1::run()
{
	if (firsttime == 0) { // BGG -- has to modify strumq/delayq at runtime
		sset(SR, freq, tf0, tfN, strumq1);
		delayset(SR, fbpitch, dq);
		firsttime = 1;
	}

	for (int i = 0; i < framesToRun(); i++) {
		if (--branch <= 0) {
			if (amptable)
				aamp = tablei(currentFrame(), amptable, amptabs) * amp;
			branch = skip;
		}
		float a = strum(d, strumq1);
		float b = dist(dgain*a);
		d = fbgain*delay(b, dq);

		float out[2];
		out[0] = (cleanlevel*a + distlevel*b) * aamp;

		if (outputChannels() == 2) { /* split stereo files between the channels */
			out[1] = (1.0 - spread) * out[0];
			out[0] *= spread;
		}

		rtaddout(out);
		increment();
	}
	return framesToRun();
}


Instrument*
makeFRET1()
{
	FRET1 *inst;

	inst = new FRET1();
	inst->set_bus_config("FRET1");

	return inst;
}
