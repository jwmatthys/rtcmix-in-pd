rtsetparams(48000,2,2048)

load("WAVETABLE")
//load("BUTTER")
load("HELMHOLTZ")

rtinput("/home/jwmatthys/Music/lovesupreme.wav")
//bus_config("BUTTER","in 0-1", "aux 0-1 out")
//bus_config("HELMHOLTZ","aux 0-1 in", "out 0-1")
freq = makeconnection("helmholtz")
amp = maketable("window",1000,1)
//BUTTER(0,0,120, 1, "lowpass", 2, 1, 0, 0.5, 0, 1000)
HELMHOLTZ(0, 0, 120, amp, 0.9, 1500, 0)
WAVETABLE(0,120, 15000, freq, 0.5)
