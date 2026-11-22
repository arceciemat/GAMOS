import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader
from GetColor import GetColor
from matplotlib.ticker import MaxNLocator

plt.rcParams['figure.dpi'] = 600
verbose = 1
iNorm = 0
bLogY = False
theFitType = ""
iFitPlot = 11
theCompType = ""
bParamTypeFound = False
bErrors = 0
theCompXMin = -sys.float_info.max
theCompXMax = sys.float_info.max
theCompYMin = -sys.float_info.max                   
theCompYMax = sys.float_info.max
theHisYMin = -sys.float_info.max                   
theHisYMax = sys.float_info.max
bPlot2 = True
theCompYError = sys.float_info.max          
theGIndexPC = 0.
theGIndexDist = 0.
theCompYMinRel = 0.00 # Y values smaller than Ymax*theGIndexMinY get gammaMin = 0.
bGIndexAbs = False  # Absolute GI= PC1/YMax, Relative GI=PC1/Yaver12
theFilePrefix = ""
theFileSuffix = ""
theChi2Max = sys.float_info.max
bFWHM = False # Full Width Half Maximum
bFWTM = False # Full Width Tenth of Maximum
bDivide = False
theYLinePC = sys.float_info.max

bCompareExpLeftRight = 0

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()

listofDict=[]
# histype= sys.argv[1] #'Profile_X_at_Braggpeak'
# path='/home/natalia/gamos/GAMOS.6.3.0/Scripts/RADPROTIM_20ene/'
# files=sys.argv[2:]#[path+'analyseSqdose_xLETDose_80.2_1001_1000000_32.csv',path+'analyseSqdose_xLETDose_90.1_1001_1000000_32.csv']
#anadir un modificador para que lo entienda como tipo de variable

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
def GetValue(ii) :
    if len(sys.argv) <= ii :
        print("!!! ERROR no word after argument ",sys.argv[ii],len(sys.argv)," > ",ii+1)
        sys.exit()
    return float(sys.argv[ii])
        
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
def printHistosInList(HisNames,HisFiles) :
    theCompXMin = -sys.float_info.max
    theCompXMax = sys.float_info.max
    if(verbose >= 3) : print("CALLED printHistosInList",HisNames)
    chi2Total = 0
#### LOOP TO HISTOGRAM NAMES
    for hisn in HisNames :
        bFound = False
        if(verbose >= 3) : print("start loop",hisn,len(HisFiles))
        ## LOOK FOR HISTO NAME IN EACH FILE
        ii = 0
        hisList = []
        if bCompareExpLeftRight == 0 :
            ax1 = plt.subplot2grid((4,1),(0,0),rowspan=3)
            ax2 = plt.subplot2grid((4,1),(3,0),rowspan=1)
            plt.subplots_adjust(hspace=0.4)
        else :
            ax1 = plt.subplot2grid((5,1),(0,0),rowspan=3)
            ax2 = plt.subplot2grid((5,1),(3,0),rowspan=1)
            ax3 = plt.subplot2grid((5,1),(4,0),rowspan=1)

            
### LOOP TO HISTOGRAM FILES TO CHECK IF HISTOGRAM NAME IS FOUND (APPEND IT TO hisList)
        #print("loop N files",len(HisFiles)) #GDEB
        hisFileNames = []
        for hfile in HisFiles :
            if(verbose >= 3) : print("loop N histo in file",len(hfile.fHistos1D))
            for his1 in hfile.fHistos1D :
                if(verbose >= 4) : print(ii,"CHECK name ",hisn,"=?=",his1.name)
                if his1.name == hisn :
                    hisList.append(his1)
                    hisFileNames.append(hfile.fileName)
                    if verbose >= 2 : print(ii,len(hisList),"HISTO FOUND ",his1.name)
                    if verbose >= 2 and len(hisList) == 2 : print(len(hisList),"2 HISTOS FOUND ",his1.name)
                    # check equal 
                    if not bFound :
                        histo1st = his1
                        bFound = True

        #print("XPC??",theYLinePC,hisList[0].name)
        if theYLinePC != sys.float_info.max :
            his1yl = hisList[0]
            hX = his1yl.Xbins()
            hY = his1yl.data
            ymax = max(hY)
            yOK = theYLinePC*ymax
            xPC = his1yl.xmin # -sys.float_info.max 
            # xPCMin = find the X point where histogram intercepts yOK, starting from the left
            for iir in range(len(hX)-1) :
                if hY[iir] == yOK :
                    xPC = hX[iir]
                    #print(iir,"XPC EXACT",hY[iir]) #GDEB
                    break
                #print(iir,hX[iir]," CHECK XPC ",hY[iir]," < ",yOK," and ",hY[iir+1]," > ",yOK) # GDEB 
                if hY[iir] < yOK and hY[iir+1] > yOK : # interpolate
                    xPC = hX[iir] + (hX[iir+1]-hX[iir])/(hY[iir+1]-hY[iir])*(yOK-hY[iir])
                    #print(iir,yOK,"XPC+",xPC," = ",hX[iir]," + (",hX[iir+1]-hX[iir],")/(",hY[iir+1]-hY[iir],")*(",yOK-hY[iir]) # GDEB
                    break 
            if xPC != his1yl.xmin : # -sys.float_info.max :
                ax1.axvline(x=xPC, color='g', linestyle='--',linewidth=1)
                ax2.axvline(x=xPC, color='g', linestyle='--',linewidth=1)
            if bCompareExpLeftRight == 1 :
                ax3.axvline(x=xPC, color='g', linestyle='--',linewidth=1)
            theCompXMin = max(0.5*xPCMin-0.5*xPCDiff,his1yl.xmin)
            theCompXMax = min(0.5*xPCMax+0.5*xPCDiff,his1yl.xmax)
            xPCMin = xPC

            # xPCMax = find the X point where histogram intercepts yOK, starting from the right
            xPC = his1yl.xmax # -sys.float_info.max
            for iir in range(len(hX)-1,1,-1) :
                if hY[iir] == yOK :
                    xPC = hX[iir]
                    break
                #print(iir," CHECK XPC ",hY[iir-1]," > ",yOK," and ",hY[iir]," < ",yOK)
                if hY[iir-1] > yOK and hY[iir] < yOK : # interpolate
                    xPC = hX[iir] + (hX[iir]-hX[iir-1])/(hY[iir]-hY[iir-1])*(yOK-hY[iir])
                    #print(iir,yOK,"XPC-",xPC," = ",hX[iir]," + (",hX[iir]-hX[iir-1],")/(",hY[iir]-hY[iir-1],")*(",yOK-hY[iir])
                    break 
            if xPC != his1yl.xmax : # -sys.float_info.max :
                ax1.axvline(x=xPC, color='g', linestyle='--',linewidth=1)
                ax2.axvline(x=xPC, color='g', linestyle='--',linewidth=1)
            if bCompareExpLeftRight == 1 :
                ax3.axvline(x=xPC, color='g', linestyle='--',linewidth=1)
            xPCMax = xPC
            xPCDiff = xPCMax-xPCMin
            #      theCompXMin = max(xPC*1.5,his1yl.xmin)
            #      theCompXMax = min(xPC*1.5,his1yl.xmax)
            theCompXMin = max(xPCMin-0.5*xPCDiff,his1yl.xmin)
            theCompXMax = min(xPCMax+0.5*xPCDiff,his1yl.xmax)

#### PLOT HISTOGRAMS IF FOUND IN MORE THAN 1 ILE
#print("N HISLIST ",len(hisList),hisList) #GDEB
        if len(hisList) >= 2 :
            for his1,hisFN1 in zip(hisList,hisFileNames) : 
                if verbose >= 2 : print("PLOTTING HIST",his1.name)
                # print("NORM ",iNorm) #GDEB
                if iNorm == 1 :
                    his1.normToIntegral(iNorm/his1.Xstep())
                elif iNorm == 2 :
                    print("01compNORM",his1.name,np.max(his1.data),np.argmax(his1.data),his1.Xbins()[np.argmax(his1.data)])
                    his1.normToValue(1.)
                    print("11compNORM",his1.name,np.max(his1.data),np.argmax(his1.data),his1.Xbins()[np.argmax(his1.data)])
                hX = his1.Xbins()
                hY = his1.data
                #    print(" HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",hX)
                xStep = his1.Xstep()
                # plt.xlim(float(his1.xmin),float(his1.xmax))
                if theCompXMin != -sys.float_info.max and theCompXMax != sys.float_info.max :
                    plt.xlim(float(theCompXMin),float(theCompXMax))
                plt.xlabel(his1.name)
                ax1.grid()
                #print("BLOG",bLogY)
                if bLogY : 
                    plt.yscale('log')
                else :
                    plt.yscale('linear')
                if theCompXMin != -sys.float_info.max and theCompXMax != sys.float_info.max :
                    ax1.set_xlim(theCompXMin,theCompXMax)
                if theHisYMin != -sys.float_info.max :
                    ax1.set_ylim(bottom=theHisYMin)
                if theHisYMax != sys.float_info.max :
                    ax1.set_ylim(top=theHisYMax)
                if bErrors == 1 :
                    lcolor = GetColor(hisFileNames.index(hisFN1))
                    #print("err",his1.dataErr) #GDEB
                    #     ax1.errorbar(hX,hY, yerr=his1.dataErr, color=lcolor, fmt='o',markersize=2)
                    ax1.errorbar(hX,hY, yerr=his1.dataErr, color=lcolor, marker='o',markersize=2)
                elif bErrors == 0 :
                    if(verbose >= 4) : print("HX ",len(hX)," HY ",len(hY))
                    if(verbose >= 4) : print("HX ",hX," HY ",hY)
                    if bLogY : 
                        ax1.set_yscale('log') 
                    ax1.plot(hX,hY)
                    
                if theFitType != "" :
                    if theFitType != "Gaussian" :
                        print("!!! ERROR: only fit Gaussian is currently supported, not ",theFitType)
                        sys.exit()
                    print("CALL his1.GaussianFit(",ii, "iFitPlot",iFitPlot) #GDEB
                    param,cov = his1.GaussianFit(ii, iFitPlot)
                    if verbose >= 0 : print("GAUSSIAN_FIT PARAM ",hisFN1,his1.name,param[0],param[1],param[2])
                    if verbose >= 1 :
                        print("GAUSSIAN_FIT COV ",hisFN1,his1.name,cov)              
                        print("GAUSSIAN_FIT DIAG_ERRORS ",hisFN1,his1.name,np.sqrt(cov[0][0]),np.sqrt(cov[1][1]),np.sqrt(cov[2][2]))

                if bFWHM :
                    # fwhm = his1.FWHM()
                    idMax = np.argmax(his1.data)
                   # xhmp, xhmn = his1.FWPCPosNegFit2g(0.5)
                   # fwhm = xhmp-xhmn
                   # print("FIT xhmp, xhmn",xhmp,xhmn,fwhm)
                    xhmp, xhmn = his1.FWPCPosNeg(0.5) # use pos neg to get approximage statistical error
                    fwhm = xhmp-xhmn
                    #print("NO FIT xhmp, xhmn",xhmp,xhmn,fwhm)
                    if verbose >= 0 : print("FWHM ",hisFN1,his1.name,fwhm,xhmn,xhmp)
                    if ii == 0 :
                        lcolor = 'black'
                        stext="MC FWHM="
                    elif ii == 1:
                        lcolor = 'red'
                        stext="Exp. FWHM="
                    xPos = his1.xmax*0.2
                    yPos = his1.maximumY()*(0.6-ii*0.1)
                    #            plt.text(xPos,yPos,"mean= "+"{:.4e}".format(param[1]), color=lcolor)
                    #            yPos = his1.maximumY()*(0.65-ii*0.1)
                    #print("SIGMA COLOR",lcolor,"xytext",xPos,yPos,) #GDEB
                    matplotlib.rcParams.update({'font.size': 15})
                    plt.text(xPos,yPos,stext+" "+"{:.4g}".format(fwhm), color=lcolor)
                    plt.draw()
                if bFWTM :
                    # fw10 = his1.FWTM()
                    idMax = np.argmax(his1.data)
                    x10p, x10n = his1.FWPCPosNegFit2g(0.1)
                    fw10 = x10p-x10n
                    print("FIT x10p, x10n",x10p,x10n,fw10)
                    x10p, x10n = his1.FWPCPosNeg(0.1) # use pos neg to get approximage statistical error
                    fw10 = x10p-x10n
                    print("NO FIT x10p, x10n",x10p,x10n,fw10)
                    if verbose >= 0 : print("FWTM ",hisFN1,his1.name,fw10,x10n,x10p)
                    if ii == 0 :
                        lcolor = 'black'
                        stext="MC FWTM="
                    elif ii == 1:
                        lcolor = 'red'
                        stext="Exp. FWTM="
                    xPos = his1.xmax*0.2
                    yPos = his1.maximumY()*(0.6-ii*0.1)
                    #            plt.text(xPos,yPos,"mean= "+"{:.4e}".format(param[1]), color=lcolor)
                    #            yPos = his1.maximumY()*(0.65-ii*0.1)
                    #print("SIGMA COLOR",lcolor,"xytext",xPos,yPos,) #GDEB
                    matplotlib.rcParams.update({'font.size': 15})
                    plt.text(xPos,yPos,stext+" "+"{:.4g}".format(fw10), color=lcolor)
                    plt.draw()

            ii += 1

        #print("2BLOG",bLogY)
        if bLogY : 
            plt.yscale('log')

#### COMPARE WITH chi2 
        if theCompType == "chi2" and len(hisList) == 2:
            if verbose >= 3 : print(hisn,"LOOP HISLIST ",hisList[0].name,len(hisList[0].data))
            chi2 = 0
            dof = 0
            his1 = hisList[0]
            his2 = hisList[1]
            Xbins1 = his1.Xbins()
            hXComp = []
            hYComp = []
            hYErrComp = []
            if verbose >= 3 : print(his1.name,"len(Xbins1",len(Xbins1))
            for ii in range(len(Xbins1)) :
                xval1 = Xbins1[ii]
                if xval1 < theCompXMin or xval1 > theCompXMax :
                    continue
             #   print(his1.name,ii,len(his1.data)) #GDEB
                yval1 = his1.data[ii]
                yerr1 = his1.dataErr[ii]
                yval2,yerr2 = his2.GetValueErr(xval1)
                hXComp.append(Xbins1[ii])
                bContinue = False
                if yval2 == None :
                    if verbose >= 3 : print("COMP NOT USED yval2",ii,yval2)
                    bContinue = True
                else :
                    #                 print("COMP",xval1,theCompXMin,xval1,theCompXMax) #GDEB
                    if xval1 < theCompXMin or xval1 > theCompXMax:
                        if verbose >= 3 : print("COMP NOT USED xval1 out",ii,theCompXMin,xval1,theCompXMax)
                        bContinue = True
                    else :
                        if yval1 < theHisYMin or yval1 > theHisYMax:
                            if verbose >= 3 : print("COMP NOT USED xyal1 out",ii,theHisYMin,yval1,theHisYMax)
                            bContinue = True
                        else : 
                            if theCompYError == sys.float_info.max and yerr1 + yerr2 == 0. :
                                if verbose >= 3 : print("COMP NOT USED yerr",ii,yerr1,yerr2)
                                bContinue = True
                if bContinue:
                    hYComp.append(0.)
                    hYErrComp.append(0.)
                    continue

                if yval1 != 0. and yval2 != 0. :
                    dof += 1                    
                    if verbose >= 3 : print("COMP",yval1,yval2,dof)
                if theCompYError == sys.float_info.max :
                    yerr = np.sqrt(yerr1*yerr1+yerr2*yerr2)
                else :
                    yerr = theCompYError
                if yval1 != 0 :
                    hYErrComp.append(yerr)
                else :
                    hYErrComp.append(0.)
                chi2_1 = abs(yval1-yval2)/yerr
                
                if theChi2Max == sys.float_info.max :                
                    hYComp.append(chi2_1)
                else :
                    hYComp.append(min(chi2_1,theChi2Max))
                chi2 += chi2_1
                if verbose >= -3 : print(his1.name,xval1,"chi2_1",chi2_1,"",chi2,yval1,"-",yval2,"/",yerr,yerr1,yerr2)
            pval = 0.
            chi2 /= dof
            if verbose >= 1 : print("@@@",his1.name,"CHI2=",chi2," dof=",dof,"chi2*dof=",chi2*dof)
            chi2Total += chi2
            # ax1.legend()
            hxmin = max(his1.xmin,his2.xmin)
            hxmax = min(his1.xmax,his2.xmax)
            xPos = hxmin+(0.6)*(hxmax-hxmin)
            hymin = max(his1.minimumY(),his2.minimumY())
            hymax = min(his1.maximumY(),his2.maximumY())
            yPos = hymin+(hymax-hymin)*(0.95-0.2*int(theFitType!="")) 
            lcolor = 'black'
            ax1.text(xPos,yPos,"chi2= "+"{:.4f}".format(chi2), color=lcolor)
            yPos = his1.maximumY()*(0.65-0.2*int(theFitType!=""))
            ax1.text(xPos,yPos,"p-val= "+"{:.4f}".format(pval), color=lcolor)
            #t            ax1.draw()
             
            #            ax2.bar(hXComp,hYComp, color='None', edgecolor='black', yerr=hYErrComp) #, width=xStep)
            if theCompXMin != -sys.float_info.max and theCompXMax != sys.float_info.max :
                ax2.set_xlim(theCompXMin,theCompXMax)
            if theCompYMin != -sys.float_info.max :
                ax2.set_ylim(bottom=theCompYMin)
            if theCompYMax != sys.float_info.max :
                ax2.set_ylim(top=theCompYMax)
            if verbose >= 3 : print("CHI2ylim",theCompYMin,theCompYMax)
            ax2.plot(hXComp,hYComp) 
            ax2.axhline(y=1., color='g', linestyle='--',linewidth=1)
#            if bLogY : 
 #               plt.yscale('log')
            ax2.grid()
            #t            ax2.draw()

        if verbose >= 3 : print(theCompType,len(hisList),"test GAMMA INDEX ",theGIndexPC, theGIndexDist,len(hisList))
#### COMPARE WITH GAMMA INDEX 
#if theGIndexPC != 0. and len(hisList) == 2:
        if theCompType == "GI" and len(hisList) == 2:

            if verbose >= 3 : print(hisn,"agi LOOP HISLIST ",hisList[0].name,len(hisList[0].data))
            nGImore1 = 0
            nGI = 0
            GIAver = 0.
            GIMaxFound = -999#sys.float_info.min
            GIMaxNbins = 3
            his1 = hisList[0]
            his2 = hisList[1]
            if iNorm == 1 :
                his2.normToIntegral(iNorm/his2.Xstep())
            elif iNorm == 2 :
                print("02compNORM",his2.name,np.max(his2.data),np.argmax(his2.data),his2.Xbins()[np.argmax(his2.data)])
                his2.normToValue(1.)
                print("12compNORM",his2.name,np.max(his2.data),np.argmax(his2.data),his2.Xbins()[np.argmax(his2.data)])
            Xbins1 = his1.Xbins()
            hXGI = []
            hYGI = []
            hYErrGI = []
            xStep = his1.Xstep()
            ## GET how many around to llok
            yMax1 = max(his1.data)
            #print("YMAX1",yMax1,his1.data) #GDEB
            theGammaIndexValues = {}
            for ii in range(len(Xbins1)) :
                theGammaIndexValues[ii] = -1 # in case "continue" is called
                xval1 = Xbins1[ii]
                if xval1 < theCompXMin or xval1 > theCompXMax:
                    if verbose >= 3 : print(his1.name,ii,"GAMMA INDEX NOT USED xval1 out of limits",ii,theCompXMin,xval1,theCompXMax)
                    hXGI.append(Xbins1[ii])
                    hYGI.append(0.)
                    continue
             #   print(his1.name,ii,len(his1.data)) #GDEB
                yval1 = his1.data[ii]
                yerr1 = his1.dataErr[ii]
                #print(his1.name,ii,"XYval1",xval1,yval1,yerr1) #GDEB
                # GET PC: COMPARE WITH SAME xval
                if xval1 < his2.xmin or xval1 > his2.xmax :
                    yval2,yerr2 = None,None
                else :
                    yval2,yerr2 = his2.GetValueErr(xval1)
                if yval2 == None : # different limits in the two histograms
                    continue 
                #print("yval2",yval2,xval1,his2.xmin,his2.xmax) 
                yaver = (yval1+yval2)/2.
                if bGIndexAbs :
                    PCCurrent = abs((yval2-yval1)/yMax1)/theGIndexPC
                    if verbose >= 3 : print(ii,":",Xbins1[0]+(ii-0.5)*xStep,"33GI ABS xval1: ",xval1," AbsMaxPCCurrent",PCCurrent,yval2,"-",yval1,"=",yval2-yval1,"/",yMax1,"/",theGIndexPC)
                else :
                    if yaver != 0 : 
                        PCCurrent = abs((yval2-yval1)/yaver)/theGIndexPC
                    else :
                        PCCurrent = 0.                        
                    if verbose >= 3 : print(ii,":",Xbins1[0]+(ii-0.5)*xStep,"GI REL xval1: ",xval1," PCCurrent",PCCurrent,"=",yval2,"-",yval1,"/",yaver,"/",theGIndexPC)
                gammaMin = PCCurrent
                bBinByBinDist = False
                if bBinByBinDist :
                    ### Get how many voxels around to look for distance (so that dist/theGIDist < PCCurrent
                    iDist = int(PCCurrent/(xStep/theGIndexDist))
                    ### If second has Y=0 => GI=0
                    if yval2 == 0. :
                        gammaMin = 0.
                    else :
                        if verbose >= 3 : print("GI: iDist",iDist,"=",PCCurrent,"/",xStep,"/",theGIndexDist)
                        for ix2 in range( ii-iDist,ii+iDist+1) :
                            if ix2 < 0 : continue
                            if ix2 >= his1.nbin : continue
                            yval2,yerr2 = his2.GetValueErr(Xbins1[ix2])
                            if yval2 == None : # different limits in the two histograms
                                continue
                            #PCCurrent = abs((yval2-yval1)/yMax1)/theGIndexPC
                            yaver = (yval1+yval2)/2.
                            if bGIndexAbs :
                                PC1 = abs((yval2-yval1)/yMax1)/theGIndexPC
                                if verbose >= 3 : print(ii,ix2,"AbsMaxGI: PC1=",PC1,"=abs(("+str(yval2)+"-"+str(yval1)+")/"+str(yMax1)+")/"+str(theGIndexPC))
                            else :
                                if yaver != 0 :
                                    PC1 = abs((yval2-yval1)/yaver)/theGIndexPC
                                else :
                                    PC1 = 0.
                                    if verbose >= 3 : print(ii,ix2,"GI: PC1=",PC1,"=abs(("+str(yval2)+"-"+str(yval1)+")/"+str(yaver)+")/"+str(theGIndexPC))
                            Dist1 = (ix2-ii)*xStep/theGIndexDist
                            gamma1 = np.sqrt(np.power(PC1,2)+np.power(Dist1,2))
                            gammaMin = min(gammaMin,gamma1)
                            if verbose >= 3 : print(ii,ix2,"GI: PC1/dist1",gamma1,PC1,Dist1,"gmin=",gammaMin)
                    
                else :
                    xval2 = his2.GetClosestX(yval1,xval1)
                    gamma1 = np.abs(xval2-xval1)/theGIndexDist 
                    gammaMin = min(gammaMin,gamma1)
                    if verbose >= 3 : print(ii,"GI: ",gammaMin,"gi1_PC",PCCurrent,"gi1_dist",gamma1,"=",xval2,"-",xval1)

                if yaver < yMax1*theCompYMinRel : # Too low y, do not consider GI
                    gammaMin = 0.
                    if verbose >= 3 : print(ii,"GI: made 0 ",yaver," < ",yMax1,"*",theCompYMinRel)

                if gammaMin < theCompYMin :
                    if verbose >= 3 : print("GAMMA INDEX TOO LOW=",gammaMin," SET TO ",theCompYMin,"NOT COUNTED IN STATS")
                    hXGI.append(Xbins1[ii])
                    hYGI.append(theCompYMin)
                    if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL GI=",gammaMin)
                    continue
                if gammaMin > theCompYMax:
                    if verbose >= 3 : print("GAMMA INDEX TOO HIGH=",gammaMin," SET TO ",theCompYMax,"NOT COUNTED IN STATS")
                    hXGI.append(Xbins1[ii])
                    hYGI.append(theCompYMax)
                    if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL GI=",gammaMin)
                    continue

                if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL GI=",gammaMin)
                
                hXGI.append(Xbins1[ii])
                hYGI.append(gammaMin)
                if verbose >= 3 : print(his2.name,ii,xval1,len(hYGI),"HYGI append",gammaMin,"GI: ",gammaMin,"gi1_PC",PCCurrent,"gi1_dist",gamma1," :",yval1,"",yval2)
                GIAver += gammaMin
                theGammaIndexValues[ii] = gammaMin
                if ii > 1 :
                    gi3 = (theGammaIndexValues[ii-2]+theGammaIndexValues[ii-1]+theGammaIndexValues[ii])/3.
                    GIMaxFound = max(GIMaxFound,gi3)
                nGI += 1
                if gammaMin > 1. : nGImore1 += 1
                if verbose >= 2 : print(ii,his1.name,"nGI",nGI,"GI:",gammaMin,xval1,yval1,yval2,PCCurrent)
            GIAver /= nGI

            #ax1.legend()
            hxmin = max(his1.xmin,his2.xmin)
            hxmax = min(his1.xmax,his2.xmax)
            xPos = hxmin+(0.6)*(hxmax-hxmin)
            hymin = max(his1.minimumY(),his2.minimumY())
            hymax = min(his1.maximumY(),his2.maximumY())
            if not bLogY :
                yPos = hymin+(hymax-hymin)*(0.55-0.2*int(theFitType!=""))
                #print(his1.name,"1YPOS",yPos,"=",his1.minimumY(),"+(",hymax,hymin,")*(0.95-0.2*",int(theFitType!=""),0.95-0.2*int(theFitType!="")) #GDEB
            else :
         #       yPos = hymin+(np.log(hymax)-np.log(hymin))*(0.55-0.2*int(theFitType!=""))
#                print("HYmin",hymin,HYMAX,hymax)
                yPos = hymin+(hymax-hymin)*(0.55-0.2*int(theFitType!=""))
                # print(his1.name,"1YPOS",yPos,"=",his1.minimumY(),"+(",np.log(hymax),np.log(hymin),")*(0.95-0.2*",int(theFitType!=""),0.95-0.2*int(theFitType!="")) #GDEB

            lcolor = 'black'
            eners = his1.name[1:].replace("_Depth","")
            ax1.text(xPos,yPos,"GI= "+"{:.4f}".format(GIAver), color=lcolor)
            #print(eners,theHisFileNames[0],"GIaver= "+"{:.4f}".format(GIAver),"max3GI= "+"{:.4f}".format(GIMaxFound))
            print(eners,"GIaver= "+"{:.4f}".format(GIAver),"max3GI= "+"{:.4f}".format(GIMaxFound)) #GDEB
            
            yPos = his1.maximumY()*(0.48-0.2*int(theFitType!=""))
            # print(his1.name,"2YPOS",yPos,"=",his1.minimumY(),"+(",his1.maximumY(),"-",his1.minimumY(),")*(0.95-0.3*",int(theFitType!="")) #GDEB
            if nGImore1 != 0 :
                ax1.text(xPos,yPos,"nGI>1= "+str(nGImore1)+"/"+str(nGI), color=lcolor)
            #t            ax1.draw()
           
            #            ax2.bar(hXGI,hYGI, color='None', edgecolor='black') #, width=xStep)            
#            for ii in range(len(hXGI)) :
                #print(ii,"HXYGI",hXGI[ii],hYGI[ii]) #GDEB
            if theCompXMin != -sys.float_info.max and theCompXMax != sys.float_info.max :
                ax2.set_xlim(theCompXMin,theCompXMax)
                #for iiy in range(len(hYGI)) :
               # print(his2.name,iiy," HXY ", hXGI[iiy], hYGI[iiy]) #GDEB 
            if theCompYMin != -sys.float_info.max :
                ax2.set_ylim(bottom=theCompYMin)
            if theCompYMax != sys.float_info.max :
                ax2.set_ylim(top=theCompYMax)
            print("ax2.plot(hXGI,hYGI")
            ax2.plot(hXGI,hYGI) 
            ax2.axhline(y=1., color='g', linestyle='--',linewidth=1)
            if bLogY : 
                plt.yscale('log')
            ax2.grid()
            #t            ax2.draw()

#### COMPARE WITH DIVISION
        if theCompType == "div" and len(hisList) == 2:
            if verbose >= 3 : print(hisn,"div LOOP HISLIST ",hisList[0].name,len(hisList[0].data))
            his1 = hisList[0]
            his2 = hisList[1]
            Xbins1 = his1.Xbins()
            hXDIV = []
            hYDIV = []
            hYDIVABS = []
            hYErrDIV = []
            xStep = his1.Xstep()
            ## GET how many around to look
            yMax1 = max(his1.data)
            #print("YMAX1",yMax1,his1.data) #GDEB
            print("NBINS",len(Xbins1))
            for ii in range(len(Xbins1)) :
                xval1 = Xbins1[ii]
                if xval1 < theCompXMin or xval1 > theCompXMax: 
                    if verbose >= 3 : print(his1.name,ii,"NOT USED xval1 out of limits",ii,theCompXMin,xval1,theCompXMax)
                    hXDIV.append(Xbins1[ii])
                    hYDIV.append(0.)
                    hYDIVABS.append(0.)
                    continue
             #   print(his1.name,ii,len(his1.data)) #GDEB
                yval1 = his1.data[ii]
                yerr1 = his1.dataErr[ii]
                #print(his1.name,ii,"XYval1",xval1,yval1,yerr1) #GDEB
                # GET PC: COMPARE WITH SAME xval
                if xval1 < his2.xmin or xval1 > his2.xmax :
                    yval2,yerr2 = None,None
                else :
                    yval2,yerr2 = his2.GetValueErr(xval1)
                if yval2 == None : # different limits in the two histograms
                    continue 
                #print("yval2",yval2,xval1,his2.xmin,his2.xmax) 
                yaver = (yval1+yval2)/2.

                xval2 = his2.GetClosestX(yval1,xval1)
                    
                if yval2 != 0 :
                    #print(ii,"CHANGE gi",yval1/yval2,"=",yval1,"/",yval2,"OLD",divValue) #GDEB
                    divValue = yval1/yval2
                    divValueABS = (yval1-yval2)/yMax1
                else:
                    divValue = 0.
                    divValueABS = 0.

                if divValue < theCompYMin :
                    if verbose >= 3 : print("DIV VALUE TOO LOW=",divValue," SET TO ",theCompYMin,"NOT COUNTED IN STATS")
                    hXDIV.append(Xbins1[ii])
                    hYDIV.append(theCompYMin)
                    hYDIVABS.append(divValueABS)
                    if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL DIV=",divValue)
                    continue
                if divValue > theCompYMax:
                    if verbose >= 3 : print("DIV VALUE TOO HIGH=",divValue," SET TO ",theCompYMax,"NOT COUNTED IN STATS")
                    hXDIV.append(Xbins1[ii])
                    hYDIV.append(theCompYMax)
                    hYDIVABS.append(divValueABS)
                    if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL DIV=",divValue)
                    continue

                if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL DIV=",divValue)
                
                hXDIV.append(Xbins1[ii])
                hYDIV.append(divValue)
                hYDIVABS.append(divValueABS)
            if verbose >= 3 :
                for ii in range(len(hYDIV)) :
                    print("DIVVALUE ",hYDIV[ii],hXDIV[ii],ii,hisn)
                    print("DIVVALUE_ABS ",hYDIVABS[ii],hXDIV[ii],ii,hisn,his1.data[ii],his2.data[ii])

            lcolor = 'black'
           
            if theCompXMin != -sys.float_info.max and theCompXMax != sys.float_info.max :
                ax2.set_xlim(theCompXMin,theCompXMax)
            ax2.set_ylim(theCompYMin,theCompYMax)
            if verbose >= 3 : print("CHI2ylim",theCompYMin,theCompYMax)
#            ax2.set_yticks([])
            ax2.plot(hXDIV,hYDIV)
#            ax2.tick_params(axis='y', labelsize=11)
#            new_yticks = np.linspace(theCompYMin, theCompYMax, 5)
#            ax2.set_yticks(new_yticks)
#            ax2.set_yticklabels([f"{tick:.1f}" for tick in new_yticks])
#            ax2.yaxis.set_minor_locator(plt.NullLocator())
#            plt.tight_layout()
##            ax2.set_yticks(new_ticks)
#            ax2.set_yticklabels([f"{tick:.1f}" for tick in new_ticks])
            ax2.axhline(y=1., color='g', linestyle='--',linewidth=1)
#            ax2.set_yticks([])
##            ax2.set_yticks(np.linspace(0.8, 1.2, 5))
#            #ax2.yticks(np.linspace(0.8, 1.2, 5))  # 5 ticks from -1 to 1
##            ax2.yaxis.set_major_locator(MaxNLocator(nbins=5))

            #            ax2.bar(hXGI,hYGI, color='None', edgecolor='black') #, width=xStep)            

            #### Get
            if bCompareExpLeftRight == 1 :
                hXDIV.clear()
                hYDIV.clear()
                print("bCompareExpLeftRight",hisFileNames[1].find("Exp"),his2.name)
                if hisFileNames[1].find("Exp") != -1 :
                    his2Inv = Histo1D()
                    his2Inv.FillFromArrays(his2.data,his2.dataErr,his2.xmin,his2.xmax)
                    his2Inv.InvertX()
                    #print("his2DATA",his2.data)
                    #print("his2InvDATA",his2Inv.data)
#                    for ii in range(his2.nbin) :
 #                       print(ii,"HIS2 INV",his2.Xbins()[ii],his2.data[ii],his2Inv.data[ii])
                    hYDIV.clear()
                    for ii in range(len(Xbins1)) :
                        xval1 = Xbins1[ii]
                        if xval1 < theCompXMin or xval1 > theCompXMax: 
                            if verbose >= 3 : print(his1.name,ii,"NOT USED xval1 out of limits",ii,theCompXMin,xval1,theCompXMax)
                            hXDIV.append(Xbins1[ii])
                            hYDIV.append(0.)
                            continue
                        #   print(his1.name,ii,len(his1.data)) #GDEB
                        #print(his1.name,ii,"XYval1",xval1,yval1,yerr1) #GDEB
                        # GET PC: COMPARE WITH SAME xval
                        if xval1 < his2Inv.xmin or xval1 > his2Inv.xmax :
                            yval1,yerr1 = None,None
                            yval2,yerr2 = None,None
                        else :
                            yval1,yerr1 = his2.GetValueErr(xval1)
                            yval2,yerr2 = his2Inv.GetValueErr(xval1)
                        if yval2 == None : # different limits in the two histograms
                            continue 
                        #print("yval2",yval2,xval1,his2.xmin,his2.xmax) 

                        xval2 = his2.GetClosestX(yval1,xval1)
                    
                        if yval2 != 0 :
                            divValue = yval1/yval2
                        else:
                            divValue = 0.

                        if verbose >= 3 : print(ii,Xbins1[ii],"CHANGE bExp",yval1/yval2,"=",yval1,"/",yval2,"DIV",divValue) 
                        if divValue < theCompYMin :
                            if verbose >= 3 : print("DIV VALUE TOO LOW=",divValue," SET TO ",theCompYMin,"NOT COUNTED IN STATS")
                            hXDIV.append(Xbins1[ii])
                            hYDIV.append(theCompYMin)
                            if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL DIV=",divValue)
                            continue
                        if divValue > theCompYMax:
                            if verbose >= 3 : print("DIV VALUE TOO HIGH=",divValue," SET TO ",theCompYMax,"NOT COUNTED IN STATS")
                            hXDIV.append(Xbins1[ii])
                            hYDIV.append(theCompYMax)
                            if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL DIV=",divValue)
                            continue

                        if verbose >= 3 : print(ii,":theHisFileNames",Xbins1[0]+(ii-0.5)*xStep,"FINAL DIV=",divValue)
                
                        hXDIV.append(Xbins1[ii])
                        hYDIV.append(divValue)
                        print("AX3",divValue,len(hYDIV))
                    ax3.grid()
                    if theCompXMin != -sys.float_info.max and theCompXMax != sys.float_info.max :
                        ax3.set_xlim(theCompXMin,theCompXMax)
                    ax3.set_ylim(theCompYMin,theCompYMax)
                    if verbose >= 3 : print("CHI2ylim",theCompYMin,theCompYMax)
                    print("AX3 hXDIV",len(hXDIV),"hYDIV",len(hYDIV))
                    ax3.plot(hXDIV,hYDIV)
                    ax3.axhline(y=1., color='g', linestyle='--',linewidth=1)
                
        #print(" bPlot2 ",bPlot2,len(hisList))
        if not bPlot2 or len(hisList) == 2 :
            #          if not bPlot2 or len(hisList) != 999 :
            #            if not bPlot2 or len(hisList) >= 1 :
            if verbose >= 1 : 
                print("plt.savefig: hist"+theFilePrefix+hisn+theFileSuffix+".jpg",theFilePrefix,hisn,theFileSuffix)   
            plt.savefig("hist"+theFilePrefix+hisn+theFileSuffix+".jpg")    
            #   plt.savefig("hist"+theFilePrefix+hisn+theFileSuffix+".png",dpi=100,transparent=False)    
        #  plt.clf()

          
                
        #    plt.show()
        #     plt.savefig("histcomparison_"+types+".jpg")
        if verbose >= 3 : print("savefig",hisn+".jpg")
        plt.savefig(hisn+".jpg")    
        #    print('xvalues:',xvalues,' yvalues:',yvalues)    
      
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
if __name__ == "__main__":
    theHisFileNames = []
    theHisFiles = []
    theHisNames = set()
    #print("ERROR 0 " ,bErrors)
    
    ### PROCESS PARAMETERS
    ii = 1
    while ii < len(sys.argv):
        # print(ii,"PARAM",sys.argv[ii])
        if sys.argv[ii] == "-f" :
            bParamTypeFound = True
            # check it is last argument
            for ii2 in range(ii+1,len(sys.argv)) :
                if sys.argv[ii2][0] == "-" :
                    print("!!! ERROR -f MUST BE LAST ARGUMENT")
                    sys.exit()
                else :
                    theHisFileNames.append(sys.argv[ii2])
                    ii = ii+1
                    if(verbose >= 3) : print( ii,"theHisFileNames ",len(theHisFileNames))
            if len(theHisFileNames) == 0 :
                print("!!! ERROR no word after argument ",sys.argv[ii])
                sys.exit()
        elif sys.argv[ii] == "-err" :
            bErrors = int(GetValue(ii+1))
            bParamTypeFound = True
            # print("SET ERROR " ,bErrors)
        elif sys.argv[ii] == "-norm" :
            iNorm = int(GetValue(ii+1))
        elif sys.argv[ii] == "-hisYMin" :
            theHisYMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-hisYMax" :
            theHisYMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-comp" :
            theCompType = sys.argv[ii+1]
           # if theCompType == "GI" :
            #    if theCompYMin == -sys.float_info.max :
             #       theCompYMin = 0.5
              #  if theCompYMax == sys.float_info.max :
               #     theCompYMax = 1.5
            if theCompType == "div" :
                if theCompYMin == -sys.float_info.max :
                    theCompYMin = 0.
                if theCompYMax == sys.float_info.max :
                    theCompYMax = 1.5
            elif theCompType == "GI" :
                theGIndexPC = float(sys.argv[ii+2])/100.
                theGIndexDist = float(sys.argv[ii+3])
                ii = ii+2
                                    
        elif sys.argv[ii] == "-compXMin" :
            theCompXMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compXMax" :
            theCompXMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYMin" :
            theCompYMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYMax" :
            theCompYMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYMinRel" :
            theCompYMinRel = float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYErr" :
            theCompYError = float(sys.argv[ii+1])
        elif sys.argv[ii] == "-YLinePC" :
            theYLinePC = float(sys.argv[ii+1])
            print("SET theYLinePC ",theYLinePC)
        elif sys.argv[ii] == "-gIndex" :
            theGIndexPC = float(sys.argv[ii+1])/100.
            theGIndexDist = float(sys.argv[ii+2])
            ii = ii+1
            # print("GAMMA INDEX ",theGIndexPC, theGIndexDist) #GDEB
            theCompType = "GI"
  #          if theCompYMin == -sys.float_info.max :
  #              theCompYMin = 0.5
  #          if theCompYMax == sys.float_info.max :
  #              theCompYMax = 1.5
        elif sys.argv[ii] == "-giAbs" :
            bGIndexAbs = bool(int(int(sys.argv[ii+1])))
            print("bGIndexAbs",bGIndexAbs,sys.argv[ii+1],int(sys.argv[ii+1]),int(int(sys.argv[ii+1])),bool(int(int(sys.argv[ii+1]))))

        elif sys.argv[ii] == "-fit" :
            theFitType = sys.argv[ii+1]
        elif sys.argv[ii] == "-fitPlot" :
            iFitPlot = int(sys.argv[ii+1])
        elif sys.argv[ii] == "-logY" :
            bLogY = bool(int(sys.argv[ii+1]))
        elif sys.argv[ii] == "-bFWHM" :
            bFWHM = bool(int(sys.argv[ii+1]))
        elif sys.argv[ii] == "-bFWTM" :
            bFWTM = bool(int(sys.argv[ii+1]))
        else :
            print("!! WARNING: ALL ARGUMENTS FROM ",sys.argv[ii]," WILL BE USED AS FILE NAMES !!")
            for ii2 in range(ii,len(sys.argv)) :
                if ii2 != ii and sys.argv[ii2][0] == "-" :
                    print(ii,ii2,sys.argv[ii2],"!!! ERROR -f MUST BE LAST ARGUMENT")
                    sys.exit()
                else :
                    theHisFileNames.append(sys.argv[ii2])
            if(verbose >= 3) : print( ii,"theHisFileNames ",len(theHisFileNames))
        #print("II",ii)
        ii = ii+2
        #print("II",ii)
    
    if theCompType != "" and len(theHisFileNames) != 2 :
        print("!ERROR ONLY TWO FILES MAY BE COMPARED WITH chi2 ")
        sys.exit()

    if theGIndexPC != 0. and len(theHisFileNames) != 2 :
        print("!ERROR ONLY TWO FILES MAY BE COMPARED With gindex",theGIndexPC,len(theHisFileNames))
        sys.exit()
        
    if not bParamTypeFound and len(sys.argv) == 3:
        for ii in range(1,len(sys.argv)) :        
            theHisFileNames.append(sys.argv[ii])

    print("N HisFileNames",len(theHisFileNames),theHisFileNames)

    ### BUILD LIST OF HISTOGRAM NAMES 
    for fileName in theHisFileNames:
        hFile = csvHistoReader(fileName)
        theHisFiles.append(hFile) 
        #print("NHISTOS 1D=",len(hFile.fHistos1D)) #GDEB
        # print(fileName,"theHisFiles",len(theHisFiles),":",fileName) #GDEB
        for his1 in hFile.fHistos1D:
            #                if not his1.name in theHisNames :
    #        if not theHisNames.find(hFile) :
            if(verbose >= 3) : print("ADD HISFILENAME",his1.name,"FROM ",fileName)
            theHisNames.add(his1.name)
    if(verbose >= 3) : print(len(theHisFileNames),"N histo Names ",len(theHisNames),theHisNames)
        
    ### PRINT HISTOGRAMS IN LIST
    printHistosInList(theHisNames,theHisFiles)
    
    sys.exit()
