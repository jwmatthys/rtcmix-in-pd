rtsetparams(44100, 1)
load("PFSCHED")
load("WAVETABLE")

delayed_envelope = maketable("line", 100, 0,0.0,  1,1.0)
value = makeconnection("pfbus", 1, 0.0)
PFSCHED(0.5, 3.5, 1, delayed_envelope)

delayed_LFO = makeLFO("sine", 5.0, 0, 0.03)
pvalue = makeconnection("pfbus", 2, 0.0)
PFSCHED(4.0, 3.5, 2, delayed_LFO)

wave = maketable("wave", 1000, "sine")

WAVETABLE(0, 7.0, 20000*value, 8.00+pvalue, 0.5, wave)

