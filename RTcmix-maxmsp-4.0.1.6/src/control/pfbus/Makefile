# NOTE:  This will not build the correct loadable object lib
# for RTcmix!  It is intended to function to create a lib
# for the rtcmix~ object.  BGG 12/5/2009

include ../../../makefile.conf

OBJS = PFBusPField.o pfbusglue.o
SRCS = PFBusPField.cpp pfbusglue.cpp

CXXFLAGS += -I. -I../../rtcmix
CONNLIB = libpfbus.a

all: $(CONNLIB)

# BGG MM
#$(CONNLIB): depend $(OBJS)
#	$(CXX) $(SHARED_LDFLAGS) -o $@ $(OBJS)
$(CONNLIB): $(OBJS)
	$(RM) $@
	ar q $@ $(OBJS)
	$(RANLIB) $@

pfbusglue.o: pfbusglue.cpp PFBusPField.h
	$(CXX) $(CXXFLAGS) -c -o pfbusglue.o pfbusglue.cpp

install: all
#	$(INSTALL) $(CONNLIB) $(LIBDESTDIR)

uninstall:
#	$(RM) $(LIBDESTDIR)/$(CONNLIB)

clean:
	$(RM) *.o $(CONNLIB)

cleanall: clean

