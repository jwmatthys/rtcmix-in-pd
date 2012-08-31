#ifndef _RTCMIXMAIN_H_
#define _RTCMIXMAIN_H_

#include <RTcmix.h>

class RTcmixMain : public RTcmix {
public:
	// BGG mm -- got rid of argc and argv for max/msp verison
	RTcmixMain();	// called from main.cpp
	void			run();	
	// BGG mm -- for flushing Queue/Heap from flush_sched() (main.cpp)
	void resetQueueHeap();

protected:
	// Initialization methods.
	void			parseArguments(int argc, char **argv);
	static void		interrupt_handler(int);
	static void		signal_handler(int);
	static void		set_sig_handlers();

	static void *	sockit(void *);
	
private:
	char *			makeDSOPath(const char *progPath);
	static int 		xargc;	// local copy of arg count
	static char *	xargv[/*MAXARGS + 1*/];
	static int 		interrupt_handler_called;
	static int 		signal_handler_called;
	static int		noParse;
	#ifdef NETAUDIO
	static int		netplay;     // for remote sound network playing
	#endif
	/* for more than 1 socket, set by -s flag to CMIX as offset from MYPORT */
	static int		socknew;
};

#endif	// _RTCMIXMAIN_H_
