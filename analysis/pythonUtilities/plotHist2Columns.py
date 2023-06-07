import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

#tfrom checkPythonVersion import *

bPlotErrors = 1

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
#checkPythonVersion()

bPlotErrors = 0
nargs = len(sys.argv)
if len(sys.argv) > 2 :
    bPlotErrors = int(sys.argv[nargs-2])
print("N ARGS= ",len(sys.argv),bPlotErrors)
outName = ""
if len(sys.argv) > 3 :
    outName = sys.argv[nargs-1]

for iarg in range(1,nargs-2):
    fileName = sys.argv[iarg]
    print("READING FILE ",fileName)
    file = open(fileName)
    lines = file.readlines()
    hX = []
    hY = []
    hYerr = []
    theTitle = ""
    theXLabel = ""
    theYLabel = ""
    for line in lines :
        words=line.rstrip().split()
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
            hX.append(float(words[0])*yfact)
            hY.append(float(words[1]))
            if bPlotErrors :
                hYerr.append(float(words[2]))
        print("hx= ",hX," hy=",hY)
    #    xStep = his1.Xstep(his1)
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
        
plt.savefig("hist."+outName+".jpg")
#for ii in range(0,len(hX)) :
#    print("hXY: ",hX[ii],hY[ii])
#print("hY: ",hY)
plt.clf()

sys.exit()
    
