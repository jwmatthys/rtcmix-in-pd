#include "HOLO.h"

#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include <rt.h>
#include <rtdefs.h>

static const float sameSideCoeffs[] = {
0.00027003, -0.00030861, 0.00069436, -0.00150446, 0.00277746, -0.00204452, 
0.00069436, 0.00212167, -0.00825522, 0.01843922, -0.03417814, 0.05786367, 
-0.09238899, 0.14604791, -0.25047255, 0.71608222, 1.00000000, -0.39586467, 
0.07306253, -0.04478648, 0.10616055, 0.07638005, 0.00393473, -0.00671219, 
-0.04941558, -0.02650156, -0.04108321, -0.02106238, -0.02430274, -0.01747483, 
-0.01921074, -0.01932647, -0.02167959, -0.02387841, -0.02623153, -0.02719593, 
-0.02873896, -0.02924044, -0.02970335, -0.03012769, -0.02993481, -0.02974193, 
-0.03020484, -0.03001196, -0.03008911, -0.02966478, -0.02978050, -0.02974193, 
-0.02966478, -0.02920187, -0.02893184, -0.02885468, -0.02823747, -0.02804459, 
-0.02758168, -0.02738881, -0.02723450, -0.02638583, -0.02588435, -0.02573005, 
-0.02538287, -0.02511283, -0.02445705, -0.02387841, -0.02364695, -0.02318404, 
-0.02252826, -0.02233538, -0.02160244, -0.02110095, -0.02086950, -0.02002083, 
-0.01975080, -0.01967365, -0.01878641, -0.01816919, -0.01755198, -0.01751341, 
-0.01755198, -0.01662616, -0.01612468, -0.01581607, -0.01546889, -0.01508313, 
-0.01492883, -0.01446592, -0.01415731, -0.01357868, -0.01300004, -0.01315434, 
-0.01249855, -0.01195849, -0.01184276, -0.01157273, -0.01095552, -0.01056976, 
-0.01022258, -0.01018401, -0.00983682, -0.00945107, -0.00948964, -0.00918104, 
-0.00856382, -0.00864097, -0.00779231, -0.00767658, -0.00736797, -0.00725225, 
-0.00721367, -0.00663503, -0.00651931, -0.00613355, -0.00617213, -0.00570922, 
-0.00594067, -0.00574779, -0.00493770, -0.00501485, -0.00470625, -0.00439764, 
-0.00443621, -0.00435906, -0.00381900, -0.00370327, -0.00378043, -0.00335609, 
-0.00370327, -0.00297034, -0.00297034, -0.00285461, -0.00277746, -0.00277746, 
-0.00235312, -0.00231455, -0.00204452, -0.00192879, -0.00165876, -0.00173591, 
-0.00177449, -0.00138873, -0.00138873, -0.00096439, -0.00061721, -0.00092582, 
-0.00096439, -0.00077152, -0.00073294, -0.00042433, -0.00019288, -0.00015430, 
-0.00027003, -0.00015430, -0.00015430, -0.00046291
};

static const float oppositeSideCoeffs[] = {
0.00003858, -0.00011573, -0.00027003, 0.00015430, 0.00003858, 0.00007715, 
-0.00007715, -0.00100297, 0.00258458, -0.00567064, 0.01014543, -0.01681904, 
0.02561432, -0.03749566, 0.05381322, -0.09389345, -0.04362921, -0.03819003, 
0.04783397, 0.30498013, -0.61057746, -0.29514331, 0.04204760, 0.00023145, 
0.01751341, -0.04112178, -0.00594067, -0.02364695, 0.00073294, -0.00354897, 
0.00536203, 0.00366470, 0.00443621, 0.00401188, 0.00412761, 0.00374185, 
0.00362612, 0.00366470, 0.00459052, 0.00470625, 0.00505343, 0.00528488, 
0.00559349, 0.00590209, 0.00632643, 0.00559349, 0.00582494, 0.00605640, 
0.00559349, 0.00578637, 0.00586352, 0.00586352, 0.00536203, 0.00532346, 
0.00516915, 0.00524631, 0.00513058, 0.00513058, 0.00497628, 0.00497628, 
0.00478340, 0.00447479, 0.00455194, 0.00439764, 0.00424334, 0.00401188, 
0.00405046, 0.00412761, 0.00416618, 0.00401188, 0.00339467, 0.00339467, 
0.00343324, 0.00354897, 0.00358755, 0.00354897, 0.00320179, 0.00304749, 
0.00335609, 0.00320179, 0.00293176, 0.00281603, 0.00297034, 0.00277746, 
0.00254600, 0.00273888, 0.00273888, 0.00250743, 0.00316321, 0.00304749, 
0.00273888, 0.00300891, 0.00273888, 0.00285461, 0.00320179, 0.00262315, 
0.00285461, 0.00273888, 0.00293176, 0.00289318, 0.00266173, 0.00297034, 
0.00258458, 0.00277746, 0.00297034, 0.00327894, 0.00327894, 0.00289318, 
0.00320179, 0.00312464, 0.00320179, 0.00324037, 0.00300891, 0.00339467, 
0.00293176, 0.00312464, 0.00358755, 0.00324037, 0.00335609, 0.00331752, 
0.00316321, 0.00339467, 0.00343324, 0.00331752, 0.00358755, 0.00358755, 
0.00373882, 0.00361747, 0.00370250, 0.00349219, 0.00384714, 0.00384342, 
0.00351026, 0.00368659, 0.00324881, 0.00353955, 0.00344369, 0.00324235, 
0.00313565, 0.00287386, 0.00284631, 0.00280919, 0.00284373, 0.00242554, 
0.00224600, 0.00235154, 0.00191342, 0.00176631, 0.00164860, 0.00140907, 
0.00115883, 0.00108165, 0.00001302, 0.00000456
};

const int nCoeffs = sizeof(sameSideCoeffs) / sizeof(sameSideCoeffs[0]);

HOLO::HOLO() : Instrument(), in(NULL), out(NULL)
{
	count = 0;
    pastsamps[0] = pastsamps[1] = NULL;
    pastsamps2[0] = pastsamps2[1] = NULL;
}

HOLO::~HOLO()
{
    for (int n = 0; n < 2; n++) {
		delete [] pastsamps[n];
		delete [] pastsamps2[n];
	}
	delete [] out;
	delete [] in;
}


int HOLO::init(double p[], int n_args)
{
/* HOLO: stereo FIR filter to perform crosstalk cancellation
*
*  p0 = outsk
*  p1 = insk
*  p2 = dur
*  p3 = amp
*  p4 = xtalk amp mult
*
*/

	int i, rvin;

	if (inputChannels() != 2) {
		return die("HOLO", "Input must be stereo.");
	}
	if (outputChannels() != 2) {
		return die("HOLO", "Output must be stereo.");
	}

	rvin = rtsetinput(p[1], this);
	if (rvin == -1) { // no input
		return(DONT_SCHEDULE);
	}
	rvin = rtsetoutput(p[0], p[2], this);
	if (rvin == -1) { // no output
		return(DONT_SCHEDULE);
	}

	ncoefs = nCoeffs;

	for (int n = 0; n < 2; n++) {
		pastsamps[n] = new float[ncoefs + 1];
		pastsamps2[n] = new float[ncoefs + 1];
		for (i = 0; i < ncoefs; i++) {
			pastsamps[n][i] = 0.0;
			pastsamps2[n][i] = 0.0;
		}
	}

	amp = p[3];

	xtalkAmp = (p[4] != 0.0) ? p[4] : 1.0;

	intap = 0;

	skip = (int)(SR / (float) resetval);

	return 0;
}

int HOLO::configure()
{
	in = new float [RTBUFSAMPS * inputChannels()];
	out = new float [RTBUFSAMPS * inputChannels()];
	return (in && out) ? 0 : -1;
}

int HOLO::run()
{
	int i, j;
	float output[2];

	int rsamps = framesToRun()*inputChannels();
	rtgetin(in, this, rsamps);

	for (i = 0; i < rsamps; i += 2) {
		if (--count <= 0) {
			double p[5];
			update(p, 5);
			amp = p[3];
			xtalkAmp = (p[4] != 0.0) ? p[4] : 1.0;
			count = skip;
		}
	    for (int n = 0; n < 2; n++) {
		    int c;
			output[n] = 0.0;
			pastsamps[n][intap] = in[i+n];

			// sum all past samples * coefficients
			// two loops to avoid bounds checking

			for (j = intap, c = 0; j < ncoefs; j++, c++) 
				output[n] += (pastsamps[n][j] * sameSideCoeffs[c]);

			const int remaining = ncoefs - c;

			for (j = 0; j < remaining; j++, c++) 
				output[n] += (pastsamps[n][j] * sameSideCoeffs[c]);

			// feed signal to opposite side
			pastsamps2[n][intap] = in[i + 1 - n];

			// add this into output for each side

			for (j = intap, c = 0; j < ncoefs; j++, c++) 
				output[n] += xtalkAmp * (pastsamps2[n][j] * oppositeSideCoeffs[c]);

			const int remaining2 = ncoefs - c;

			for (j = 0; j < remaining2; j++, c++) 
				output[n] += xtalkAmp * (pastsamps2[n][j] * oppositeSideCoeffs[c]);

			output[n] *= amp;
		}
		rtaddout(output);
		increment();
		if (--intap < 0)
		    intap = ncoefs - 1;
	}
	return(i);
}



Instrument*
makeHOLO()
{
	HOLO *inst;

	inst = new HOLO();
	inst->set_bus_config("HOLO");

	return inst;
}

/* BGG mm -- consolidates in src/rtcmix/rtprofile.cpp
void
rtprofile()
{
	RT_INTRO("HOLO",makeHOLO);
}
*/
