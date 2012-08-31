include ../../makefile.conf

OBJS = headers.o io.o extra.o

all: sndlib.a

# BGG UB
#sndlib.a: $(OBJS)
#	$(RM) sndlib.a
#	$(LD) -r $(OBJS) -o sndlib.a
sndlib.a: $(OBJS)
	$(RM) $@
	ar q $@ $(OBJS)
	$(RANLIB) $@

install: all
uninstall:

clean: 
	$(RM) $(OBJS) sndlib.a

cleanall: clean

distclean: clean

