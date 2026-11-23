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
    his1ToAdd = {}
    his2ToAdd = {}
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
            if not hName in his1ToAdd :
                his1ToAdd[hName] = []
            his1ToAdd[hName].append(his1)
            # print("ADDING ",hName,len(his1ToAdd[hName])) #GDEB
        for his2 in histoFile.fHistos2D :
            if fWeight != 1. :
                his2 = his2.Mult(fWeight)
            hName = his2.name
            if not hName in his2ToAdd :
                his2ToAdd[hName] = []
            his2ToAdd[hName].append(his2)
            # print("ADDING ",hName,len(his2ToAdd[hName])) #GDEB

    # Redo 1D histograms if bin size is equal, but histo limits are not
    for hName, hlist in his1ToAdd.items() :
        #print("HNAME",hName,"N",len(hlist))#GDEB
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
            xstepAll = hl1.Xstep()
            xminAll = max(xminAll,hl1.xmin)
            xmaxAll = min(xmaxAll,hl1.xmax)
            #print(hl1.name,"xminAll",xminAll,"xmaxAll",xmaxAll) #GDEB
        if xstepAll == 0. :
            bBadHisto = True
        #print(xstepAll,"BBADHISTO",bBadHisto,hName) #GDEB
        if bBadHisto :
            continue
        nBinsNew = int((xmaxAll-xminAll)/xstepAll*1.001)
        for il1 in range(len(hlist)) :
            #print("IL1",il1)
            hl1 = hlist[il1]
            if hl1.xmin != xminAll or hl1.xmax != xmaxAll :
                if  hl1.xmin < xminAll :
                    hl1.reduceLimitsExact(xminAll,hl1.xmax)
                if hl1.xmax > xmaxAll :
                    hl1.reduceLimitsExact(hl1.xmin,xmaxAll)
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

    # Redo 2D histograms if bin size is equal, but histo limits are not
    for hName, hlist in his2ToAdd.items() :
        #    print("HNAME",hName,"N",len(hlist))#GDEB
        xminAll = -sys.float_info.max
        xmaxAll = sys.float_info.max
        yminAll = -sys.float_info.max
        ymaxAll = sys.float_info.max
        #print(len(hlist),"HNAME",hName,"HLIST",hlist) #GDEB
        bBadHisto = False
        xstepAll = -1
        ystepAll = -1
        for hl2 in hlist :
            if xstepAll != -1 and hl2.Xstep() != xstepAll :
                print("!!! ERROR, cannot sum histograms with different bin size ",hl2.name)
                print(xstepAll,hl2.Xstep(),"NBINS", xstepAll != -1," and ",hl2.Xstep() != xstepAll )
                bBadHisto = True
                continue
            xstepAll = hl2.Xstep()
            xminAll = max(xminAll,hl2.xmin)
            xmaxAll = min(xmaxAll,hl2.xmax)
            #print(hl2.name,"xminAll",xminAll,"xmaxAll",xmaxAll) #GDEB
            ystepAll = hl2.Ystep()
            yminAll = max(yminAll,hl2.ymin)
            ymaxAll = min(ymaxAll,hl2.ymax)
            #print(hl2.name,"yminAll",yminAll,"ymaxAll",ymaxAll) #GDEB
        if ystepAll == 0. :
            bBadHisto = True
        #print(ystepAll,"BBADHISTO",bBadHisto,hName) #GDEB
            if ystepAll != -1 and hl2.Ystep() != ystepAll :
                print("!!! ERROR, cannot sum histograms with different bin size ",hl2.name)
                print(ystepAll,hl2.Ystep(),"NBINS", ystepAll != -1," and ",hl2.Ystep() != ystepAll )
                bBadHisto = True
                continue
            ystepAll = hl2.Ystep()
            yminAll = max(yminAll,hl2.ymin)
            ymaxAll = min(ymaxAll,hl2.ymax)
            print(hl2.name,"yminAll",yminAll,"ymayAll",ymaxAll) #GDEB
        if ystepAll == 0. :
            bBadHisto = True
        #print(ystepAll,"BBADHISTO",bBadHisto,hName) #GDEB
        if bBadHisto :
            continue

        nBinsNew = int((xmaxAll-xminAll)/xstepAll*1.001)
        for il2 in range(len(hlist)) :
            #print("IL2",il2)
            hl2 = hlist[il2]
            #print(il2,hl2.name,"X check limits",hl2.xmin,xminAll,hl2.xmax,xmaxAll)  #GDEB
            if hl2.xmin != xminAll or hl2.xmax != xmaxAll :
                if  hl2.xmin < xminAll :
                    hl2.reduceXLimitsExact(xminAll,hl2.xmax)
                if hl2.xmax > xmaxAll :
                    hl2.reduceXLimitsExact(hl2.xmin,xmaxAll)
                ixmin = hl2.GetIdInterpolatedValue(xminAll+xstepAll*0.001,hl2.ymin)[0]
                #print(hl2.name,"ixmin",ixmin,hl2.xmin,xminAll) #GDEB
                ixmax = hl2.GetIdInterpolatedValue(xmaxAll-xstepAll*0.001,hl2.ymin)[0]
                #print(hl2.name,"ixmax",ixmax,hl2.xmax,xmaxAll) #GDEB
                nXbinNew = ixmax-ixmin+1
                #print(hl2.name,hl2.xnbin,hl2.xmin,hl2.xmax,hl2.Xstep(),"XNBINNEW",xnbinNew,xminAll,xmaxAll,ixmin,ixmax) #GDEB
                hl2NewData = np.zeros((hl2.ynbin,xnbinNew))
                xbins = hl2.Xbins()
                for ix in range(xnbinNew) :
                    for iy in range(hl2.ynbin) :
                        hl2NewData[iy,ix] = hl2.dataNP[iy,ix+ixmin]
                hl2.xmin = xminAll
                hl2.xmax = xmaxAll
                hl2.dataNP = hl2NewData
        #    print(il2,hl2.name,"Y check limits",hl2.ymin,yminAll,hl2.ymax,ymaxAll)  #GDEB
            if hl2.ymin != yminAll or hl2.ymax != ymaxAll :
                if  hl2.ymin < yminAll :
                    hl2.reduceYLimitsExact(yminAll,hl2.ymax)
                if hl2.ymax > ymaxAll :
                    hl2.reduceYLimitsExact(hl2.ymin,ymaxAll)
                iymin = hl2.GetIdInterpolatedValue(hl2.xmax,yminAll+ystepAll*0.001)[1]
                #print(hl2.name,"iymin",iymin,hl2.ymin,yminAll) #GDEB
                iymax = hl2.GetIdInterpolatedValue(hl2.xmax,ymaxAll-ystepAll*0.001)[1]
                #print(hl2.name,"iymax",iymax,hl2.ymax,ymaxAll) #GDEB
                nYbinNew = iymax-iymin+1
                #print(hl2.name,hl2.ynbin,hl2.ymin,hl2.ymax,hl2.Ystep(),"YNBINNEW",ynbinNew,yminAll,ymayAll,iymin,iymax) #GDEB
                hl2NewData = np.zeros((ynbinNew,hl2.xnbin))
                ybins = hl2.Ybins()
                for iy in range(ynbinNew) :
                    for ix in range(hl2.xnbin) :
                        hl2NewData[iy,ix] = hl2.dataNP[iy+iymin,ix]
                hl2.ymin = yminAll
                hl2.ymax = ymaxAll
                hl2.dataNP = hl2NewData
                
    for hName, hlist in his1ToAdd.items() :
        if verbose >= 3 : print("LOOP HISTO= ",hName)
        dataNew = []
        dataErrNew = []
        if bAdd :
            factAdd = 1
        else : 
            factAdd = len(hlist)
        xbins = hlist[0].Xbins()
        # print(bAdd,"factAdd",factAdd) # GDEB
        dataNew = np.zeros(len(xbins))  
        dataErrNew = np.zeros(len(xbins))  
        for ii in range(len(xbins)) :
            for hl1 in hlist :
                #    print(hName,ii,"ADD PREV",xbins[ii],dataNew[ii],hl1.data[ii]) #GDEB
                data1 = hl1.data[ii]/factAdd
                dataErr1 = hl1.dataErr[ii]/factAdd
                if not bAverageWithErrors :
                    dataNew[ii] += data1
                    dataErrNew[ii] = np.sqrt(dataErrNew[ii]**2+dataErr1**2)
                else :
                    if dataErrNew[ii] != 0. :
                        wOld = 1./dataErrNew[ii]**2
                    else :
                        wOld = 0.
                    if dataErr1 != 0 :                            
                        wNew = 1./dataErr1**2
                    else :
                        wNew = 0.
                    if wOld + wNew != 0. :
                        dataNew[ii] = (dataNew[ii]*wOld+data1*wNew)/(wOld+wNew)
                        dataErrNew[ii] = np.sqrt(1./(wOld+wNew))
                    else :
                        dataNew[ii] = 0.
                        dataErrNew[ii] = 0.
                #print(hName,ii,"ADD",xbins[ii],dataNew[ii],hl1.data[ii]) #GDEB
        histoDict[hName] = hlist[0]
        histoDict[hName].data = dataNew

    for hName, hlist in  his2ToAdd.items() :
        if verbose >= 3 : print("LOOP HISTO= ",hName)
        dataNew = []
        dataErrNew = []
        if bAdd :
            factAdd = 1
        else : 
            factAdd = len(hlist)
        xbins = hlist[0].Xbins()
        ybins = hlist[0].Ybins()
        # print(bAdd,"factAdd",factAdd) # GDEB
        dataNew = np.zeros((len(ybins),len(xbins)))
        dataErrNew = np.zeros((len(ybins),len(xbins)))
        for ix in range(len(xbins)) :
            for iy in range(len(ybins)) :
                for hl2 in hlist :
                    data1 = hl2.dataNP[iy,ix]/factAdd
                    dataErr1 = hl2.dataErrNP[iy,ix]/factAdd
                    if not bAverageWithErrors :
                        dataNew[iy,ix] += data1
                        dataErrNew[iy,ix] = np.sqrt(dataErrNew[iy,ix]**2+dataErr1**2)
                    else :
                        if dataErrNew[iy,ix] != 0. :
                            wOld = 1./dataErrNew[iy,ix]**2
                        else :
                            wOld = 0.
                        if dataErr1 != 0. :
                            wNew = 1./dataErr1**2
                        else :
                            wNew = 0.
                        if wOld + wNew != 0. :
                            dataNew[iy,ix] = (dataNew[iy,ix]*wOld+data1*wNew)/(wOld+wNew)
                            dataErrNew[iy,ix] = np.sqrt(1./(wOld+wNew))
                        else :
                            dataNew[iy,ix] = 0.
                            dataErrNew[iy,ix] = 0.
                            

                    #     print(hName,iy,ix,"ADD PREV",xbins[iy,ix],dataNew[iy,ix],hl2.dataNP[iy,ix]) #GDEB
                #print(hName,ii,"ADD",xbins[ii],dataNew[ii],hl2.dataNP[ii]) #GDEB
        histoDict[hName] = hlist[0]
        histoDict[hName].dataNP = dataNew
    
    fOut = open(hfOutName,"w")
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
    
