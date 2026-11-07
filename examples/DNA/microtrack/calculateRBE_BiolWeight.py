#!/bin/python3
import sys
import os
import numpy as np
from MyHistos import Histo1D, Histo2D
from csvHistoReader import csvHistoReader
import matplotlib.pyplot as plt

verbose = 0

#def Interpolate_logX_linY(xVals, yVals, xQuery):
def InterpolateYBWF( xQuery):
    """
###     Interpolate Y at a given X using logarithmic spacing in X and linear interpolation in Y.

    """
    # Convert to numpy arrays
#    xVals = np.array(xVals, dtype=float)
#    yVals = np.array(yVals, dtype=float)

    if xQuery < theMinBWFX :
        return 0.

    # Work in log-space for X
    logx = theBWFX
    logxQuery = xQuery

    # Find indices of the two nearest points
    idx = np.searchsorted(logx, logxQuery) - 1
    idx = np.clip(idx, 0, len(theBWFX) - 2)

    # Get bounding points
    x1, x2 = logx[idx], logx[idx+1]
    y1, y2 = theBWFY[idx], theBWFY[idx+1]

    # Linear interpolation in Y across log-scaled X
    t = (logxQuery - x1) / (x2 - x1)
    # print(logxQuery,"INTER", y1 + t * (y2 - y1),t,"T",y1,y2,"X",x1,x2) #GDEB
    return y1 + t * (y2 - y1)

######################---------- Main --------------#######################

fHisMC = csvHistoReader(sys.argv[1])
fBWF = "Loncol.lis"
if len(sys.argv) == 3 :
    fBWF = sys.argv[2]

### Read Biological Weighting Function
theBWFX = np.array([])
theBWFY = np.array([])
theMinBWFX = 999.
RBE = 0.
yF = 0.
with open(fBWF) as fin:
    for line in fin :
        wl = line.rstrip().split()
        theBWFX = np.append(theBWFX,float(wl[0]))
        theBWFY = np.append(theBWFY,float(wl[1]))

theBWFX = np.log10(theBWFX)
theMinBWFX = theBWFX[0]

for his in fHisMC.fHistos1D :
    if his.name.startswith("fy"):
        xbins = his.Xbins()
        xstepH = his.Xstep()/2.
        yF = 0.
        RBE = 0.
        for ii in range(0,len(xbins) ):  # integral y f(y)
            yhis1 = xbins[ii] +xstepH # linear energy in log10
            yhis1Lin = np.power(10.,xbins[ii] +xstepH) # linear energy
            fhis1 = his.data[ii] # f(y)
            #print("yhis1",yhis1,"yhis1Lin",yhis1Lin,"fins1",fhis1," xstep",xstepH)
            yfhis1 = yhis1Lin * fhis1   # not normalized yet (= /Int(y*f(y))
            yF += yfhis1
            yBWF = InterpolateYBWF(yhis1)
            RBE1 = yfhis1 * yBWF
            RBE += RBE1
            if verbose >= 3 and yfhis1 != 0. :
                print(ii," log10y=",yhis1,"y=",yhis1Lin,"fy=",fhis1,"  yBWF=",yBWF)
                print("yfhis=",yfhis1,"RBE1",RBE1,"   yF=",yF," RBE=",RBE)
        RBE /= yF

print("@@@ RBE: ", RBE)
