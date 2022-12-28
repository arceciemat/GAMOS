import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab

from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader

bPlotErrors = 1

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
verbose = 0
histoFile = csvHistoReader(sys.argv[1])

if len(sys.argv) > 2 :
    bPlotErrors = int(sys.argv[2])
print("N ARGS= ",len(sys.argv),bPlotErrors)

print("NHISTOS 1D=",len(histoFile.fHistos1D))
for his1 in histoFile.fHistos1D:
    hX = his1.Xbins()
    hY = his1.data
#    print(" HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",hX)            
    xStep = his1.Xstep(his1)
    plt.xlim(float(his1.xmin),float(his1.xmax))
    plt.xlabel(his1.name)
    if bPlotErrors == 1 :
        plt.bar(hX,hY, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep)
    elif bPlotErrors == 0 :        
        plt.plot(hX,hY) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
    else :
        print("!!! ERROR: second argument can only be 0 or 1, it is:",bPlotErrors)
        sys.exit()
    plt.savefig("hist"+his1.name+".jpg",)
    plt.clf()
    hX.clear()
    hY.clear()

print("NHISTOS 2D=",len(histoFile.fHistos2D))
for his2 in histoFile.fHistos2D:
    hX = his2.Xbins(his2)
    hY = his2.Xbins(his2)
    #            hY = his2.Ybins(his2)
    hZ = his2.data
    if verbose >= 3 : print(" PLOT HIS2 ",his2.name)
    #            print(his2.name,"DATA ARRAY PLOTTED ",his2.dataArray)
    #https://www.freesion.com/article/7746457300/
    cmap = mpl.cm.get_cmap("Blues").copy()
    plt.imshow(his2.dataArray,origin="lower",extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),aspect='auto',interpolation='none', cmap=cmap) #,norm=LogNorm(vin=0.001,vmax=1))
    plt.savefig("hist"+his2.name+".jpg")    
    plt.clf()
    hX.clear()
    hY.clear()
    hZ.clear()
    
sys.exit()
    
