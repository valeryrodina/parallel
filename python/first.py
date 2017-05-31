# -*- coding: cp1251 -*-

import sys
import subprocess
from math import sqrt

# arguments
args = list(sys.argv )

programm = "C:\\Users\\lera\\Documents\\Visual Studio 2013\\Projects\\parallel\\ParallelSimple\Release\ParallelSimple.exe"
inputFile = "C:\\Users\\lera\\Documents\\Visual Studio 2013\\Projects\\parallel\\ParallelSimple\\ParallelSimple\\Пикник на обочине.txt"
nGramm = "говорит Дик"
numRepeats = 100

if len(args) >= 5:    
    programm = args [1]
    inputFile = args [2]
    nGramm = args[3]
    numRepeats = int ( args [4])
    #sys.exit (" Program parameters: programm  path | input file | nGramm | num repeats")


# program
PIPE = subprocess.PIPE

for threads in [1]:
    timeList = []
    for num in range(numRepeats):
        p = subprocess.Popen([programm, inputFile, nGramm], stdout=PIPE)
        for line in p.stdout:
            if 'runtime without reading = ' in line :
                timeList.append(int(line.split()[-1]))        

    m=sum(timeList)/numRepeats
    disp = 0.00
    for val in timeList :
        disp = disp + (val - m) ** 2
        
    if numRepeats == 1:
        disp = disp / numRepeats
    else :
        disp = disp / ( numRepeats - 1)

    sigma = sqrt(disp)

    t=1.984
    interHigh = m + t*(sigma/(sqrt(numRepeats)))
    interLow = m - t*(sigma/(sqrt(numRepeats)))
    print("{} threads : average = {}, dispersion = {}, interval = [{}, {}]".format(1 , m , disp, interHigh, interLow))


