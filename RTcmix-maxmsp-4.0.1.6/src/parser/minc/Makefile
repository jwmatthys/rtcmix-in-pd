include ../../../makefile.conf

INCLUDES += -I../../include -I../../rtcmix
OBJS = y.tab.o builtin.o callextfunc.o error.o sym.o trees.o utils.o handle.o
SRCS = builtin.c callextfunc.cpp error.c sym.c trees.c utils.c handle.c
MINC = libminc.a

LSRC = minc.l
LEX = flex
LEXFLAGS = -l #-d    # -d for debug output

YSRC = minc.y
YACC = yacc
#YACC = bison -y
YFLAGS = -d #-v

all: $(MINC)

# BGG UB
#$(MINC): depend $(OBJS)
#	$(LD) -r $(OBJS) -o $(MINC)
$(MINC): $(OBJS)
	$(RM) $@
	ar q $@ $(OBJS)
	$(RANLIB) $@

# BGG mm -- modified these by hand, not sure how to imbed the changes in the
#  lex.l and y.tab.c source files, so I don't want them overwritten
#lex.yy.c: $(LSRC) minc_internal.h
#	$(LEX) $(LEXFLAGS) $(LSRC)
#
#y.tab.c: lex.yy.c $(YSRC) minc_internal.h
#	$(YACC) $(YFLAGS) $(YSRC)

# BGG UB
#depend:
#	-$(SHELL) -ec '$(CC) -M $(CFLAGS) $(SRCS)' > depend

#-include depend

install: all
uninstall:

# BGG mm -- see note above about lex.yy.c and y.tab.c files
clean:
	$(RM) *.o $(MINC) y.output
#	$(RM) *.o $(MINC) lex.yy.c y.tab.c y.tab.h y.output depend

cleanall: clean
	$(RM) depend

