print_on(1);
load("SCRUB");
set_option("audio_on");
rtsetparams(44100, 2, 2048);
//rtinput("/home/dscott/sounds/Track11-EnglishSpokenMP.wav");
rtinput("/snd/gg2.aiff")
transp = 1;
dur = 5;
skip = 0;
// Play forward from time 10 for 5 seconds
SCRUB(0,skip,dur,1,transp, 16,40);

