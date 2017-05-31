# -*- coding: cp1251 -*-

import sys
import subprocess
from math import sqrt

# arguments
args = list(sys.argv )

programm = "D:\University\Master2\Parallel\C\ParallelTread\Release\ParallelTread.exe"
inputFile = "D:\University\Master2\Parallel\C\ParallelTread\ParallelTread\Пикник на обочине.txt"
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

for threads in [1, 2, 4, 8]:#[4]:#
    timeList = []
    for num in range(numRepeats):
        p = subprocess.Popen([programm, inputFile, str(threads)], stdout=PIPE)
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
    print("{} threads : average = {}, dispersion = {}, interval = [{}, {}]".format(threads , m , disp, interHigh, interLow))


