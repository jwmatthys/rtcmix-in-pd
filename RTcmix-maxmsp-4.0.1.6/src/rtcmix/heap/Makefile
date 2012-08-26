include ../../../makefile.conf

INCLUDES += -I.. -I../../include -I$(INCLUDEDIR)
SRCS = heap.cpp queue.cpp rtQueue.cpp
OBJS = heap.o queue.o rtQueue.o
PROG = rtHeap.o

all: $(PROG)

# BGG UB
#$(PROG): $(OBJS)
#	$(LD) -r $(OBJS) -o $(PROG)
$(PROG): $(OBJS)
	$(RM) $@
	ar q $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(RM) $(PROG) $(OBJS) depend

# BGG UB
#depend:
#ifeq ($(ARCH),LINUX)
#	-$(SHELL) -ec '$(CC) -M $(CFLAGS) $(SRCS)' > depend
#endif
#ifeq ($(ARCH),SGI)
## Note: -MM is a gcc extension. If no gcc, use -M.
## # Unfortunately, this doesn't work...   -JGG
## #   $(SHELL) -ec '$(CC) -M $(CFLAGS) $(SRCS)' > depend
#endif

#-include depend
