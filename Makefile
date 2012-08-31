current: rtcmix~

clean: ; rm -f *.pd_linux *.o

# ----------------------- NT -----------------------

rtcmix~_nt: rtcmix~.dll

.SUFFIXES: .dll

PDNTCFLAGS = /W3 /WX /DNT /DPD /nologo
VC="C:\Program Files\Microsoft Visual Studio\Vc98"

PDNTINCLUDE = /I. /I\tcl\include /I..\src /I$(VC)\include

PDNTLDIR = $(VC)\lib
PDNTLIB = $(PDNTLDIR)\libc.lib \
	$(PDNTLDIR)\oldnames.lib \
	$(PDNTLDIR)\kernel32.lib \
	..\bin\pd.lib

.c.dll:
	cl $(PDNTCFLAGS) $(PDNTINCLUDE) /c $*.c
	link /dll /export:rtcmix_tilde_setup $*.obj $(PDNTLIB)

#-------------- Linux --------------------------------

textbuffer_linux: rtcmix~.pd_linux

.SUFFIXES: .pd_linux

LINUXCFLAGS = -DPD -O2 -fPIC -funroll-loops -fomit-frame-pointer \
	-Wall -W -Wshadow -Werror \
	-Wno-unused -Wno-parentheses -Wno-switch

LINUXINCLUDE =  -I../..

.c.pd_linux:
	cc $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	ld -shared -o $*.pd_linux $*.o -lc -lm
	strip --strip-unneeded $*.pd_linux
	rm $*.o
