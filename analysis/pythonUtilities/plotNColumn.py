import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab
import re

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

    if verbose >= 3 : print('line fit: y = {:.2f} + {:.4f} x'.format(const, slope))

    return const, slope

###################################
def split_line_with_quotes(line):
    # Regular expression to match words within quotes and words outside quotes
    pattern = re.compile(r'"([^"]+)"|(\S+)')

    # Find all matches in the line
    matches = pattern.findall(line)

    # Extract words from matches, handling quotes
    words = [match[0] or match[1] for match in matches]

    return words

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
verbose = 1
bFit = False
bSort = True
bError = False

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
        elif sys.argv[ii] == "-bErr" :
            bError = bool(sys.argv[ii+1])
            ii = ii+1

### Read file
file = open(fileName)

bData = 0
lines = file.readlines()
if lines[0][0:5] == ":DATA" :
    bData = 1
    words = split_line_with_quotes(lines[0])
    nColumns = len(words)-1
    if bError :
        nColumns = int(nColumns/2)
    print("NCOLUMNS",nColumns,"WORDS ",words) #GDEB
    if len(words) < 3:
        if verbose >= 0 : print("!!! ERROR: first line must contain at least three words: ':DATA' <X_axis> <Y_axis_1> (... <Y_axis_N>, it is",lines[0])
        sys.exit()
    XAxisName = words[1]
    YAxisNames = []
    for ii in range (2,len(words)):
        YAxisNames.append(words[ii])
else :
    if verbose >= 1 : print("!!! WARNING: first word of file ",sys.argv[1]," should be ':DATA'; first line is",lines[0])
    #    sys.exit()
    words = lines[1].rstrip().split()
    nColumns = len(words)-1
    if bError :
        nColumns = int(nColumns/2)
#        print("NCOLUMS",nColumns) #GDEB
    if len(words) < 2:
        if verbose >= 0 : print("!!! ERROR: first line must contain at least two words: <X_axis> <Y_axis_1> (... <Y_axis_N>, it is",lines[1])
        sys.exit()
    XAxisName = "XVALs"
    YAxisNames = []
    for ii in range (1,len(words)):
        YAxisNames.append("YVAL"+str(ii))
        
XPos = []
YPos = []
YPosErr = []
for il in range(nColumns) :
    ylist = []
    YPos.append(ylist)
    ylistErr = []
    YPosErr.append(ylistErr)
    
for il in range(bData,len(lines)) :
    words = lines[il].rstrip().split()
    XPos.append(float(words[0]))
    if bError == False :
        for iy in range(nColumns) :
            if verbose >= 3 : print(iy,"NCOLUM",nColumns,len(words),words)
            YPos[iy].append(float(words[iy+1]))
#            print(il,iy,"NO ERR YPOS",float(words[iy+1]),len(YPos[iy]))#GDEB
    else:
        for iy in range(nColumns) :
            # if verbose >= 3 : print(iy,"NCOLUM",nColumns,len(words),words)
            #print("Y id ",int(iy/2),iy,len(words))  #GDEB
            YPos[iy].append(float(words[2*iy+1]))
            YPosErr[iy].append(float(words[2*iy+2]))
#            print(il,iy,"YPOS",float(words[2*iy+1]),"+-",float(words[2*iy+2]),len(YPos[iy]),len(YPosErr[iy]))#GDEB

#print("BEFORE ORDER",YPos[0]) #GDEB
NPoints = len(XPos)
if bSort :
    SortIdx = sorted(range(NPoints), key=lambda k: XPos[k])
    XPosS = []
    for sid in SortIdx :
        XPosS.append(XPos[sid])
    XPos = XPosS
    
    for il in range(nColumns) :
        YPosS = []
        YPosErrS = []
        for sid in SortIdx :
            # print("YPOSS",il,sid,len(YPos),len(YPos[il])) #GDEB
            YPosS.append(YPos[il][sid])
            if bError : 
                YPosErrS.append(YPosErr[il][sid])
        YPos[il] = YPosS
        if bError : 
            YPosErr[il] = YPosErrS
    
#print("AFTER ORDER",YPos[0]) #GDEB

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
    if bError == False :
        plt.plot(XPos,YPos[il], color=lcolor)
    else :
        plt.errorbar(XPos,YPos[il], color=lcolor, yerr=YPosErr[il],fmt="-o")
#    print(il,"PLT.PLOT",XPos,YPos[il])#GDEB
#    plt.legend()
    xtPos = min(XPos)+(max(XPos)-min(XPos))*0.9
    if verbose >= 3 : print("MAXY ",max(YPos[il]),"*",(0.7-il*0.1)) 
    if verbose >= 3 : print(il,"TEXT POS",yMax)
    ytPos = yMin+(yMax-yMin)*(0.3-(il*0.1))
    if verbose >= 3 : print(il,"TEXT POS",xtPos,ytPos,yMax)
    if verbose >= 3 : print(il,"PLOT",XPos,YPos[il])
    if verbose >= 3 : print("YAXIS",il,len(YAxisNames))
    if len(YPos) != 1 :
        plt.text(xtPos,ytPos,YAxisNames[il], color=lcolor)
    else :
        ytPos = yMin+(yMax-yMin)*(0.3-(il*0.1))
        plt.ylabel(YAxisNames[il], color=lcolor,rotation=90)

    plt.draw()
    ### Fit to a line
    if bFit == True :
        const,slope = line_fit(XPos, YPos[il])
        if verbose >= 3 : print(YAxisNames[il],'SLOPE fit: {:.2f}e-3'.format(slope*1000.))
        hXFit = []
        hYFit = []
        hXFit.append(XPos[0])
        hXFit.append(XPos[NPoints-1])
        hYFit.append(YPos[il][0])
        hYFit.append(YPos[il][NPoints-1])
        plt.plot(hXFit, hYFit, linestyle='dotted',color=lcolor)
        ytPos = yMin+(yMax-yMin)*(0.3-(il*0.1)-0.05)
        if verbose >= 3 : print(YAxisNames[il],'SLOPE fit: {:.2f}e-3'.format(slope*1000.))
        plt.text(xtPos,ytPos,'s={:.2f}e-3'.format(slope*1000.))
        
plt.savefig("histNColumn.jpg")

for il in range(nColumns) :
    his = Histo1D
    his.name = YAxisNames[il]
    step = (max(XPos)-min(XPos))/(len(XPos)-1)
    his.nbin = len(XPos)
    his.xmin = min(XPos)-step/2.
    his.xmax = max(XPos)+step/2.
    his.data = YPos[0]
    his.dataErr = [0] * his.nbin

    if verbose >= 3 : print(min(XPos), his.xmin ,step,max(XPos)-min(XPos),"DATA",his.data)
    if verbose >= 3 : print(max(XPos),"ERR",his.dataErr)
    fhis = open("plotNColumn.csv",'w')
    his.Write(his,fhis)
