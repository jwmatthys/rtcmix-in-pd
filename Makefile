current: rtcmix~_linux

clean: ; rm -f *.pd_linux *.o
	- rm -rf lib

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

rtcmix~_linux: rtcmix~.pd_linux

.SUFFIXES: .pd_linux

LINUXCFLAGS = -DPD -O2 -fPIC -funroll-loops -fomit-frame-pointer \
	-Wall -W -Wshadow \
	-Wno-unused -Wno-parentheses -Wno-switch

LINUXINCLUDE = -I/usr/local/include/pd -I/usr/include/pdl2ork

rtcmix:
	RTcmix/configure
	make -C RTcmix clean
	make -C RTcmix

.c.pd_linux:
	cc $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	ld -shared -o $*.pd_linux $*.o -lc -lm
	strip --strip-unneeded $*.pd_linux
	rm $*.o
	test -d lib || mkdir lib
	cp RTcmix/src/rtcmix/*.so lib
	cp rtcmix_scripteditor.py lib
