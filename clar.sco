rtsetparams(44100, 2)
load("CLAR")
makegen(1, 24, 1000, 0, 1, 1, 1)
makegen(2, 24, 1000, 0, 1, 1, 1)
CLAR(0, 1, 0.02, 78, 31, 7000, 0)
CLAR(1, 1, 0.02, 35, 4, 7000, 0)
CLAR(2, 1, 0.02, 35, 9, 7000, 0)
CLAR(3, 1, 0.02, 51, 20, 7000, 0.5)
