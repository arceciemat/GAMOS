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

### Manage arguments 
fileName=""
icolumn = 0
if len(sys.argv) == 2 :
    fileName = sys.argv[1]
else :
    for ii in range(1,len(sys.argv)) :
        if sys.argv[ii] == "-f" :
            fileName = sys.argv[ii+1]
            bParamTypeFound = True
            ii = ii+1
        elif sys.argv[ii] == "-col" :
            icolumn = int(sys.argv[ii+1])
            bParamTypeFound = True
            ii = ii+1

### Read file
file = open(fileName)

lines = file.readlines()
if lines[0][0:5] == ":DATA" : 
    words = lines[0].rstrip().split()
    if len(words) < 2:
        print("!!! ERROR: first line must contain at least two words: ':DATA' <X_axis>, it is",lines[0])
        sys.exit()
    XAxisName = words[1]
    line1st = 1
else :
    XAxisName = "data"
    line1st = 0
    
XPos = []
XPosNP = np.array([])
mean=0
mean2=0
nData=len(lines)
for il in range(line1st,nData):
    words = lines[il].rstrip().split()
    if len(words) == 0 : continue
    if il%10000 == 0:
        print(il,icolumn,"Reading line",words)
    print(il,icolumn,"Reading line",words[icolumn],words)        
    val = float(words[icolumn])
    XPos.append(val)
    XPosNP = np.append(XPosNP,val)
    mean += val
    mean2 += val*val
    # print(val,"mean1",mean,mean)#GDEB
RMS = np.sqrt( (mean2*nData - mean*mean)/(nData-1))
mean /= nData
print("RMS",RMS,mean2*nData,"-",mean*mean,"/",nData-1)
# Calculate the mean
mean_value = sum(XPos) / len(XPos)
# Calculate the sum of squared differences from the mean
sum_squared_diff = sum((x - mean_value) ** 2 for x in XPos)
# Calculate the mean of the squared differences
mean_squared_diff = sum_squared_diff / len(XPos)
# Calculate the square root of the mean of squared differences
rms_value = mean_squared_diff ** 0.5
print("RMS_VALUE",rms_value)    

hist_values, bin_edges, _ = plt.hist(XPosNP, bins=50)
xmin = bin_edges[0]
xmax = bin_edges[50]
ymax = max(hist_values)
#xmin,xmax = plt.xlim()
xdiff=xmax-xmin
#yy, xx, _ = plt.hist(hdata)
#ymax = yy.max()
npmean=XPosNP.mean()
npsem=np.std(XPosNP)/nData
print("NPmean",XPosNP.mean(),"+-",np.std(XPosNP)/nData,"rmsP=",np.sqrt(np.mean(XPosNP**2)),"rmsD=",RMS)

plt.xlabel(XAxisName)
plt.draw()
print(bin_edges,"XLIM",xmin,xmax,xdiff,ymax)
#print("N",len(bin_edges),nData)
t2show = "N=  "+str(nData)
plt.text(xmax-0.25*xdiff,ymax*0.98,t2show)
t2show = "mean=  "+"{:.3g}".format(npmean)
plt.text(xmax-0.25*xdiff,ymax*0.94,t2show)
t2show = "SEM=   "+"{:.3g}".format(npsem)
plt.text(xmax-0.25*xdiff,ymax*0.9,t2show)
t2show = "RMS   "+"{:.3g}".format(rms_value)
plt.text(xmax-0.25*xdiff,ymax*0.86,t2show)

plt.savefig("hist1Column.jpg")

#print("MEAN= ",mean,"RMS= ",RMS)
print("MEAN= ",npmean,"RMS=",rms_value,"SEM= ",npsem)

errorNeg,errorPos = GetLeftRightErrors(XPosNP,1.)
print("MEAN= ",npmean,"REL+",'{:.4g}'.format(errorPos/mean),"REL-",'{:.4g}'.format(errorNeg/mean))
