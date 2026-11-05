import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab
import numpy as np
from scipy.stats import norm

from checkPythonVersion import *
from MyHistos import Histo1D
import runpy

import inspect

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
def GetLeftRightErrors(data,sigmas) :
    fInsideSigmas = (norm.cdf(sigmas) - norm.cdf(-sigmas)) # percentage inside number of sigmas
    mean = data.mean()
    # count number of data below and above 
    dataBelow = np.sort(data[data < mean]) 
    dataAbove = np.sort(data[data > mean])
    if verbose >= 3: print("dataBelow",dataBelow)
    if verbose >= 3: print("dataAbove",dataAbove)
    # BELOW
    nInsideB = int(fInsideSigmas*len(dataBelow)) # number of data below inside sigmas
    dataInsideB = dataBelow[-nInsideB:]  # list of below data inside
    if verbose >= 2: print("nInsideB",nInsideB,len(dataInsideB),"dataInsideB",dataInsideB)
    if len(dataInsideB) == 0 :
        dataInsideB = np.append(dataInsideB,dataBelow[0]) 
        if verbose >= 1: print("ADDED nInsideB",nInsideB,len(dataInsideB),"dataInsideB",dataInsideB)
    minBelowT = dataInsideB[0]  # first value inside (lowest value)
    if verbose >= 2: print("fInsideSigmas",fInsideSigmas,"nInsideB",nInsideB,len(dataBelow),"minBelowT",minBelowT)
    # now extrapolate between first and the one before
    fracInsideB = fInsideSigmas*len(dataBelow)-nInsideB # rest of integer nInsideB
    if verbose >= 2: print("fracInsideB",fracInsideB,"=",nInsideB,"-",fInsideSigmas*len(dataBelow))
    minBelow = minBelowT - (minBelowT-dataBelow[len(dataBelow)-nInsideB-1])*fracInsideB # extrapolate
    if verbose >= 2: print("FINAL minBelow",minBelow,len(dataBelow),"-",nInsideB,len(dataBelow)-nInsideB-1,"*",dataBelow[len(dataBelow)-nInsideB-1],"-",minBelowT,")*",fracInsideB)
    #ABOVE
    nInsideA = int(fInsideSigmas*len(dataAbove)) # number of data above inside sigmas
#    print("TTT",nInsideA,"=", int(fInsideSigmas*len(dataAbove)),fInsideSigmas*len(dataAbove),fInsideSigmas,"*",len(dataAbove)) #GDEB
    dataInsideA = dataAbove[:nInsideA]  # list of above data inside
    if verbose >= 2: print("nInsideA",nInsideA,len(dataInsideA),"dataInsideA",dataInsideA)
    if len(dataInsideA) == 0 :
        dataInsideA = np.append(dataInsideA,dataAbove[0]) 
        if verbose >= 1: print("ADDED nInsideA",nInsideA,len(dataInsideA),"dataInsideA",dataInsideA)
    maxAboveT = dataInsideA[-1]  # first value inside (lowest value)
    if verbose >= 2: print("fInsideSigmas",fInsideSigmas,"nInsideA",nInsideA,len(dataAbove),"maxAboveT",maxAboveT)
    # now extrapolate between first and the one before
    fracInsideA = fInsideSigmas*len(dataAbove)-nInsideA # rest of integer nInsideA
    if verbose >= 2: print("fracInsideA",fracInsideA,"=",nInsideA,"-",fInsideSigmas*len(dataAbove))
    maxAbove = maxAboveT + (dataAbove[0+nInsideA]-maxAboveT)*fracInsideA # extrapolate
    if verbose >= 2: print("FINAL maxAbove",maxAbove,len(dataAbove),"+",0+nInsideA,"*",dataAbove[0+nInsideA],"-",maxAboveT,")*",fracInsideA)
    
    if verbose >= 1: print("@@@ GetLeftRightErrors -",mean-minBelow,"+",maxAbove-mean)
    return (mean-minBelow),(maxAbove-mean)


###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
verbose = 0
bLogY = False

### Manage arguments 
fileName=""
fileOutName = "hist1Column"
icolumn = 0
minX = sys.float_info.min
maxX = sys.float_info.max
nStepX = 100
fdpi=100

if len(sys.argv) == 2 :
    fileName = sys.argv[1]
else :
    ii = 1
    while ii < len(sys.argv):
        if sys.argv[ii] == "-f" :
            fileName = sys.argv[ii+1]
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-fOut" :
            fileOutName = sys.argv[ii+1]
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-col" :
            icolumn = int(sys.argv[ii+1])
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-minX" :
            minX = float(sys.argv[ii+1])
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-maxX" :
            maxX = float(sys.argv[ii+1])
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-nStepX" :
            nStepX = float(sys.argv[ii+1])
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-logY" :
            bLogY = bool(int(int(sys.argv[ii+1])))
            bParamTypeFound = True
            ii += 2
        elif sys.argv[ii] == "-dpi" :
            fdpi  = float(sys.argv[ii+1])
            ii += 2
        else :
            print("!!! ERROR: PARAMETER NOT FOUND ",sys.argv[ii])
            sys.exit()

if nStepX == sys.maxsize :
    nStepX = 100
#sys.argv = ["getMeanRMS.py", fileName]
#result = runpy.run_path("getMeanRMS.py", run_name="__main__")

### Read file
with open(fileName) as f:
    firstLine = f.readline().strip()
if firstLine.startswith(":DATA"):
    words = firstLine.split()
    if len(words) < 2:
        print("!!! ERROR: first line must contain at least two words: ':DATA' <X_axis>, it is",lines[0])
        sys.exit()
    XAxisName = words[1] if len(words) > 1 else "data"
    line1st = 1
else:
    XAxisName = "data"
    line1st = 0
    
XPos = np.loadtxt(fileName, usecols=icolumn, comments=(':DATA','#'))
nVals = XPos.size
mean = XPos.mean()
std = XPos.std(ddof=1) if nVals > 1 else 0.   # sample standard deviation
sem = std / np.sqrt(nVals)                 # standard error of the mean
rms = np.sqrt(np.mean(XPos**2))           # root mean square about zero
print("MEAN=","{:.3f}".format(mean), "STD=", "{:.3f}".format(std), "RMS=", "{:.3f}".format(rms), "SEM=", "{:.3f}".format(sem), "N=", nVals)

if minX == sys.float_info.min or maxX == sys.float_info.max or nStepX == sys.maxsize :
    hist_XPos, bin_edges, _ = plt.hist(XPos, bins=nStepX)
    minX = bin_edges[0]
    maxX = bin_edges[len(bin_edges)-1]
else :
    hist_XPos, bin_edges, _ = plt.hist(XPos, bins=int(nStepX), range=(minX,maxX))
if bLogY : 
    plt.yscale('log')

his1 = Histo1D()
his1.name = fileOutName
print(inspect.signature(his1.FillFrom1List))
#print("NSTEPX",nStepX)
stepX = (maxX-minX)/nStepX
minX -= stepX/2.
maxX += stepX/2.
his1.FillFrom1List(XPos,nVals,nStepX,minX,maxX)

xmin = bin_edges[0]
xmax = bin_edges[nStepX]
ymax = max(hist_XPos)
#xmin,xmax = plt.xlim()
xdiff=xmax-xmin
#yy, xx, _ = plt.hist(hdata)
#ymax = yy.max()

plt.xlabel(XAxisName)
plt.draw()
#print(bin_edges,"XLIM",xmin,xmax,xdiff,ymax) #GDEB
#print("N",len(bin_edges),nVals)
t2show = "N=  "+str(nVals)
plt.text(xmax-0.25*xdiff,ymax*0.98,t2show)
t2show = "mean=  "+"{:.3g}".format(mean)
plt.text(xmax-0.25*xdiff,ymax*0.94,t2show)
t2show = "SEM=   "+"{:.3g}".format(sem)
plt.text(xmax-0.25*xdiff,ymax*0.9,t2show)
t2show = "RMS   "+"{:.3g}".format(rms)
plt.text(xmax-0.25*xdiff,ymax*0.86,t2show)

plt.savefig(fileOutName+".jpg",dpi=fdpi)

errorNeg,errorPos = GetLeftRightErrors(XPos,1.)
if mean != 0. :
    print("MEAN= ",mean,"REL+",'{:.4g}'.format(errorPos/mean),"REL-",'{:.4g}'.format(errorNeg/mean))
else :
    print("MEAN= ",mean,"+",'{:.4g}'.format(errorPos),"-",'{:.4g}'.format(errorNeg))

fhis = open(fileOutName+".csv",'w')
his1.Write(fhis)

