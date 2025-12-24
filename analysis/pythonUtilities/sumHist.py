import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np

from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader

hfOutName = ""
bAverageWithErrors = 0

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
#    foutNew = {}
    hisToAdd = {}
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
            if not hName in hisToAdd :
                hisToAdd[hName] = []
            hisToAdd[hName].append(his1)
            # print("ADDING ",hName,len(hisToAdd[hName])) #GDEB
#        foutNew[ii] = open(hfName.replace(".csv","")+"NEW"+str(ii)+".csv","w")
#    foutNewTest = open("tt.csv","w")
    # Redo histograms if bin size is equal, but histo limits is not
    for hName, hlist in hisToAdd.items() :
        # print("HNAME",hName,"N",len(hlist))#GDEB
        xminAll = -sys.float_info.max
        xmaxAll = sys.float_info.max
        #print(len(hlist),"HNAME",hName,"HLIST",hlist) #GDEB
        bBadHisto = False
        xstepAll = -1
        for hl1 in hlist :
            if xstepAll != -1 and hl1.Xstep() != xstepAll :
                print("!!! ERROR, cannot sum histograms with different bin size ",hl1.name)
                print(xstepAll,hl1.Xstep(),"NBINS", xstepAll != -1," and ",hl1.Xstep() != xstepAll )
                bBadHisto = True
                continue
                #t sys.exit()
            xstepAll = hl1.Xstep()
            xminAll = max(xminAll,hl1.xmin)
            xmaxAll = min(xmaxAll,hl1.xmax)
            #print(hl1.name,"xminAll",xminAll) #GDEB
            #print(hl1.name,"xmaxAll",xmaxAll) #GDEB
        if xstepAll == 0. :
            bBadHisto = True
        #print(xstepAll,"BBADHISTO",bBadHisto,hName) #GDEB
        if bBadHisto :
            continue
        nBinsNew = int((xmaxAll-xminAll)/xstepAll*1.001)
        for il1 in range(len(hlist)) :
            #print("IL1",il1)

            hl1 = hlist[il1]
#        for hl1 in hlist :            
            if hl1.xmin != xminAll or hl1.xmax != xmaxAll :
                if  hl1.xmin < xminAll :
                    hl1.reduceLimitsExact(xminAll,hl1.xmax)
                if hl1.xmax > xmaxAll :
                    hl1.reduceLimitsExact(hl1.xmin,xmaxAll)
                #print(il1,"foutnew",foutNew)  #GDEB
                ###fout = foutNew[il1]
                ###hl1.Write(fout)
                iimin = hl1.GetIdInterpolatedValue(xminAll+xstepAll*0.001)
                #print(hl1.name,"iimin",iimin,hl1.xmin,xminAll) #GDEB
                iimax = hl1.GetIdInterpolatedValue(xmaxAll-xstepAll*0.001)
                #print(hl1.name,"iimax",iimax,hl1.xmax,xmaxAll) #GDEB
                nbinNew = iimax-iimin+1
                #print(hl1.name,hl1.nbin,hl1.xmin,hl1.xmax,hl1.Xstep(),"NBINNEW",nbinNew,xminAll,xmaxAll,iimin,iimax) #GDEB
                hl1NewData = np.zeros(nbinNew)
                xbins = hl1.Xbins()
                for ii in range(nbinNew) :
                    hl1NewData[ii] = hl1.data[ii+iimin]
                hl1.xmin = xminAll
                hl1.xmax = xmaxAll
                hl1.data = hl1NewData
        
    for hName, hlist in  hisToAdd.items() :
        if verbose >= 3 : print("LOOP HISTO= ",hName)
        dataNew = []
        if bAdd :
            factAdd = 1
        else : 
            factAdd = len(hlist)
        xbins = hlist[0].Xbins()
        # print(bAdd,"factAdd",factAdd) # GDEB
        dataNew = np.zeros(len(xbins))  
        for ii in range(len(xbins)) :
            for hl1 in hlist :
#                if ii == 150 : print(hl1.name,"NBIN",hl1.nbin)
#                print(hName,ii,"ADD PREV",xbins[ii],dataNew[ii],hl1.data[ii]) #GDEB
                dataNew[ii] += hl1.data[ii]/factAdd
#                if hl1.nbin == 201 and ii == 150 :
 #                   xbins = hl1.Xbins()
  #                  print(hl1.name,"hl1",xbins[ii],hl1.data[ii],":",hl1.data[ii]/factAdd,"->",dataNew[ii])
                #print(hName,ii,"ADD",xbins[ii],dataNew[ii],hl1.data[ii]) #GDEB
                #   if ii == 140 : print(hName,xbins[ii],"ADD",dataNew[140],hl1.data[140]) #GDEB
        histoDict[hName] = hlist[0]
        histoDict[hName].data = dataNew
    
#            if hName in histoDict.keys() :
#                if verbose >= 3 : print(" adding histo",his1.name)
#                if not bAdd :
#                    hisnew = histoDict[hName].Average(his1,True,bAverageWithErrors)
#                    xbins = hisnew.Xbins()
#                    for ii in range(len(histoDict[hName].data)) : # GDEB
#                        print(hName,ii,"AVERAGE",xbins[ii],his1.data[ii],"+",hisnew.data[ii])
#                    histoDict[hName] = hisnew
#                    for ii in range(len(histoDict[hName].data)) :
#                        print(hName,ii,"SUMMED AVERAGE",xbins[ii],histoDict[hName].data[ii],"+",hisnew.data[ii])
#                else :
#                    histoDict[hName] = histoDict[hName].Add(his1)
#            else : 
#                if verbose >= 3 : print(" new histo",his1.name)
#                histoDict[hName] = his1
#            for ii in range(his1.nbin) :
#                if histoDict[hName].data[ii] >1. :  print(ii,hName, histoDict[hName].data[ii],his1.data[ii])
#        for his2 in histoFile.fHistos2D :
#            if fWeight != 1. :
#                his2 = his2.Mult(fWeight)
#            hName = his2.name
#            if hName in histoDict.keys() :
#                if verbose >= 3 : print(" adding histo",his2.name)
#                histoDict[hName] = histoDict[hName].Add(his2)
#            else : 
#                if verbose >= 3 : print(" new histo",his2.name)
#                histoDict[hName] = his2

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
    elif sys.argv[ii] == "-bErr" :
        bAverageWithErrors = bool(int(sys.argv[ii+1]))

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
    
