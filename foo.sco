rtsetparams(44100, 2)
load("FLANGE")
load("JDELAY")
load("REVERBIT")

bus_config("WAVETABLE", "aux 0-1 out")
bus_config("FLANGE", "aux 0-1 in", "aux 10-11 out")
bus_config("JDELAY", "aux 10-11 in", "aux 4-5 out")
bus_config("REVERBIT", "aux 4-5 in", "out 0-1")

totdur = 30
masteramp = 1
atk = 2
dcy = 4
pitchtab = { 5, 5.001, 5.02, 5.03, 5.05, 5.07, 5.069, 5.1, 6 }
numnotes = len(pitchtab)
transposition = 2 // try 7 also, for some cool aliasing...
srand(2)
notedur = 0.1
incr = notedur + 0.015
maxampdb = 92
minampdb = 75
control_rate(20000) // need high control rate for short synth notes
env = maketable("line", 10000, 0, 0, 1, 1, 20, 0)
wavet = maketable("wave", 10000, 1, 0.9, 0.7, 0.5, 0.3, 0.2, 0.1, 0.05, .02)
ampdiff = maxampdb - minampdb
st = 0
while (st < totdur) {
index = trunc(random() * numnotes)
pitch = pchoct(octpch(pitchtab[index]) + octpch(transposition))
amp = ampdb(minampdb + (ampdiff * random()))
WAVETABLE(st, notedur, amp * env, pitch, pan=random(), wavet)
st += incr
}
reset(500)
amp = masteramp
resonance = 0.3
lowpitch = 5
moddepth = 90
modspeed = 0.08
wetdrymix = 0.5
flangetype = "IIR"
wavetabsize = 100000
wavet = maketable("wave", wavetabsize, "sine")
maxdelay = 1 / cpspch(lowpitch)
FLANGE(st=0, insk=0, totdur, amp, resonance, maxdelay, moddepth, modspeed, wetdrymix, flangetype, inchan=0, pan=1, ringdur=0, wavet)
wavet = maketable("wave3", wavetabsize, 1, 1, -180)
lowpitch += 0.07
maxdelay = 1 / cpspch(lowpitch)
FLANGE(st=0, insk=0, totdur, amp, resonance, maxdelay, moddepth, modspeed, wetdrymix, flangetype, inchan=1, pan=0, ringdur=0, wavet)
deltime = notedur * 2.2
regen = 0.7
wetdry = 0.12
cutoff = 0
ringdur = 2
env = maketable("line", 1000, 0, 0, atk, 1, totdur-dcy, 1, totdur, 0)
JDELAY(st=0, insk=0, totdur, amp * env, deltime, regen, ringdur, cutoff, wetdry, inchan=0, pan=1)
JDELAY(st=0.02, insk=0, totdur, amp * env, deltime, regen, ringdur, cutoff, wetdry, inchan=1, pan=0)

revtime = 1

revpct = 0.3


rtchandel = 0.05
cf = 0

REVERBIT(st=0, insk=0, totdur + ringdur, amp, revtime, revpct, rtchandel, cf)
x=100000
print_off()

