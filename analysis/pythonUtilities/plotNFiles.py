import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab
import re

from checkPythonVersion import *
from MyHistos import Histo1D
from GetColor import *

from scipy.optimize import curve_fit

verbose = 3

def poly0( xx, const, slope ) :
    print("poly0(", xx, const, slope )
    return const + slope*xx
           
###################################
def line_fit(X, Y):

    xbar = sum(X)/len(X)
    ybar = sum(Y)/len(Y)
    n = len(X) # or len(Y)

    numer = sum([xi*yi for xi,yi in zip(X, Y)]) - n * xbar * ybar
    denom = sum([xi**2 for xi in X]) - n * xbar**2

    slope = numer / denom
    const = ybar - slope * xbar

    if verbose >= 1 : print('line fit: y = {:.2g} + {:.4f} x'.format(const, slope))

    param, covariance = curve_fit(poly0, X, Y, p0=[const*2, slope*1.1])

    print("PARAM",param)
    
    #    return const, slope
    return param[0], param[1]

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
bFit = False
bSort = True
bError = True
fOutName = "plotNFiles.jpg"
theFileNames = []
YAxisNames = []

### Manage arguments
if len(sys.argv) == 2 :
    fileName = sys.argv[1]
else :
    for ii in range(1,len(sys.argv)) :
        if(verbose >= 3) :  print("LOOPING line ",ii)
        if sys.argv[ii] == "-fOut" :
            bParamTypeFound = True
            fOutName = sys.argv[ii+1]
            
        elif sys.argv[ii] == "-f" :
            bParamTypeFound = True
            # check it is last argument
            for ii2 in range(ii+1,len(sys.argv)) :
                if sys.argv[ii2][0] == "-" :
                    print("!!! ERROR -f MUST BE LAST ARGUMENT")
                    sys.exit()
                else :
                    theFileNames.append(sys.argv[ii2])
                    ii = ii+1
                    if(verbose >= 3) : print( ii,"theFileNames ",len(theFileNames))
                    #-            CheckArgumentExists(len(theHisFileNames),ii)
            
        elif sys.argv[ii] == "-fl" :
            bParamTypeFound = True
            print(ii,"fl",len(sys.argv))
            flIn = open(sys.argv[ii+1])
            lines = flIn.readlines()
            for line in lines :
                words = line.rstrip().split()
                theFileNames.append(words[0])
            ii = ii+1
        elif sys.argv[ii] == "-bFit" :
            bFit = bool(int(sys.argv[ii+1]))
            ii = ii+1
        elif sys.argv[ii] == "-bSort" :
            bSort = bool(int(sys.argv[ii+1]))
            ii = ii+1
        elif sys.argv[ii] == "-bErr" :
            bError = bool(int(sys.argv[ii+1]))
            ii = ii+1

### Read file
ifn = 0
yMin = sys.float_info.max
yMax = sys.float_info.min
for fileName in theFileNames : 
    file = open(fileName)
    if verbose >= 3 : print("READING FILE",fileName)
    bData = 0
    lines = file.readlines()

    # FIRST LINE
    if verbose >= 3 : print("LINE0",lines[0][0:5],"  ",lines[0])
    if lines[0][0:5] == ":DATA" :
        bData = 1
        words = split_line_with_quotes(lines[0])
        nColumns = len(words)
        if nColumns == 3 :
            XAxisName = words[1]
            YAxisNames.append(words[2])
        else :
            if verbose >= 0 : print("!!! ERROR: first line must contain least three words: ':DATA' <X_axis> <Y_axis>, it is",lines[0])
            sys.exit()
            
    else :
        bData = 0
        if verbose >= 1 : print("!!! WARNING: first word of file ",sys.argv[1]," should be ':DATA'; first line is",lines[0])
        words = lines[0].rstrip().split()
        XAxisName = "XVAL"
        YAxisNames.append("YVAL")
        
    XPos = []
    YPos = []
    YPosErr = []
    for il in range(bData,len(lines)) :
        words = lines[il].rstrip().split()
        #print("WORDS",words)#GDEB
        if len(words) == 0 :
            continue
        XPos.append(float(words[0]))
        YPos.append(float(words[1]))
        nColumns = len(words)-1
        if bError and nColumns == 2:
            bErrorInFile = 1 
            YPosErr.append(float(words[2]))
        else :
            bErrorInFile = 0
            YPosErr.append(0.)
        #print(il,"NCOLUMS",nColumns,"bErrorInFile",bErrorInFile) #GDEB
  
        #print("BEFORE ORDER",YPos[0]) #GDEB
        NPoints = len(XPos)
        if bSort :
            SortIdx = sorted(range(NPoints), key=lambda k: XPos[k])
            XPosS = []
            for sid in SortIdx :
                XPosS.append(XPos[sid])
            XPos = XPosS
    
            YPosS = []
            YPosErrS = []
            for sid in SortIdx :
                # print("YPOSS",il,sid,len(YPos),len(YPos)) #GDEB
                YPosS.append(YPos[sid])
                if bError : 
                    YPosErrS.append(YPosErr[sid])
            YPos = YPosS
            YPosErr = YPosErrS
    
    if verbose >= 4 : print("AFTER ORDER",YPos) #GDEB

    #plt.xlim(float(his1.xmin),float(his1.xmax))
    plt.xlabel(XAxisName)
    yMin = min(yMin,min(YPos))
    yMax = max(yMax,max(YPos))

    if verbose >= 2 : print(fileName,"MAKE PLOT=",YAxisNames[ifn])
    lcolor = GetColor(ifn)
    if bError == False :
        plt.figure(figsize=(1000/100, 1000/100))        
        plt.plot(XPos,YPos, color=lcolor,marker="s",markersize=1.)
        print("XPOS",XPos)
        print("YPOS",YPos)
    else :
        plt.errorbar(XPos,YPos, color=lcolor, yerr=YPosErr,fmt="-o",markersize=2.,linestyle='-')
    plt.grid()
    #    print(il,"PLT.PLOT",XPos,YPos[il])#GDEB
    #    plt.legend()
    xtPos = min(XPos)+(max(XPos)-min(XPos))*0.5
    if verbose >= 3 : print("MAXY ",max(YPos),"*",(0.7-ifn*0.1)) 
    if verbose >= 3 : print(ifn,"TEXT POS",yMax)

    #plt.draw()
    ### Fit to a line
    if bFit == True :
        const,slope = line_fit(XPos, YPos)
        print("CONST",const,"SLOPE",slope)
        hXFit = []
        hYFit = []
        hXFit.append(XPos[0])
        hXFit.append(XPos[NPoints-1])
        hYFit.append(poly0(XPos[0],const,slope))
        hYFit.append(poly0(XPos[NPoints-1],const,slope)) 
        plt.plot(hXFit, hYFit, linestyle='dotted',color=lcolor)
        ytPos = yMin+(yMax-yMin)*(0.3-(ifn*0.2)) 
        #print(ifn,"DYTPOS",ytPos,yMin,yMax-yMin,0.3-(ifn*0.2)) #GDEB
        if verbose >= 3 : print(ytPos,YAxisNames[ifn],'SLOPE fit: {:.2g}'.format(slope))
        plt.text(xtPos,ytPos,'c={:.2g}'.format(const)+' s={:.2g}'.format(slope),color=lcolor)
        
    fhis = open("plotNFiles.csv",'w')
    his = Histo1D
    his.name = YAxisNames[ifn]
    if len(XPos) <= 1 :
        print("!!!! VERY FEW LINES IN FILE ",fileName,":",len(XPos))
        sys.exit()
    step = (max(XPos)-min(XPos))/(len(XPos)-1)    
    his.nbin = len(XPos)
    his.xmin = min(XPos)-step/2.
    his.xmax = max(XPos)+step/2.
    his.data = YPos
    if len(YPosErr) != 0 :
        his.dataErr = YPosErr
    else :
        his.dataErr = [0] * his.nbin

    if verbose >= 3 : print(min(XPos),"HISmin/max", his.xmin ,step,max(XPos)-min(XPos)) #,"DATA",his.data)
    if verbose >= 3 : print(max(XPos),"ERR",his.dataErr)
    his.Write(his,fhis)

    ifn += 1


## print in figure axis names
for ifn in range(len(YAxisNames)) :
    ytPos = yMin+(yMax-yMin)*(0.3-(ifn*0.1))
    #    if verbose >= 3 : print(ifn,"PLOT",XPos,YPos)#GDEB
    #    if verbose >= 3 : print("YAXIS",ifn,len(YAxisName))    
    lcolor = GetColor(ifn)
    ytPos = yMin+(yMax-yMin)*(1.-(ifn*0.1))
    print(ifn,"SYTPOS",ytPos,yMin,yMax-yMin,1.-(ifn*0.1)) #GDEB
    if verbose >= 3 : print(ifn,"TEXT POS",xtPos,ytPos,YAxisNames[ifn])
    if len(YPos) != 1 :
        plt.text(xtPos,ytPos,YAxisNames[ifn], color=lcolor)
    else :
        plt.ylabel(YAxisNames[il], color=lcolor,rotation=90)
plt.grid()

if verbose >= 3 : print("SAVING FILE ",fOutName)
plt.savefig(fOutName)
    
