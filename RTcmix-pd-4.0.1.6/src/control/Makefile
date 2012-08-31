include ../../makefile.conf

# BGG mm
#DIRS = common midi mouse display datafile
#ifeq ($(OSC_SUPPORT), TRUE)
#	DIRS += osc
#endif
DIRS = maxmsp pfbus

all: $(DIRS) $(OBJS)
	@for DIR in $(DIRS); \
	do \
	  ( cd $$DIR; $(MAKE) $(MFLAGS) all; ) \
	done

.PHONY: install clean cleanall

install:
	@for DIR in $(DIRS); \
	do \
	  ( cd $$DIR; $(MAKE) $(MFLAGS) install; ) \
	done

uninstall:
	@for DIR in $(DIRS); \
	do \
	  ( cd $$DIR; $(MAKE) $(MFLAGS) uninstall; ) \
	done

# BGG UB
#depend:
#	@for DIR in $(DIRS); \
#	do \
#	  ( cd $$DIR; $(MAKE) $(MFLAGS) depend; ) \
#	done

clean:
	@for DIR in $(DIRS); \
	do \
	  ( cd $$DIR; $(MAKE) $(MFLAGS) clean; ) \
	done

cleanall:
	@for DIR in $(DIRS); \
	do \
	  ( cd $$DIR; $(MAKE) $(MFLAGS) cleanall; ) \
	done
