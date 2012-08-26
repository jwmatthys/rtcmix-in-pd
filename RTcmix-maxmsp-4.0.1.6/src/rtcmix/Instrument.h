/* RTcmix  - Copyright (C) 2000  The RTcmix Development Team
   See ``AUTHORS'' for a list of contributors. See ``LICENSE'' for
   the license to this software and for a DISCLAIMER OF ALL WARRANTIES.
 */
#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_ 1

#include <RefCounted.h>
#include <bus.h>
#include <rt_types.h>
#include <sys/types.h>

// BGG mm -- added this for mm_buf
#include "rtdefs.h"

#define MAXNUMPARAMS 100

class heap;
class PFieldSet;
class PField;
class BusSlot;

struct InputState {
   InputState();
   int            fdIndex;         // index into unix input file desc. table
   off_t          fileOffset;      // current offset in file for this inst
   double         inputsr;		   // SR of input file
   int            inputchans;	   // Chans of input file
// BGG mm
	mm_buf			*my_mm_buf;		// local copy of the active [buffer~]
};

class Instrument : public RefCounted {
protected:

	// These replace the old globals
   static int	  RTBUFSAMPS;
   static int     NCHANS;
   static float   SR;

   float          _start;
   float          _dur;
   int            cursamp;
   int            chunksamps;
   int            i_chunkstart;   // we need this for rtperf
   int            endsamp;
   int            output_offset;

   int            sfile_on;        // a soundfile is open (for closing later)

   InputState     _input;		   // collected state for input source

   int            outputchans;

   int            mytag;           // for note tagging/rtupdate() 

   BUFTYPE        *outbuf;         // private interleaved buffer

   BusSlot        *_busSlot;
   PFieldSet	  *_pfields;
   static double  s_dArray[];
#ifdef RTUPDATE
   // new RSD variables
   EnvType rsd_env;
   int rise_samps, sustain_samps, decay_samps;
   float *rise_table,r_tabs[2];
   float *sustain_table,s_tabs[2];
   float *decay_table,d_tabs[2];
   float rise_time, sustain_time, decay_time;
   int _startOffset;
   int rsd_samp;
#endif /* RTUPDATE */

private:
   char 		  *_name;	// the name of this instrument
   BUFTYPE        *obufptr;
   bool           bufferWritten[MAXBUS];
   bool           needs_to_run;
   int            _skip;
   int            _nsamps;

   static pthread_mutex_t endsamp_lock;

#ifdef RTUPDATE
// stores the current index into the pfpath array
   int			  cumulative_size[MAXNUMPARAMS];

// stores the current index for each 'j' value
   int 			  pfpathcounter[MAXNUMPARAMS];

// used in linear interpolation to store the updated value
   double 		  newpvalue[MAXNUMPARAMS];

// used in linear interpolation to store the last updated value
   double 		  oldpvalue[MAXNUMPARAMS][2];

// These are the tables used by the gen interpolation
   float		  *ptables[MAXNUMPARAMS];
   float 		  ptabs[MAXNUMPARAMS][2];

// stores the sample number when updating begins to allow valid indexing into
// the gen array
   int 			  oldsamp[MAXNUMPARAMS];

// stores the current index into the pipath array
   int			  cumulative_isize[MAXNUMPARAMS];

// The instrument number and instrument slot number
   int 			  instnum;
   int			  slot;
   
	
// used to keep track of which set of data in the pfpath array we are looking 
// at.  This allows multiple calls to note_pfield_path and inst_pfield_path
// with different envelopes specified
   int			  j[MAXNUMPARAMS];
   int 			  k[MAXNUMPARAMS];
#endif /* RTUPDATE */

public:
	// Instruments should use these to access variables.
	int				currentFrame() const { return cursamp; }
	int				framesToRun() const { return chunksamps; }
	int				nSamps() const { return _nsamps; }
	int				inputChannels() const { return _input.inputchans; }
	int				outputChannels() const { return outputchans; }
	int				getSkip() const { return _skip; }

// BGG mm -- arg!
	InputState		getInputState() { return _input; }

	// Use this to increment cursamp inside single-frame run loops.
	void			increment() { ++cursamp; }
	// Use this to increment cursamp inside block-based run loops.
	void	    	increment(int amount) { cursamp += amount; }
	// These inlines are declared at bottom of this header.
	inline float	getstart();
	inline float	getdur();
	inline int		getendsamp();
	void			setendsamp(int);
	inline void		setchunk(int);
	inline void		set_ichunkstart(int);
	inline void		set_output_offset(int);
	inline const BusSlot *	getBusSlot() const;

	void 			schedule(heap *rtHeap);
	void			set_bus_config(const char *);
	virtual int		setup(PFieldSet *);				// Called by checkInsts()
	virtual int		init(double *, int);			// Called by setup()
	int				configure(int bufsamps);		// Called by inTraverse
	int				run(bool needsTo);
	virtual int		update(double *, int, unsigned fields=0);	// Called by run()
	double			update(int, int totframes=0);

	int				exec(BusType bus_type, int bus);
	void			addout(BusType bus_type, int bus);
	bool			isDone() const { return cursamp >= _nsamps; }
	const char *	name() const { return _name; }

	// These are called by the base class methods declared above.

	virtual int		configure();	// Sometimes overridden in derived class.
	virtual int		run() = 0;		// Always redefined in derived class.

#ifdef RTUPDATE
   float			rtupdate(int, int);  // tag, p-field for return value
   void				pf_path_update(int tag, int pval);
   void				pi_path_update(int pval);
   void				set_instnum(char* name);

   void				RSD_setup(int RISE_SLOT, int SUSTAIN_SLOT,
              				  int DECAY_SLOT, float duration);
   void				RSD_check();
   float			RSD_get();
#endif /* RTUPDATE */

// BGG -- added this for Ortgetin object support (see lib/Ortgetin.C)
   friend			class Ortgetin;

protected:
   // Methods which are called from within other methods
	Instrument();
	virtual		~Instrument();	// never called directly -- use unref()
   
	// This is called by set_bus_config() ONLY.
    void			setName(const char *name);

	static int		rtsetoutput(float, float, Instrument *);
	static int		rtsetinput(float, Instrument *);
	static int		rtinrepos(Instrument *, int, int);
	static int		rtgetin(float *, Instrument *, int);
	int rtaddout(BUFTYPE samps[]);  				// replacement for old rtaddout
	int rtbaddout(BUFTYPE samps[], int length);	// block version of same

	const PField &	getPField(int index) const;
	const double *	getPFieldTable(int index, int *tableLen) const;

private:
   void				gone(); // decrements reference to input soundfile
};

/* ------------------------------------------------------------- getstart --- */
inline float Instrument::getstart()
{
   return _start;
}

/* --------------------------------------------------------------- getdur --- */
inline float Instrument::getdur()
{
   return _dur;
}

/* ----------------------------------------------------------- getendsamp --- */
inline int Instrument::getendsamp()
{
   return endsamp;
}


/* ------------------------------------------------------------- setchunk --- */
inline void Instrument::setchunk(int csamps)
{
   chunksamps = csamps;
}

/* ------------------------------------------------------ set_ichunkstart --- */
inline void Instrument::set_ichunkstart(int csamps)
{
   i_chunkstart = csamps;
}

/* ---------------------------------------------------- set_output_offset --- */
inline void Instrument::set_output_offset(int offset)
{
   output_offset = offset;
}

inline const BusSlot *	Instrument::getBusSlot() const
{
	return _busSlot;
}

#endif /* _INSTRUMENT_H_  */

