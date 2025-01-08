import sys
#python ../pythonUtilities/plotHist2Columns.py enerAfterNozzle.070.2.lis 0 test.csv

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
from MyHistos import Histo1D, Histo2D

#tfrom checkPythonVersion import *

bPlotErrors = 1

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
#checkPythonVersion()

bPlotErrors = 0
nargs = len(sys.argv)
if len(sys.argv) > 2 :
    print("ARGS= ",nargs-1,sys.argv[2])
    bPlotErrors = int(sys.argv[2])
print("N ARGS= ",nargs,bPlotErrors)
outName = "his"
if len(sys.argv) > 3 :
    outName = sys.argv[3]
print("OUTNAME",outName)

theYTitle = "val"
theXLabel = "X"
theYLabel = "Y"
hOut = open(outName+".csv",'w')
#for iarg in range(1,nargs):
for iarg in range(1,2):
    print("IARG",iarg)
    fileName = sys.argv[iarg]
    print("READING FILE ",fileName)
    file = open(fileName)
    args = []
    his1 = Histo1D(args)
    his1.name = "his" + theYTitle
    his1.under = 0.
    his1.underErr = 0.
    his1.over = 0.
    his1.overErr = 0.
    hisMaxX = -sys.float_info.max
    hisMinX = sys.float_info.max
    lines = file.readlines()
    nBins = len(lines)
    hX = []
    hY = []
    hYerr = []
    theTitle = ""
    theXLabel = ""
    theYLabel = ""
    print("NLINE ",len(lines)) # GDEB
    for line in lines :        
        words=line.rstrip().split()
        if words[0] == ":DATA" or words[0] == "DATA:" :
            nBins -= 1
            continue
        if len(words) == 1 or len(words) > 2+bPlotErrors :
            print("!!! ERROR each line in file must have ",2+bPlotErrors," columns\n LINE: ",line)
            sys.exit()
        if words[0] == ":XTIT" :
            theXLabel = words[1]
        elif words[0] == ":YTIT" :
            theYTitle = words[1]
        elif words[0] == ":TIT" :
            theTitle = words[1]
        else :
            yfact = 1
            valX = float(words[0])*yfact            
            hX.append(valX)
            hY.append(float(words[1]))
            hisMaxX = max(hisMaxX,valX)
            hisMinX = min(hisMinX,valX)
            #print("VALS",valX, words[1])
            if bPlotErrors :
                hYerr.append(float(words[2]))
            else :
                hYerr.append(0.)
#    print("hx= ",hX," hy=",hY)
    #    xStep = his1.Xstep()
    #    plt.xlim(float(his1.xmin),float(his1.xmax))
    #    plt.xlabel(his1.name)
    if bPlotErrors == 1 :
    #        plt.bar(hX,hY, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep)
        plt.bar(hX,hY, color='None', edgecolor='black', yerr=hYerr)
    elif bPlotErrors == 0 :
        plt.plot(hX,hY) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
    else :
        print("!!! ERROR: second argument can only be 0 or 1, it is:",bPlotErrors)
        sys.exit()
    plt.xlabel(theXLabel)
    plt.ylabel(theYTitle)
    plt.title(theTitle)
    print("SAVING ",outName+".jpg")
    plt.savefig(outName+".jpg")
    his1.nbin = nBins
    his1.nent = nBins
    step = (hisMaxX-hisMinX)/(nBins-1)
    his1.xmin = hisMinX-step/2.
    his1.xmax = hisMaxX+step/2.
    his1.data = hY
    his1.dataErr = hYerr
    print("NBINS ",nBins,"=",len(his1.data))
    his1.CalculateMeanAndRMS()
    his1.Write(hOut)
#for ii in range(0,len(hX)) :
#    print("hXY: ",hX[ii],hY[ii])
#print("hY: ",hY)
plt.clf()


sys.exit()
    
