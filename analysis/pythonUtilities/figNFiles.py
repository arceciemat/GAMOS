import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab

from checkPythonVersion import *
from MyHistos import Histo1D
from GetColor import GetColor

###### -------------- main ----------- ######
checkPythonVersion()
verbose = 3
bLogX = False
bLogY = False
bSort = True
fileNames = []
dataNames = []
yMin = sys.float_info.max
yMax = -sys.float_info.max
### Manage arguments 
if len(sys.argv) == 2 :
    fileListName = sys.argv[1]
    file = open(fileListName)
    with open(fileListName, 'r') as file:
        for line in file:
            print("LINE",line) #GDEB
            fileNames.append(line.rstrip())
else :
    if sys.argv[2][:1] == "-" :
        for ii in range(1,len(sys.argv)) :
            if sys.argv[ii] == "-logX" :
                bLogX = int(sys.argv[ii+1])
                ii = ii+1
            elif sys.argv[ii] == "-logY" :
                bLogY = int(sys.argv[ii+1])
                ii = ii+1
            elif sys.argv[ii] == "-f" :
                for jj in range(ii+1,len(sys.argv)) :
                    fileNames.append(sys.argv[jj])
                    ii = ii+1
#    for ii in range(1,len(sys.argv)) :
#        if sys.argv[ii] == "-f" :
#            fileName = sys.argv[ii+1]
#            bParamTypeFound = True
#            ii = ii+1
    for ii in range(1,len(sys.argv)) :
        fileNames.append(sys.argv[ii])

### Read files

for ifn in range(len(fileNames)) :
    file = open(fileNames[ifn])
    bData = 0
    lines = file.readlines()
    if lines[0][0:5] == ":DATA" :
        bData = 1
        words = lines[0].rstrip().split()
        nColumns = len(words)-2
        if len(words) != 2:
            if verbose >= 0 : print("!!! ERROR: first line must contain two words: ':DATA' <X_axis>, it is",lines[0])
            sys.exit()
        dataName = words[1]
    else :
        dataName = "XVALs"
        if verbose >= 1 : print("!!! WARNING: first word of file ",sys.argv[1]," should be ':DATA'; first line is",lines[0])
    dataNames.append(dataName)

    XPos = []
    YPos = []
    YErr = []
    for il in range(bData,len(lines)) :
        # print("LINE",lines[il]) #GDEB    
        words = lines[il].rstrip().split()
        if len(words) == 0:
            continue
        XPos.append(float(words[0]))
        YPos.append(float(words[1]))
        if len(words) >= 3 :
            YErrRel = float(words[2])
            YErr.append(YPos[il]*YErrRel)
        else :
            YErr.append(0.)
            
    #print("READ",XPos,YPos) # GDEB
    NPoints = len(XPos)

    if bSort :
        SortIdx = sorted(range(NPoints), key=lambda k: XPos[k])
        XPosS = []
        YPosS = []
        YErrS = []
        for sid in SortIdx :
            XPosS.append(XPos[sid])
            YPosS.append(YPos[sid])
            YErrS.append(YErr[sid])
            
        XPos = XPosS
        YPos = YPosS
        YErr = YErrS
    
    #plt.xlim(float(his1.xmin),float(his1.xmax))
    plt.xlabel(dataName)
    yMin = min(yMin,min(YPos))
    yMax = max(yMax,max(YPos))

    lcolor = GetColor(ifn)
    if bLogX == 1 : plt.xscale('log')
    if bLogY == 1 : plt.yscale('log')
    plt.errorbar(XPos,YPos, color=lcolor, yerr=YErr)
    # print("PLT PLOT",XPos,YPos,YErr,lcolor) # GDEB
    #    plt.legend()

for ifn in range(len(fileNames)) :

    lcolor = GetColor(ifn)
    xtPos = min(XPos)+(max(XPos)-min(XPos))*0.6
    if verbose >= 3 : print("MAXY ",yMax,"*",(0.7-ifn*0.1)) 
    ytPos = yMin+(yMax-yMin)*(0.9-(ifn*0.1))
    if verbose >= 3 : print(ifn,"YMINMAX",yMin,yMax)
    if verbose >= 3 : print(ifn,"TEXT POS",xtPos,ytPos,yMax)
    if verbose >= 3 : print(ifn,"PLOT",XPos,YPos)
    if verbose >= 3 : print("DATA",ifn,len(dataName[ifn]))
    plt.text(xtPos,ytPos,dataNames[ifn], color=lcolor)
    
    plt.draw()
        
plt.savefig("figNFiles.jpg")

