rtsetparams(44100, 2)
load("FMINST")
load("WAVESHAPE")
load("STRUM")

makegen(1, 24, 1000, 0, 0, 3.5,1, 7,0)
makegen(2, 10, 1000, 1)
makegen(3, 24, 1000, 0,1, 7,0)
FMINST(0, 7, 20000, 8.00, 179, 0, 10, 0.1)


makegen(1, 24, 1000, 0,0, 3.5,1, 7,0)
makegen(2, 10, 1000, 1)
makegen(3, 17, 1000, 0.9, 0.3, -0.2, 0.6, -0.7)
makegen(4, 24, 1000, 0,0, 3.5,1, 7,0)
WAVESHAPE(0, 7, 7.02, 0, 1, 9000, 0.99)
makegen(1, 24, 1000, 0,0, 1.5,1, 7,0)
makegen(4, 24, 1000, 0,1, 7,0)
WAVESHAPE(4, 7, 6.091, 0, 1, 10000, 0.01, 0)
WAVESHAPE(4.1, 7, 6.091, 0, 1, 10000, 0.01, 1)


setline(0,1,1,1)
START1(0, 2, 6.08, 1, 1, 10, 0.05, 7.00, 0, 1, 10000, 2, 0.5)
makegen(10, 24, 1000, 0, 0, 1, 1, 2, 0)
BEND1(2, 4, 6.08, 7.00, 10, 1, 1, 10, 0.05, 7.00, 0, 1, 10000, 100, 0.5)
FRET1(6, .2, 6.10, 1, 1, 10, 0.05, 7.00, 0, 1, 10000, 0.5)
FRET1(6.2, .1, 7.00, 1, 1, 10, 0.05, 7.00, 0, 1, 10000, 0.5)
FRET1(6.3, .1, 7.02, 1, 1, 10, 0.05, 7.00, 0, 1, 10000, 0.5)
FRET1(6.4, .1, 7.00, 1, 1, 10, 0.05, 7.00, 0, 1, 10000, 0.5)
FRET1(6.5, 1, 6.10, 1, 1, 10, 0.5, 7.00, 0, 1, 10000, 0.5)
FRET1(7.5, 1, 6.10, 1, 1, 10, 0.5, 7.07, 0, 1, 10000, 0.5)
FRET1(8.5, 1, 6.10, 1, 1, 10, 0.5, 7.09, 0, 1, 10000, 0.5)
FRET1(9.5, 2, 6.10, 1, 1, 10, 0.5, 6.01, 0, 1, 10000, 0.5)
FRET1(11.5, 2, 6.10, 1, 1, 10, 0.5, 8.01, 0, 1, 10000, 0.5)