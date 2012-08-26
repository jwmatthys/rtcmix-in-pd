include ../../makefile.conf

CXXFLAGS += $(SOUND_DRIVER)
INCLUDES += -I$(INCLUDEDIR) -I../include -I../audio -I../sndlib -I./heap -I../conf

COMMON_CSRCS = \
check_byte_order.c \
command_line.c \
funderflow.c \
getsample.c \
infile.c \
io.c \
load_utils.c \
merror.c \
message.c \
minc_functions.c \
minout.c \
m_system.c \
printsf.c \
profile.c \
resetamp.c \
sampfunc.c \
sfcopy.c \
sfprint.c \
sfstats.c \
sound.c \
soundio.c \
tempo.c \
ug_intro.c \
gen/fdump.c \
gen/floc.c \
gen/fnscl.c \
gen/fplot.c \
gen/fsize.c \
gen/gen10.c \
gen/gen17.c \
gen/gen18.c \
gen/gen1.c \
gen/gen20.c \
gen/gen24.c \
gen/gen25.c \
gen/gen2.c \
gen/gen3.c \
gen/gen4.c \
gen/gen5.c \
gen/gen6.c \
gen/gen7.c \
gen/gen9.c \
gen/makegen.c \
gen/modgens.c


# BGG mm -- added mm_rtsetparams.cpp and rtprofile.cpp to the list below
COMMON_CPPSRCS = \
addcheckfunc.cpp \
addrtInst.cpp \
buffers.cpp \
bus_config.cpp \
checkInsts.cpp \
dispatch.cpp \
filter.cpp \
Instrument.cpp \
intraverse.cpp \
loader.cpp \
lfo.cpp \
minc_info.cpp \
DataFile.cpp \
mm_rtsetparams.cpp \
Option.cpp \
PField.cpp \
PFieldSet.cpp \
Random.cpp \
RefCounted.cpp \
RTcmix.cpp \
rtcmix_types.cpp \
rtcmix_wrappers.cpp \
rtgetin.cpp \
rtgetsamps.cpp \
rtinput.cpp \
rtoutput.cpp \
rtprofile.cpp \
rtsendsamps.cpp \
rtsetinput.cpp \
rtsetoutput.cpp \
rtsetparams.cpp \
rtwritesamps.cpp \
set_option.cpp \
table.cpp \
tableutils.cpp \
modtable.cpp \
ops.cpp \
utils.cpp \
rt_ug_intro.cpp \
connection.cpp \
converter.cpp \
monitor.cpp

MAIN_CPPSRCS = RTcmixMain.cpp main.cpp

# BGG mm -- we use it in  max/msp, listed above
# This is not linked into librtcmix
#rtprofile.cpp

IMBEDSRCS =

CPPOBJS = $(patsubst %.cpp,%.o,$(COMMON_CPPSRCS))

COBJS = $(patsubst %.c,%.o,$(COMMON_CSRCS))

MIX_INST = ../../insts/base/MIX/MIX.o
# BGG mm
WAVETABLE_INST = ../../insts/base/WAVETABLE/WAVETABLE.o

# BGG mm -- instrument objects
include INSTDOBJS
include INJGOBJS
include INSTKOBJS
include INMAXMSPOBJS
include INBGGOBJS
include INJOELOBJS

# BGG mm -- added WAVETABLE_INST INSTDOBJS INJGOBJS INSTKOBJS
#   INMAXMSPOBJS INBGGOBJS INJOELOBJS
COMMON_OBJS = $(CPPOBJS) $(COBJS) heap/rtHeap.o $(MIX_INST) \
	$(WAVETABLE_INST) $(INSTDOBJS) $(INJGOBJS) $(INSTKOBJS) \
	$(INMAXMSPOBJS) $(INBGGOBJS) $(INJOELOBJS) \
	../audio/audio_devices.o

ifeq ($(NPLAY_SUPPORT), TRUE)
   COMMON_OBJS += setnetplay.o
   CPPSRCS += setnetplay.cpp
endif

ifeq ($(ARCH), MACOSX_10.0)
   COMMON_OBJS += osx_workaround.o
endif

LIBCONF = ../conf/libconf.a
LIBMINC = ../parser/minc/libminc.a
AUDIOLIB = ../audio/librtaudio.a
SNDLIB = ../sndlib/sndlib.a
# BGG mm -- for the PField inlet control
LIBINLET = ../control/maxmsp/libinlet.a
# BGG mm -- for the PField pfbus control
LIBPFBUS = ../control/pfbus/libpfbus.a
# BGG UB -- for some reason had to include these explicitly
LIBJGOBJ = ../../insts/jg/objlib/objlib.a
LIBSTK = ../../insts/stk/stklib/stklib.a

# NB: these LIBS defs will be empty if the features are not configured
AUDIOLIB += $(FFTW_LIBS) $(JACK_LIBS)

LIBS = $(AUDIOLIB) $(SNDLIB) $(LIBCONF)

PROGS = MIX
MAIN_OBJS = RTcmixMain.o main.o
# BGG mm
#TARGETS = $(RTLIB) $(MAIN_OBJS) RTsockfuncs.o $(PROGS)
TARGETS = $(COMMON_OBJS) $(MAIN_OBJS) RTsockfuncs.o $(PROGS)

ifeq ($(PERL_SUPPORT), TRUE)
   IMBEDSRCS += pRTcmix.cpp
   TARGETS += pRTcmix.o
   PROGS += PMIX
endif
ifeq ($(PYTHON_SUPPORT), TRUE)
   IMBEDSRCS += pyRTcmix.cpp
   TARGETS += pyRTcmix.o
   PROGS += PYMIX
   ARCHFLAGS += -DPYTHON
   PYTHONLIBS = `$(PYTHON) ../parser/python/print_libpython.py`
   ifeq ($(PYTHON_EXT_STATIC_LINK), TRUE)
      PYEXT = ../parser/python/rtcmixmodule.o
   endif
endif

# This workaround converts the linker path "/usr/local/lib" to "/usr/lib" for machines
# which have perl installed in /usr/bin, but have the broken config which adds the
# wrong linker path anyway.

ifeq ($(PERL_IN_LOCAL), TRUE)
PERL_LDFLAGS = `$(PERL) -MExtUtils::Embed -e ldopts`
else
PERL_LDFLAGS = `$(PERL) -MExtUtils::Embed -e ldopts | sed 's/usr\/local/usr/'`
endif

# BGG mm
all: $(TARGETS)
#	$(INSTALL) main.o $(LIBDIR)
#	$(INSTALL) RTcmixMain.o $(LIBDIR)
#	$(INSTALL) RTsockfuncs.o $(LIBDIR)
ifeq ($(PERL_SUPPORT), TRUE)
	$(INSTALL) pRTcmix.o $(LIBDIR)
endif
ifeq ($(PYTHON_SUPPORT), TRUE)
	$(INSTALL) pyRTcmix.o $(LIBDIR)
endif
#	$(INSTALL) $(RTLIB) $(LIBDIR)

# rules to allow complete lib compile from this dir only

$(LIBCONF):
	(cd ../conf; $(MAKE) $(MFLAGS) all;)

$(LIBMINC):
	(cd ../parser; $(MAKE) $(MFLAGS) all;)

$(P_O):
	(cd ../parser; $(MAKE) $(MFLAGS) all;)

$(PY_O):
	(cd ../parser; $(MAKE) $(MFLAGS) all;)

$(SNDLIB):
	(cd ../sndlib; $(MAKE) $(MFLAGS) all;)

$(AUDIOLIB):
	(cd ../audio; $(MAKE) $(MFLAGS) all;)

# BGG mm -- for the PField inlet control
$(LIBINLET):
	(cd ../control/maxmsp; $(MAKE) $(MFLAGS) all;)

# BGG mm -- for the PField pfbuscontrol
$(LIBPFBUS):
	(cd ../control/pfbus; $(MAKE) $(MFLAGS) all;)

# BGG mm -- the INSTDOBJS objects
$(INSTDOBJS):
	(cd ../../insts/std; $(MAKE) $(MFLAGS) all;)

# BGG mm -- the INJGOBJS objects
$(INJGOBJS):
	(cd ../../insts/jg; $(MAKE) $(MFLAGS) all;)

# BGG mm -- the INSTKOBJS objects
$(INSTKOBJS):
	(cd ../../insts/stk; $(MAKE) $(MFLAGS) all;)

# BGG mm -- the INMAXMSPOBJS objects
$(INMAXMSPOBJS):
	(cd ../../insts/maxmsp; $(MAKE) $(MFLAGS) all;)

# BGG mm -- the INBGGOBJS objects
$(INBGGOBJS):
	(cd ../../insts/bgg; $(MAKE) $(MFLAGS) all;)

# BGG mm -- the INJOELOBJS objects
$(INJOELOBJS):
	(cd ../../insts/joel; $(MAKE) $(MFLAGS) all;)

loader.o: loader.cpp
	$(CXX) $(CFLAGS) -DSHAREDLIBDIR=\"$(LIBDESTDIR)\" -c loader.cpp

connection.o: connection.cpp
	$(CXX) $(CXXFLAGS) -DSHAREDLIBDIR=\"$(LIBDESTDIR)\" -c connection.cpp

addcheckfunc.o: addcheckfunc.cpp
	$(CXX) $(CXXFLAGS) -DSHAREDLIBDIR=\"$(LIBDESTDIR)\" -c addcheckfunc.cpp

RTcmix.o: RTcmix.cpp
	$(CXX) $(CXXFLAGS) -DSHAREDLIBDIR=\"$(LIBDESTDIR)\" -c RTcmix.cpp

# BGG mm
#$(RTLIB): depend $(COMMON_OBJS) $(LIBMINC) $(LIBS)
#	$(CXX) $(DYNAMIC_LDFLAGS) -o $@ $(COMMON_OBJS) $(LIBMINC) $(LIBS) $(SYSLIBS)

Option.o: Option.cpp Option.h
	$(CXX) $(CXXFLAGS) -DSHAREDLIBDIR=\"$(LIBDESTDIR)\" -c $< -o $@

$(MIX_INST):	
	(cd ../../insts/base/MIX; make all)

heap/rtHeap.o: heap/heap.o heap/queue.o
	@echo compiling heap.
	(cd heap; $(MAKE) $(MFLAGS) all;)

# rules for three executables we build

# BGG mm/(UB -- added $(UB_CFLAGS) $(LIBJGOBJ) $(LIBSTK) )
MIX: $(MAIN_OBJS) $(M_O) $(LIBS) $(PROFILE_O)
#	$(CXX) -o MIX $(DYN) $(MAIN_OBJS) $(PROFILE_O) $(M_O) -L. $(LDFLAGS)
	$(CXX) $(UB_CFLAGS) -shared -flat_namespace -o rtcmixdylib.so \
	$(COMMON_OBJS) $(LIBS) $(MAIN_OBJS) $(LIBINLET) $(LIBPFBUS) $(M_O) \
	$(LIBJGOBJ) $(LIBSTK) $(LIBMINC) \
	-L$(LIBDIR) -lgen $(SYS_LDFLAGS)

PMIX: $(MAIN_OBJS) $(P_O) $(LIBS) $(PROFILE_O)
ifeq ($(PERL_SUPPORT), TRUE)
	$(CXX) -o PMIX $(DYN) $(MAIN_OBJS) $(PROFILE_O) \
		$(P_O) -L. $(LDFLAGS) $(PERL_LDFLAGS)
else
	@echo "You haven't configured with PERL_SUPPORT!"
endif

PYMIX: $(MAIN_OBJS) $(PY_O) $(LIBS) $(PROFILE_O)
ifeq ($(PYTHON_SUPPORT), TRUE)
	$(CXX) -o PYMIX $(DYN) $(MAIN_OBJS) $(PROFILE_O) \
		$(PY_O) -L. $(LDFLAGS) $(PYEXT) $(PYTHONLIBS)
else
	@echo "You haven't configured with PYTHON_SUPPORT!"
endif

.PHONY: all standalone install standalone_install \
		uninstall standalone_uninstall clean cleanall

# BGG ub
#depend:
#ifeq ($(ARCH),LINUX)
#	-$(SHELL) -ec '$(CC) -M $(CFLAGS) $(COMMON_CSRCS) $(COMMON_CPPSRCS) $(IMBEDSRCS) $(MAIN_CPPSRCS) `$(PERL) -MExtUtils::Embed -e ccopts`' > depend
#endif
#ifeq ($(ARCH),SGI)
## Note: -MM is a gcc extension. If no gcc, use -M.
## Unfortunately, this doesn't work...   -JGG
##	$(SHELL) -ec '$(CC) -M $(CFLAGS) $(SRCS) $(CPPSRCS)' > depend
#endif

#-include depend

install:	standalone_install

standalone_install: all
	@if test ! -d $(DESTDIR); then mkdir $(DESTDIR); fi;
	$(INSTALL) MIX $(DESTDIR)/CMIX
ifeq ($(PERL_SUPPORT), TRUE)
	$(INSTALL) PMIX $(DESTDIR)/PCMIX
endif
ifeq ($(PYTHON_SUPPORT), TRUE)
	$(INSTALL) PYMIX $(DESTDIR)/PYCMIX
endif

uninstall: standalone_uninstall
	$(RM) $(LIBDIR)/$(RTLIB)
	$(RM) $(LIBDIR)/main.o
	$(RM) $(LIBDIR)/RTcmixMain.o
	$(RM) $(LIBDIR)/RTsockfuncs.o
	$(RM) $(LIBDIR)/pRTcmix.o
	$(RM) $(LIBDIR)/pyRTcmix.o

standalone_uninstall:
	$(RM) $(DESTDIR)/CMIX
	$(RM) $(DESTDIR)/PCMIX
	$(RM) $(DESTDIR)/PYCMIX

clean:
	(cd heap; $(MAKE) $(MFLAGS) clean;)
	$(RM) $(COMMON_OBJS) $(TARGETS) core rtcmixdylib.so

cleanall: clean
	$(RM) depend

