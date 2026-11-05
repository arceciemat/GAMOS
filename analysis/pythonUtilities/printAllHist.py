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
verbose = 0

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
histoFile = csvHistoReader(sys.argv[1])

bPlotErros = False
if len(sys.argv) > 2 :
    bPlotErrors = int(sys.argv[2])

iStat = 0000 #22222
if len(sys.argv) > 3 :
    iStat = int(sys.argv[3])

bContents = False
if len(sys.argv) > 4 :
    bContents = bool(sys.argv[4])

if verbose >= 3 : print("NHISTOS 1D=",len(histoFile.fHistos1D))
for his1 in histoFile.fHistos1D:
    hX = his1.Xbins()
    hY = his1.data
    if verbose >= 3 : print(" PLOT HIS1 ",his1.name)
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
    for ii in range(6) :
        bStat.append(int((iStat/pow(10,ii))%10))
        #        print(ii,bStat[ii],"BSTAT",int(iStat/pow(10,ii))%10,int((iStat/pow(10,ii)))) #GDEB

    if bContents :
        print("HX",hX) # GDEB
        for ii in range(len(hX)) :
            print(his1.name,hX[ii],hY[ii])            
            
    t2plot=""
    for ii in range(6) :
        t2print=""
        ista = bStat[ii]
        if ii == 0 :
            if ista >= 1 :
                t2print = his1.name+" NEnt= "+str(his1.nent)
        elif ii == 1 :
            if ista >= 1 :
                t2print = his1.name+" mean=  "+"{:.3g}".format(his1.mean)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his1.meanErr)
        elif ii == 2 :
            if ista >= 1 :
                t2print = his1.name+" RMS=   "+"{:.3g}".format(his1.RMS)
            if ista >= 2 :
                t2print += " +- "+"{:.3e}".format(his1.RMSErr)
        elif ii == 3 :
            if ista >= 1 :
                t2print = his1.name+" Over=  "+"{:.3g}".format(his1.over)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his1.overErr)
        elif ii == 4 :
            if ista >= 1 :
                t2print = his1.name+" Under= "+"{:.3g}".format(his1.under)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his1.underErr)
        elif ii == 5 :
            # dataSum = reduce(lambda nn,mm: nn+mm,his1.data)
            dataSum = sum(his1.data)
            dataErrSum = np.sqrt(sum([xx**2 for xx in his1.dataErr]))
            #                print("INTEGRAM",dataSum,dataErrSum,sum(his1.dataErr*his1.dataErr))
            if ista >= 1 :
                t2print = his1.name+" Integr= "+"{:.3g}".format(dataSum)
                if ista >= 2 :
                    t2print += " +- "+"{:.3g}".format(dataErrSum)
        if t2print != "" : print(ii,t2print)
        t2plot += t2print.replace(his1.name,"")+"\n"
        #print(ii,"ISTA",ista,bStat) #GDEB

    xmin, xmax = plt.xlim()
    ymin, ymax = plt.ylim()
    plt.text(xmax*0.98,ymax*0.98,t2plot,fontsize=8,va='top',ha='right') #, fontfamily='serif')
    
    plt.savefig("hist"+his1.name+".jpg",)
    plt.clf()
    hX.clear()
    
if verbose >= 3 : print("NHISTOS 2D=",len(histoFile.fHistos2D))
for his2 in histoFile.fHistos2D:
    hX = his2.Xbins()
    hY = his2.Ybins()
    hZ = his2.dataNP
    if verbose >= 3 : print(" PLOT HIS2 ",his2.name)
    #            print(his2.name,"DATA ARRAY PLOTTED ",his2.dataNP)
    mycmap = mpl.colormaps["rainbow"]
    mask = hZ == 0  # Create a mask for zero elements
    data_masked = hZ.copy()  # Create a copy of data for modification
    data_masked[mask] = 'nan'
    mycmap.set_bad("white")
    # mycmap.set_under("white") # doesn't work
    #    plt.imshow(his2.dataNP,origin="lower",extent=(his2.xmin,his2.xmax/2,his2.ymin,his2.ymax),aspect='auto',interpolation='none', cmap='gray', vmin=0, vmax=1) #,norm=LogNorm(vin=0.001,vmax=1))
    #plt.imshow(his2.dataNP,extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),origin="lower", interpolation='none',cmap=mycmap)
    img = plt.imshow(data_masked,extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),origin="lower", interpolation='none',cmap=mycmap)
    plt.colorbar(img, orientation="vertical")
    if bContents :
        print("HX",hX) # GDEB
        print("HY",hY) # GDEB
        for ix in range(len(hX)) :
            for iy in range(len(hY)) :
                print(his2.name,hX[ix],hY[iy],hZ[iy,ix])
                
    t2plot=""
    for ii in range(6) :
        t2print=""
        ista = bStat[ii]
        if ii == 0 :
            if ista >= 1 :
                t2print = his2.name+" NEnt= "+str(his2.nent)
        elif ii == 1 :
            if ista >= 1 :
                t2print = his2.name+" Xmean=  "+"{:.3g}".format(his2.xmean)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his2.xmeanErr)
            if ista >= 1 :
                t2print += "\n"+his2.name+" Ymean=  "+"{:.3g}".format(his2.ymean)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his2.ymeanErr)
        elif ii == 2 :
            if ista >= 1 :
                t2print = his2.name+" XRMS=   "+"{:.3g}".format(his2.xRMS)
            if ista >= 2 :
                t2print += " +- "+"{:.3e}".format(his2.xRMSErr)
            if ista >= 1 :
                t2print += "\n"+his2.name+" YRMS=   "+"{:.3g}".format(his2.yRMS)
            if ista >= 2 :
                t2print += " +- "+"{:.3e}".format(his2.yRMSErr)
        elif ii == 3 :
            if ista >= 1 :
                t2print = his2.name+" XOver=  "+"{:.3g}".format(his2.xover)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his2.xoverErr)
            if ista >= 1 :
                t2print += "\n"+his2.name+" YOver=  "+"{:.3g}".format(his2.yover)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his2.yoverErr)
        elif ii == 4 :
            if ista >= 1 :
                t2print = his2.name+" XUnder= "+"{:.3g}".format(his2.xunder)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his2.xunderErr)
            if ista >= 1 :
                t2print += "\n"+his2.name+" YUnder= "+"{:.3g}".format(his2.yunder)
            if ista >= 2 :
                t2print += " +- "+"{:.3g}".format(his2.yunderErr)
        elif ii == 5 :
            # dataSum = reduce(lambda nn,mm: nn+mm,his2.data)
            dataSum = np.sum(his2.dataNP)
            dataErrSum = np.sqrt(np.sum(his2.dataErrNP**2))
            #          print("INTEGRAM",dataSum,dataErrSum,sum(his2.dataErr*his2.dataErr))
            if ista >= 1 :
                t2print = his2.name+" Integr= "+"{:.3g}".format(dataSum)
                if ista >= 2 :
                    t2print += " +- "+"{:.3g}".format(dataErrSum)
        if t2print != "" : print(t2print)
        t2plot += t2print.replace(his2.name,"")+"\n"
        #print(ii,"ISTA",ista,bStat) #GDEB
#    print("T2PLOT",t2plot) #GDEB
    xmin, xmax = plt.xlim()
    ymin, ymax = plt.ylim()
    plt.text(xmax*0.98,ymax*0.98,t2plot,fontsize=8,va='top',ha='right') #, fontfamily='serif')
  
    plt.savefig("hist"+his2.name+".jpg")    
    plt.clf()
    hX.clear()
    hY.clear()
    
sys.exit()
    
