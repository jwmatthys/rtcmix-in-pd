rtsetparams(44100,2,1024)
rtinput("/home/dscott/sounds/Track11-EnglishSpokenMP.wav")
load("MMOVE")
bus_config("MMOVE","in0","aox0-1")
bus_config("RVB","aix0-1","out0-1")

mikes(45,0.5)

dist_front=100
dist_right=130
dist_rear=-145
dist_left=-178
height=100;
rvbtime=2;
abs_fac=4;
space(dist_front,dist_right,dist_rear,dist_left,height,abs_fac,rvbtime)

/* 5 feet is min distance, 100 feet max, scale by 1/distance**1.5 */
set_attenuation_params(5.0, 100.0, 1.5);

insk=0
outsk=0
dur=15;
pre_amp=12
dist_mikes=2.2		/* for normal */
//dist_mikes=0.67	/* for binaural */
inchan=0

threshold(0.0005);

// slow, then zoom in and out again
path(0,50,-90, 10,50,0, 11,10,20, 12,80,30, 15,30,90);

// MMOVE does not have rvb level arg that MOVE had.  Handled in RVB call now
MMOVE(outsk,insk,dur,pre_amp,dist_mikes,inchan);

// Reverb gain increases over time
handle rvblevel;
rvblevel = maketable("line", 1024, 0, 0.01, 1, 1.0);
RVB(0, 0, dur+rvbtime+0.5, rvblevel);

