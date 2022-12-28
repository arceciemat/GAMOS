import sys
import os

from checkPythonVersion import *

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()

verbose = 1
print(len(sys.argv))
if len(sys.argv) < 4 :
    print("!!! ERROR: NEEDS TWO ARGUMENTS: INPUT_FILE_NAME OUTPUT_FILE_NAME NUMBER_OF_COLUMNS (COLUMN_ORDER)")
    sys.exit()

fileIn = open(sys.argv[1])
fileOut = open(sys.argv[2],'w')
nColumn = int(sys.argv[3])
columnOrder = 1
if len(sys.argv) == 5 :
    columnOrder = int(sys.argv[4])

lines = fileIn.readlines()
nLinesWritten = 0
if verbose >= 1 :
    print("FILE OPENED ",sys.argv[1],len(lines))
if columnOrder == 1 :
    nStart = 0
    for iloop in range(0,999999):
        nLinesWritten = 0
        for line in lines :
            words=line.split()
            nwor = len(words)
            if verbose >= 3 : print("LINE line",nwor)
            if nwor%nColumn != 0 :
                print("!!! ERROR: NUMBER OF WORDS OF EACH LINE HAS TO BE A MULTIPLE OF ",nColumn,"LINE= ",line)
                sys.exit()

            if verbose >= 2 : print("nwor",nwor,nColumn,nStart)
            if nwor >= nColumn+nStart :
                nLinesWritten += 1
                for ii in range(0+nStart,nColumn+nStart) :
                    fileOut.write(words[ii]+" ")
                    if verbose >= 3 : print(ii,"WRITE",words[ii])
        
                fileOut.write('\n')
            if verbose >= 2 : print("nLinesWritten",nLinesWritten)
        nStart += nColumn
        if nLinesWritten == 0:
            break
        
elif columnOrder == 0 :
    for line in lines :
        words=line.split()
        nwor = len(words)
        nLines = int(nwor/int(nColumn))
        for ii in range(0,nLines) :
            for jj in range(0,nColumn) :
                fileOut.write(words[ii*nColumn+jj]+"  ")
            fileOut.write('\n')

fileOut.close()
        
