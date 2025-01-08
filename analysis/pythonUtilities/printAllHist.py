import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import matplotlib.mlab as mlab
import numpy as np

from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader
from scipy import stats

bPlotErrors = 0
verbose = 3

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
histoFile = csvHistoReader(sys.argv[1])
iStat = 22222

if len(sys.argv) > 2 :
    bPlotErrors = int(sys.argv[2])
print("N ARGS= ",len(sys.argv),bPlotErrors)

if len(sys.argv) > 3 :
    iStat = int(sys.argv[3])
    
print("NHISTOS 1D=",len(histoFile.fHistos1D))
for his1 in histoFile.fHistos1D:
    hX = his1.Xbins()
    hY = his1.data
    #print(" HIS1 ",his1.name,":",his1.data,"+-",his1.dataErr," Xs=",hX)      
    xStep = his1.Xstep()
    plt.xlim(float(his1.xmin),float(his1.xmax))
    plt.xlabel(his1.name)
    if bPlotErrors == 1 :
        plt.bar(hX,hY, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep)
    elif bPlotErrors == 0 :        
        plt.plot(hX,hY) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
    else :
        print("!!! ERROR: second argument can only be 0 or 1, it is:",bPlotErrors)
        sys.exit()
    bStat = []
    for ii in range(7) :
        bStat.append(int((iStat/pow(10,ii))%10))
        #        print(ii,bStat[ii],"BSTAT",int(iStat/pow(10,ii))%10,int((iStat/pow(10,ii)))) #GDEB
    bContents = False
    if bContents :
        for ii in range(len(hX)) :
            print(his1.name,hX[ii],hY[ii])            
            
    for ii in range(7) :
        t2show=""
        ista = bStat[ii]
        # print(ii,"ISTA",ista)
        if ii == 0 :
            if ista >= 1 :
                t2show = his1.name+" NEnt= "+str(his1.nent)
        elif ii == 1 :
            if ista >= 1 :
                t2show = his1.name+" mean=  "+"{:.3f}".format(his1.mean)                    
            if ista >= 2 :
                t2show += " +- "+"{:.3f}".format(his1.meanErr)
        elif ii == 2 :
            if ista >= 1 :
                t2show = his1.name+" RMS=   "+"{:.3f}".format(his1.RMS)
            if ista >= 2 :
                t2show += " +- "+"{:.3e}".format(his1.RMSErr)
        elif ii == 3 :
            if ista >= 1 :
                t2show = his1.name+" Over=  "+"{:.3f}".format(his1.over)
            if ista >= 2 :
                t2show += " +- "+"{:.3f}".format(his1.overErr)
        elif ii == 4 :
            if ista >= 1 :
                t2show = his1.name+" Under= "+"{:.3f}".format(his1.under)
            if ista >= 2 :
                t2show += " +- "+"{:.3f}".format(his1.underErr)
        elif ii == 5 :
            # dataSum = reduce(lambda nn,mm: nn+mm,his1.data)
            dataSum = sum(his1.data)
            dataErrSum = sum([xx**2 for xx in his1.data])
            dataErrSum = np.sqrt(dataErrSum)
            #                print("INTEGRAM",dataSum,dataErrSum,sum(his1.dataErr*his1.dataErr))
            if ista >= 1 :
                t2show = his1.name+" Integr= "+"{:.3f}".format(dataSum)
                if ista >= 2 :
                    t2show += " +- "+"{:.3f}".format(dataErrSum)
        if t2show != "" : print(t2show+"")

    if verbose >= 3 :
        for ix in range(len(hX)) :
            print(ix,his1.name,"X= ",hX[ix],his1.data[ix])

    xmax = np.max(hX)
    ymax = np.max(hY)
    xdiff = (xmax-np.min(hX))*2
    nData = his1.nent
    t2show = "N=  "+str(nData)
    #   plt.text(xmax-0.25*xdiff,ymax*0.98,t2show)
    if np.sum(hY) != 0. :
        weighted_mean=np.average(hX, weights=hY)
    else :
        weighted_mean = np.float64(0.)
    print("weighted_mean ",type(weighted_mean ),weighted_mean )
    t2show = "mean=  "+"{:.3g}".format(weighted_mean)
    plt.text(xmax-0.25*xdiff,ymax*0.94,t2show,fontsize=20,fontweight='bold') #, fontfamily='serif')
    # Calcular la varianza ponderada de hX
    mean_diff_squared = (hX - weighted_mean)**2
    if np.sum(hY) != 0. :
        weighted_variance = np.average(mean_diff_squared, weights=hY)
    else :
        weighted_variance = np.float64(0.)
    # Calcular la desviacion estandar ponderada
    weighted_std = np.sqrt(weighted_variance)
    # Calcular el numero total de observaciones
    total_observations = np.sum(hY)
    # Calcular el error estandar de la media (SEM)
    if np.sum(hY) != 0. :
        sem = weighted_std / np.sqrt(total_observations)
    else :
        sem = 0.
    t2show = "SEM=   "+"{:.3g}".format(sem)
    # print(t2show) #GDEB
#    plt.text(xmax-0.25*xdiff,ymax*0.9,t2show)
    t2show = "STD=   "+"{:.3g}".format(weighted_std)
    plt.text(xmax-0.25*xdiff,ymax*0.84,t2show,fontsize=20,fontweight='bold')
  
    plt.savefig("hist"+his1.name+".jpg",)
    plt.clf()

    
print("NHISTOS 2D=",len(histoFile.fHistos2D))
for his2 in histoFile.fHistos2D:
    hX = his2.Xbins()
    hY = his2.Xbins()
    hZ = his2.dataNP
    if verbose >= 3 : print(" PLOT HIS2 ",his2.name)
    #            print(his2.name,"DATA ARRAY PLOTTED ",his2.dataNP)
    #https://www.freesion.com/article/7746457300/
    # matplotlib.colormaps[name]`` or ``matplotlib.colormaps.get_cmap(obj)`
    mycmap = mpl.colormaps["rainbow"]
    mask = his2.dataNP == 0  # Create a mask for zero elements
    data_masked = his2.dataNP.copy()  # Create a copy of data for modification
    data_masked[mask] = 'nan'
    mycmap.set_bad("white")
    # mycmap.set_under("white") # doesn't work
    #    plt.imshow(his2.dataNP,origin="lower",extent=(his2.xmin,his2.xmax/2,his2.ymin,his2.ymax),aspect='auto',interpolation='none', cmap='gray', vmin=0, vmax=1) #,norm=LogNorm(vin=0.001,vmax=1))
    #plt.imshow(his2.dataNP,extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),origin="lower", interpolation='none',cmap=mycmap)
    plt.imshow(data_masked,extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),origin="lower", interpolation='none',cmap=mycmap)
    plt.savefig("hist"+his2.name+".jpg")    
    plt.clf()
    hX.clear()
    hY.clear()
   # hZ.clear()
    
sys.exit()
    
