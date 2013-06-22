include ../../../makefile.conf

CONNLIB = libhelmholtzconn.so
PFIELD_NAME = RTPitchPField

OBJS = $(PFIELD_NAME).o helmholtzglue.o
SRCS = $(PFIELD_NAME).cpp helmholtzglue.cpp

CXXFLAGS += -I. -I../../rtcmix

all: $(CONNLIB)

$(CONNLIB): depend $(OBJS)
	$(CXX) $(SHARED_LDFLAGS) -o $@ $(OBJS)

helmholtzglue.o: depend helmholtzglue.cpp
	$(CXX) $(CXXFLAGS) -c -o helmholtzglue.o helmholtzglue.cpp

depend:
	-$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(SRCS)' > depend

-include depend

install: all
	$(INSTALL) $(CONNLIB) $(LIBDESTDIR)

uninstall:
	$(RM) $(LIBDESTDIR)/$(CONNLIB)

clean:
	$(RM) *.o $(CONNLIB)

cleanall: clean
	$(RM) depend

