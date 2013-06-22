/* RTcmix  - Copyright (C) 2000  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
*/
#include <RTcmix.h>
#include <prototypes.h>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <heap.h>
#include "rtdefs.h"
#include <AudioDevice.h>
#include <Instrument.h>
#include <Option.h>
#include <bus.h>
#include "BusSlot.h"
#include <dbug.h>

using namespace std;

#undef TBUG
#undef ALLBUG
#undef DBUG
#undef WBUG /* this new one turns on prints of where we are */

// Temporary globals

static unsigned long bufEndSamp;
static int startupBufCount = 0;
static bool audioDone = false;

bool inTraverse(AudioDevice *device, void *arg);
bool doneTraverse(AudioDevice *device, void *arg);

int RTcmix::runMainLoop(void)
{
	Bool audio_configured = NO;

#ifdef WBUG	
	cout << "ENTERING runMainLoop() FUNCTION *****\n";
#endif

	// This lets signal handler know that we have gotten to this point.
	audioLoopStarted = 1;

	// Wait for the ok to go ahead
	::pthread_mutex_lock(&audio_config_lock);
	if (!audio_config) {
		if (Option::print())
			cout << "runMainLoop():  waiting for audio_config . . .\n";
	}
	::pthread_mutex_unlock(&audio_config_lock);

	while (!audio_configured) {
		::pthread_mutex_lock(&audio_config_lock);
		if (audio_config) {
			audio_configured = YES;
		}
		::pthread_mutex_unlock(&audio_config_lock);
		if (rtInteractive) {
			if (run_status == RT_GOOD || run_status == RT_PANIC)
				continue;
			else if (run_status == RT_SHUTDOWN)
				cout << "runMainLoop:  shutting down" << endl;
			else if (run_status == RT_ERROR)
				cout << "runMainLoop:  shutting down due to error" << endl;
			audioDone = true;
			return -1;
		}
	}

	if (audio_configured && rtInteractive) {
		if (Option::print())
			cout << "runMainLoop():  audio set." << endl;
	}

// BGG mm -- set this here for max-msp, the actual work is done by intraverse()
	rtInteractive = 1;

	// Initialize everything ... cause it's good practice
	bufStartSamp = 0;  // current end sample for buffer
	bufEndSamp = RTBUFSAMPS;

	// NOTE: audioin, aux and output buffers are zero'd during allocation

	if (rtsetparams_called) {
		// NOTE: This will be handled in the AudioDevice platform-specific.
		// Right now, the intraVerse function below calls rtsendzeros().
//		startupBufCount = ZERO_FRAMES_BEFORE / RTBUFSAMPS;
		startupBufCount = 0;

		// Device may have been closed by user interrupt.
		if (audioDevice && audioDevice->isOpen()) {
			// Set done callback on device.
			audioDevice->setStopCallback(doneTraverse, this);
			// Start audio output device, handing it our callback.
			if (audioDevice->start(inTraverse, this) != 0) {
				cerr << audioDevice->getLastError() << endl;
				audioDevice->close();
				audioDone = true;
				return -1;
			}
			return 0;	// Playing, thru HW and/or to FILE.
		}
	}
	audioDone = true;
	return -1;	// Not playing, signal caller not to wait.
}

int RTcmix::waitForMainLoop()
{
#ifdef WBUG	
	cout << "ENTERING waitForMainLoop() FUNCTION *****\n";
#endif
	while (!audioDone) {
		usleep(10000);
	}
	AudioDevice *tmp = audioDevice;
	audioDevice = NULL;
	delete tmp;
#ifdef WBUG	
	cout << "EXITING waitForMainLoop() FUNCTION *****\n";
#endif
	return 0;
}

// This is now the audio play callback

bool RTcmix::inTraverse(AudioDevice *device, void *arg)
{
	RTcmix *RTCore = (RTcmix *) arg;
	const BusSlot *iBus;
	unsigned long rtQchunkStart = 0;
	Bool panic = NO;
	short bus = -1, bus_count = 0, busq = 0;
	int i;
	short bus_q_offset = 0;

#ifdef WBUG
cout << "ENTERING inTraverse()\n";
#endif
#ifdef DBUG
	printf("Entering big loop .....................\n");
#endif

// BGG mm -- I need to find a better place to put this...
	bufEndSamp = bufStartSamp + RTBUFSAMPS;

	// send a buffer of zeros to audio output device.  NOTE:  This used to be
	// in the inTraverse init code.
	
	if (startupBufCount-- > 0) {
		return (rtsendzeros(device, 0) == 0) ? true : false;
	}

	if (rtInteractive && run_status == RT_PANIC)
		panic = YES;

	// Pop elements off rtHeap and insert into rtQueue +++++++++++++++++++++

	// deleteMin() returns top instrument if inst's start time is < bufEndSamp,
	// else NULL.  heapChunkStart is set in all cases

	unsigned long heapChunkStart = 0;
	Instrument *Iptr;

	while ((Iptr = rtHeap->deleteMin(bufEndSamp, &heapChunkStart)) != NULL) 
	{
#ifdef DBUG
		cout << "Iptr " << (void *) Iptr << " pulled from rtHeap" << endl;
		cout << "heapChunkStart = " << heapChunkStart << endl;
#endif
		if (panic) {
#ifdef DBUG
			cout << "Panic: Iptr " << (void *) Iptr << " unref'd" << endl;
#endif
			Iptr->unref();
			continue;
		}

		// Because we know this instrument will be run during this slot,
		// perform final configuration on it if we are not interactive.
		// (If interactive, this is handled at init() time).

		if (!rtInteractive) {
#ifdef ALLBUG
			cout << "Calling configure()" << endl;
#endif
			if (Iptr->configure(RTBUFSAMPS) != 0) {
#ifdef DBUG
				cerr << "Inst configure error: Iptr " << (void *) Iptr << " unref'd" << endl;
#endif
				Iptr->unref();
				continue;
			}
		}

		iBus = Iptr->getBusSlot();

		// DJT Now we push things onto different queues
		::pthread_mutex_lock(&bus_slot_lock);
		IBusClass bus_class = iBus->Class();
		switch (bus_class) {
		case TO_AUX:
			bus_count = iBus->auxout_count;
			bus_q_offset = 0;
			for(i=0;i<bus_count;i++) {
				bus = iBus->auxout[i];
				busq = bus+bus_q_offset;
#ifdef ALLBUG
				cout << "Pushing on TO_AUX[" << busq << "] rtQueue\n";
#endif
				rtQueue[busq].push(Iptr,heapChunkStart);
			}
			break;
		case AUX_TO_AUX:
			bus_count = iBus->auxout_count;
			bus_q_offset = MAXBUS;
			for(i=0;i<bus_count;i++) {
				bus = iBus->auxout[i];
				busq = bus+bus_q_offset;
#ifdef ALLBUG
				cout << "Pushing on AUX_TO_AUX[" << busq << "] rtQueue\n";
#endif
				rtQueue[busq].push(Iptr,heapChunkStart);
			}
			break;
		case TO_OUT:
			bus_count = iBus->out_count;
			bus_q_offset = MAXBUS*2;
			for(i=0;i<bus_count;i++) {
				bus = iBus->out[i];
				busq = bus+bus_q_offset;
#ifdef ALLBUG
				cout << "Pushing on TO_OUT[" << busq << "] rtQueue\n";
#endif
				rtQueue[busq].push(Iptr,heapChunkStart);
			}
			break;
		case TO_AUX_AND_OUT:
			bus_count = iBus->out_count;
			bus_q_offset = MAXBUS;
			for(i=0;i<bus_count;i++) {
				bus = iBus->out[i];
				busq = bus+bus_q_offset;
#ifdef ALLBUG
				cout << "Pushing on TO_OUT2[" << busq << "] rtQueue\n";
#endif
				rtQueue[busq].push(Iptr,heapChunkStart);
			}
			bus_count = iBus->auxout_count;
			bus_q_offset = 2*MAXBUS;
			for(i=0;i<bus_count;i++) {
				bus = iBus->auxout[i];
				busq = bus+bus_q_offset;
#ifdef ALLBUG
				cout << "Pushing on TO_AUX2[" << busq << "] rtQueue\n";
#endif
				rtQueue[busq].push(Iptr,heapChunkStart);
			}
			break;
		default:
			cout << "ERROR (intraverse): unknown bus_class\n";
			break;
		}
		::pthread_mutex_unlock(&bus_slot_lock);
	}
	// End rtHeap popping and rtQueue insertion ----------------------------

	BusType bus_type = BUS_OUT; // Default when none is set
	IBusClass qStatus = TO_AUX;
	short play_bus = 0;
	Bool aux_pb_done = NO;
	int rtQSize = 0, allQSize = 0;

	// rtQueue[] playback shuffling ++++++++++++++++++++++++++++++++++++++++
	while (!aux_pb_done) {

		switch (qStatus) {
		case TO_AUX:
			bus_q_offset = 0;
			bus_type = BUS_AUX_OUT;
			::pthread_mutex_lock(&to_aux_lock);
			bus = ToAuxPlayList[play_bus++];
			::pthread_mutex_unlock(&to_aux_lock);
			break;
		case AUX_TO_AUX:
			bus_q_offset = MAXBUS;
			::pthread_mutex_lock(&aux_to_aux_lock);
			bus = AuxToAuxPlayList[play_bus++];
			::pthread_mutex_unlock(&aux_to_aux_lock);
			bus_type = BUS_AUX_OUT;
			break;
		case TO_OUT:
			bus_q_offset = MAXBUS*2;
			::pthread_mutex_lock(&to_out_lock);
			bus = ToOutPlayList[play_bus++];
			::pthread_mutex_unlock(&to_out_lock);
			bus_type = BUS_OUT;
			break;
		default:
			cout << "ERROR (intraverse): unknown bus_class\n";
			break;
		}

		if (bus != -1) {
			busq = bus+bus_q_offset;
			rtQSize = rtQueue[busq].getSize();
			if (rtQSize > 0) {
				rtQchunkStart = rtQueue[busq].nextChunk();
				// DS ADDED
				assert(rtQchunkStart > 0 || bufStartSamp == 0);
			}
		}
		else {
			switch (qStatus) {
			case TO_AUX:
				qStatus = AUX_TO_AUX;
				play_bus = 0;
				break;
			case AUX_TO_AUX:
				qStatus = TO_OUT;
				play_bus = 0;
				break;
			case TO_OUT:
#ifdef ALLBUG
				cout << "aux_pb_done\n";
#endif
				aux_pb_done = YES;
				break;
			default:
				cout << "ERROR (intraverse): unknown bus_class\n";
				break;
			}
		}
#ifdef ALLBUG
		cout << "bus: " << bus << endl;
		cout << "busq:  " << busq << endl;
#endif

#ifdef MULTI_THREAD
#define MAX_INSTRUMENTS 32
		Instrument *instruments[MAX_INSTRUMENTS];
#endif

		// Play elements on queue (insert back in if needed) ++++++++++++++++++
		while (rtQSize > 0 && rtQchunkStart < bufEndSamp && bus != -1) {
			int chunksamps = 0;

			Iptr = rtQueue[busq].pop();  // get next instrument off queue
#ifdef DBUG
			cout << "Iptr " << (void *) Iptr << " popped from rtQueue " << busq << endl;
#endif			
			Iptr->set_ichunkstart(rtQchunkStart);

			unsigned long endsamp = Iptr->getendsamp();

			// difference in sample start (countdown)
			int offset = rtQchunkStart - bufStartSamp;  

			// DJT:  may have to expand here.  IE., conditional above
			// (rtQchunkStart >= bufStartSamp)
			// unlcear what that will do just now
			if (offset < 0) { // BGG: added this trap for robustness
				cout << "WARNING: the scheduler is behind the queue!" << endl;
				cout << "rtQchunkStart:  " << rtQchunkStart << endl;
				cout << "bufStartSamp:  " << bufStartSamp << endl;
				cout << "endsamp:  " << endsamp << endl;
				endsamp += offset;  // DJT:  added this (with hope)
				offset = 0;
			}

			Iptr->set_output_offset(offset);

			if (endsamp < bufEndSamp) {  // compute # of samples to write
				chunksamps = endsamp-rtQchunkStart;
			}
			else {
				chunksamps = bufEndSamp-rtQchunkStart;
			}
			if (chunksamps > RTBUFSAMPS) {
				cout << "ERROR: chunksamps: " << chunksamps << " limiting to " << RTBUFSAMPS << endl;
				chunksamps = RTBUFSAMPS;
			}

#ifdef DBUG
			cout << "Begin playback iteration==========\n";
			cout << "Q-rtQchunkStart:  " << rtQchunkStart << endl;
			cout << "bufEndSamp:  " << bufEndSamp << endl;
			cout << "RTBUFSAMPS:  " << RTBUFSAMPS << endl;
			cout << "endsamp:  " << endsamp << endl;
			cout << "offset:  " << offset << endl;
			cout << "chunksamps:  " << chunksamps << endl;
#endif      

			Iptr->setchunk(chunksamps);  // set "chunksamps"		 
#ifdef TBUG
			cout << "Iptr->exec(" << bus_type << "," << bus << ") [" << Iptr->name() << "]\n";
#endif		  
			int inst_chunk_finished = 0;

			// DT_PANIC_MOD
			if (!panic) {
				int inst_chunk_finished = Iptr->exec(bus_type, bus);    // write the samples * * * * * * * * * 
				endsamp = Iptr->getendsamp();
			}
			else // DT_PANIC_MOD ... just keep on incrementing endsamp
				endsamp += chunksamps;

			// ReQueue or unref ++++++++++++++++++++++++++++++++++++++++++++++
			if (endsamp > bufEndSamp && !panic) {
#ifdef DBUG
				cout << "re queueing inst " << (void *) Iptr << endl;
#endif
				rtQueue[busq].push(Iptr,rtQchunkStart+chunksamps);   // put back onto queue
			}
			else {
				iBus = Iptr->getBusSlot();

				if (qStatus == iBus->Class() && inst_chunk_finished) {
#ifdef DBUG
					cout << "unref'ing inst " << (void *) Iptr << endl;
#endif
					Iptr->unref();
				}
			}  // end rtQueue or unref ----------------------------------------

			// DJT:  not sure this check before new rtQchunkStart is necessary
			rtQSize = rtQueue[busq].getSize();
			if (rtQSize) {
				rtQchunkStart = rtQueue[busq].nextChunk(); /* FIXME:  crapping out */
				allQSize += rtQSize;                /* in RT situation sometimes */
			}
#ifdef DBUG
			cout << "rtQSize: " << rtQSize << endl;
			cout << "rtQchunkStart:  " << rtQchunkStart << endl;
			cout << "chunksamps:  " << chunksamps << endl;
			cout << "Iteration done==========\n";
#endif
		} // end Play elements on queue (insert back in if needed) -----------
	}  // end aux_pb_done --------------------------------------------------

	// Write buf to audio device - - - - - - - - - - - - - - - - - - - - -
#ifdef DBUG
	cout << "Writing samples----------\n";
	cout << "Q-rtQchunkStart:  " << rtQchunkStart << endl;
	cout << "bufEndSamp:  " << bufEndSamp << endl;
#endif

	// DT_PANIC_MOD
	if (!panic) {
		if (rtsendsamps(device) != 0) {
#ifdef WBUG
			cout << "EXITING inTraverse()\n";
#endif
			return false;
		}
	}

	elapsed += RTBUFSAMPS;	
	bufStartSamp += RTBUFSAMPS;
	bufEndSamp += RTBUFSAMPS;

	// zero the buffers
	clear_aux_buffers();
	clear_output_buffers();

	// read in an input buffer (if audio input is active)
	if (rtrecord) {
		// cout << "Reading data from audio device\n";
		// DT_PANIC_MOD
		if (!panic)
			rtgetsamps(device);
	}

	bool playEm = true;

	if (!rtInteractive) {  // Ending condition
		if ((rtHeap->getSize() == 0) && (allQSize == 0)) {
#ifdef ALLBUG
			cout << "heapSize:  " << rtHeap->getSize() << endl;
			cout << "rtQSize:  " << rtQSize << endl;
			cout << "PLAYEM = FALSE\n";
			cout << "The end\n\n";
#endif
			playEm = false;
		}
	}
	else {
		// Check status from other threads
		if (run_status == RT_SHUTDOWN) {
			cout << "inTraverse:  shutting down" << endl;
			playEm = false;
		}
		else if (run_status == RT_ERROR) {
			cout << "inTraverse:  shutting down due to error" << endl;
			playEm = false;
		}
		else if (panic && run_status == RT_GOOD) {
			cout << "inTraverse:  panic mode finished" << endl;
			panic = NO;
		}
		// DT_PANIC_MOD
		if (panic && (rtHeap->getSize() == 0) && (allQSize == 0))
			run_status = RT_GOOD;
	}
#ifdef WBUG
cout << "EXITING inTraverse()\n";
#endif
	return playEm;
}

bool RTcmix::doneTraverse(AudioDevice *device, void *arg)
{
	if (Option::print())
		cout << "closing ...\n";
#ifdef WBUG
	cout << "ENTERING doneTraverse()\n";
#endif
	rtreportstats(device);
	if (Option::print())
		cout << "\n";
	audioDone = true;	// This signals waitForMainLoop()
#ifdef WBUG
	cout << "EXITING doneTraverse() FUNCTION *****\n";
#endif

	return true;
}

