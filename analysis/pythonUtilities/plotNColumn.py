import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab

from checkPythonVersion import *
from MyHistos import Histo1D

###################################
def line_fit(X, Y):

    xbar = sum(X)/len(X)
    ybar = sum(Y)/len(Y)
    n = len(X) # or len(Y)

    numer = sum([xi*yi for xi,yi in zip(X, Y)]) - n * xbar * ybar
    denum = sum([xi**2 for xi in X]) - n * xbar**2

    slope = numer / denum
    const = ybar - slope * xbar

#    print('line fit: y = {:.2f} + {:.4f} x'.format(const, slope))

    return const, slope


###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
verbose = 0
bFit = False
bSort = True

### Manage arguments 
if len(sys.argv) == 2 :
    fileName = sys.argv[1]
else :
    for ii in range(1,len(sys.argv)) :
        if sys.argv[ii] == "-f" :
            fileName = sys.argv[ii+1]
            bParamTypeFound = True
            ii = ii+1
        elif sys.argv[ii] == "-bFit" :
            bFit = bool(sys.argv[ii+1])
            ii = ii+1
        elif sys.argv[ii] == "-bSort" :
            bSort = bool(sys.argv[ii+1])
            ii = ii+1

### Read file
file = open(fileName)

lines = file.readlines()
if lines[0][0:5] != ":DATA" : 
    print("!!! ERROR: first word of file ",sys.argv[1]," must be ':DATA'; first line is",lines[0])
    sys.exit()
words = lines[0].rstrip().split()
nColumns = len(words)-2
if len(words) < 3:
    print("!!! ERROR: first line must contain at least three words: ':DATA' <X_axis> <Y_axis_1> (... <Y_axis_N>, it is",lines[0])
    sys.exit()
XAxisName = words[1]
YAxisNames = []
for ii in range (2,len(words)):
    YAxisNames.append(words[ii])

XPos = []
YPos = []
for il in range(nColumns) :
    ylist = []
    YPos.append(ylist)
    
for il in range(1,len(lines)) :
    words = lines[il].rstrip().split()
    XPos.append(float(words[0]))
    for iy in range(nColumns) :
        # print(iy,"NCOLUM",nColumns,len(words),words)#GDEB
        YPos[iy].append(float(words[iy+1]))

NPoints = len(XPos)
if bSort :
    SortIdx = sorted(range(NPoints), key=lambda k: XPos[k])
    XPosS = []
    for sid in SortIdx :
        XPosS.append(XPos[sid])
    XPos = XPosS
    for il in range(nColumns) :
        YPosS = []
        for sid in SortIdx :
            YPosS.append(YPos[il][sid])
        YPos[il] = YPosS
    
#plt.xlim(float(his1.xmin),float(his1.xmax))
plt.xlabel(XAxisName)
yMinl = []
yMaxl = []
for il in range(nColumns) :
    yMinl.append(min(YPos[il]))
    yMaxl.append(max(YPos[il]))
yMin = max(yMinl)
yMax = max(yMaxl)

for il in range(nColumns) :
    if il == 0 :
        lcolor = 'black'
    elif il == 1 :
        lcolor = 'red'
    elif il == 2 :
        lcolor = 'blue'
    elif il == 3 :
        lcolor = 'green'
    else :
        lcolor = 'black'
    plt.plot(XPos,YPos[il], color=lcolor)
#    plt.legend()
    xtPos = min(XPos)+(max(XPos)-min(XPos))*0.5
    #    print("MAXY ",max(YPos[il]),"*",(0.7-il*0.1)) # GDEB
#    print(il,"TEXT POS",yMax)
    ytPos = yMin+(yMax-yMin)*(0.3-(il*0.1))
#    print(il,"TEXT POS",xtPos,ytPos,yMax)
#    print(il,"PLOT",XPos,YPos[il])#GDEB
    plt.text(xtPos,ytPos,YAxisNames[il], color=lcolor)
    plt.draw()
    ### Fit to a line
    if bFit == True :
        const,slope = line_fit(XPos, YPos[il])
        print(YAxisNames[il],'SLOPE fit: {:.2f}e-3'.format(slope*1000.))
        hXFit = []
        hYFit = []
        hXFit.append(XPos[0])
        hXFit.append(XPos[NPoints-1])
        hYFit.append(YPos[il][0])
        hYFit.append(YPos[il][NPoints-1])
        plt.plot(hXFit, hYFit, linestyle='dotted',color=lcolor)
        ytPos = yMin+(yMax-yMin)*(0.3-(il*0.1)-0.05)
        print(YAxisNames[il],'SLOPE fit: {:.2f}e-3'.format(slope*1000.))
        plt.text(xtPos,ytPos,'s={:.2f}e-3'.format(slope*1000.))

plt.savefig("histNColumn.jpg")

