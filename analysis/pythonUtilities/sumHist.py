import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader

hfOutName = ""

###### -------------- init ----------- ######
checkPythonVersion()
verbose = 1

if len(sys.argv) < 3 :
    print("!!! ERROR: at least two arguments are needed",
          "OUT_FILE IN_FILE")
    sys.exit()
    
if verbose >= 3 : print("hfOutName=",hfOutName)

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......#
def addFiles(*args) :

    nFiles = 0
    
    for ii in range(len(theHisFileNames)):
        hfName = theHisFileNames[ii]
        fWeight = theHisFileWeights[ii]
        if verbose >= 1 : print("READING FILE= ",hfName,"wei=",fWeight,":",ii+1,"/",len(theHisFileNames),)
        histoFile = csvHistoReader(hfName)
        if verbose >= 3 : print("FILE READ N_HISTOS",len(histoFile.fHistos1D))
        for his1 in histoFile.fHistos1D :
            if fWeight != 1. :
                his1 = his1.Mult(fWeight)
            hName = his1.name
            if verbose >= 3 : print("LOOP HISTO= ",hName)
            if hName in histoDict.keys() :
                if verbose >= 3 : print(" adding histo",his1.name)
                if not bAdd :
                    histoDict[hName] = histoDict[hName].Average(his1)
                else :
                    histoDict[hName] = histoDict[hName].Add(his1)
            else : 
                if verbose >= 3 : print(" new histo",his1.name)
                histoDict[hName] = his1
        for his2 in histoFile.fHistos2D :
            if fWeight != 1. :
                his2 = his2.Mult(fWeight)
            hName = his2.name
            if hName in histoDict.keys() :
                if verbose >= 3 : print(" adding histo",his2.name)
                histoDict[hName] = histoDict[hName].Add(his2)
            else : 
                if verbose >= 3 : print(" new histo",his2.name)
                histoDict[hName] = his2

    fOut = open(hfOutName,"w")
#    for key,his in histoDict.items() :
#        print("loop his key",key,his)
    for his in histoDict.values() :
        if verbose >= 3 : print("loop his",his)
        his.Write(fOut)
    fOut.close()
        
###### -------------- main ----------- ######
histoDict = {}
nHistos = {}

theHisFileNames = []
theHisFileWeights = []
bAdd = False # False = get weighted average of two histos, True = sum values and errors squared

### PROCESS PARAMETERS
bParamTypeFound = False
for ii in range(1,len(sys.argv)) :
    if(verbose >= 3) :  print("LOOPING ",ii)
    if sys.argv[ii] == "-fOut" :
        bParamTypeFound = True
        hfOutName = sys.argv[ii+1]
        
    elif sys.argv[ii] == "-f" :
        #           print(" ARG -f")
        bParamTypeFound = True
# check it is last argument
        for ii2 in range(ii+1,len(sys.argv)) :
            if sys.argv[ii2][0] == "-" :
                print("!!! ERROR -f MUST BE LAST ARGUMENT")
                sys.exit()
            else :
                theHisFileNames.append(sys.argv[ii2])
                theHisFileWeights.append(1.)
                ii = ii+1
                if(verbose >= 3) : print( ii,"theHisFileNames ",len(theHisFileNames))
#-            CheckArgumentExists(len(theHisFileNames),ii)
            
    elif sys.argv[ii] == "-fl" :
        bParamTypeFound = True
        print(ii,"fl",len(sys.argv))
        flIn = open(sys.argv[ii+1])
        lines = flIn.readlines()
        for line in lines :
            words = line.rstrip().split()
            theHisFileNames.append(words[0])
            if len(words) >= 2 :
                theHisFileWeights.append(float(words[1]))
            else:
                theHisFileWeights.append(1.)
        ii = ii+1
    elif sys.argv[ii] == "-bAdd" :
        bAdd = bool(int(sys.argv[ii+1]))
        ii = ii+1

if not bParamTypeFound and len(sys.argv) == 3:
    hfOutName = sys.argv[1]
    flIn = open(sys.argv[2])
    lines = flIn.readlines()
    for line in lines :
        words = line.rstrip().split()
        if len(words) == 0 : continue
        theHisFileNames.append(words[0])
#        print("WORD",words)
        if len(words) >= 2 :
            theHisFileWeights.append(float(words[1]))
        else:
            theHisFileWeights.append(1.)
        
addFiles(sys.argv)

sys.exit()
    
