#rtcmix-in-pd

Copyright 2012 by Joel Matthys

RTcmix music language embedded in PureData

## Description

RTcmix is a real-time software language for digital sound synthesis and signal processing. Using a lightweight C-like syntax, it includes Perry Cook and Gary Scavone's Synthesis Toolkit, as well as advanced and unique signal processing instruments created by Brad Garton, John Gibson, Mara Helmuth and many others.

Run as a PureData external, RTcmix can be integrated with Pd's great patching and routing functions. It is also easy to distribute as a prepackaged binary, potentially reaching a wider audience.

The source is staged at https://github.com/jwmatthys/rtcmix-in-pd

This project is based on Brad Garton's rtcmix~ 1.81 for Max/MSP.
http://music.columbia.edu/cmc/RTcmix/rtcmix~/

## INSTALLATION

To install the binary, unzip and copy the entire folder to somewhere on your Pd search path. You will need the python-tk package installed to use the script editor. (On Debian: apt-get install python-tk)

To compile from source, first enter the RTcmix folder and ./configure; make clean; make. You'll need bison and flex; not sure what else.
Then, in the rtcmix-in-pd root folder, make clean and make; that's it.

## LICENSE
distributed under the terms of the Lesser GNU Public License
