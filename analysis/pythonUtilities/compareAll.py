import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
from GetColor import GetColor
from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader

verbose = 3
###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()

listofDict=[]
# histype= sys.argv[1] #'Profile_X_at_Braggpeak'
# path='/home/natalia/gamos/GAMOS.6.3.0/Scripts/RADPROTIM_20ene/'
# files=sys.argv[2:]#[path+'analyseSqdose_xLETDose_80.2_1001_1000000_32.csv',path+'analyseSqdose_xLETDose_90.1_1001_1000000_32.csv']
#anadir un modificador para que lo entienda como tipo de variable
def GetValue(ii) :
    if len(sys.argv) <= ii :
        print("!!! ERROR no word after argument ",sys.argv[ii],len(sys.argv)," > ",ii+1)
        sys.exit()
    return float(sys.argv[ii])
        
def CheckArgumentExists(ilen,ii) :
    if ilen == 0 :
        print("!!! ERROR no word after argument ",sys.argv[ii])
        sys.exit()
    
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
if __name__ == "__main__":
    theHisTypes = []
    theHisFileNames = []
    theHisFiles = []
    theHisNames = set()
    bErrors = 0
    bLog = False
    print("ERROR 0 " ,bErrors)
    bNorm = 0
    theFitType = ""
    bFitPlot = 11
    theCompType = ""
    theCompXMin = -sys.float_info.max
    theCompXMax = sys.float_info.max
    theCompYMin = -sys.float_info.max                   
    theCompYMax = sys.float_info.max
    bPlot2 = True
    theCompYError = sys.float_info.max          
    theGIndexPC = 0.
    theGIndexDist = 0.
    theGIndexXMin = -sys.float_info.max
    theGIndexXMax = sys.float_info.max
    theGIndexGIMin = -sys.float_info.max                   
    theGIndexGIMax = sys.float_info.max
    bAbsPCMax = True
    labelFN = ""
    
    ### PROCESS PARAMETERS
    iarg = 0
    bParamTypeFound = False
    for ii in range(1,len(sys.argv)) :
        if sys.argv[ii] == "-t" :
            iarg = iarg+1
            bParamTypeFound = True
            for ii2 in range(ii+1,len(sys.argv)) :
                if sys.argv[ii2][0] == "-" :
                    continue
                else :
                    theHisTypes.append(sys.argv[ii2])
                    ii = ii+1
            CheckArgumentExists(len(theHisTypes),ii)
        elif sys.argv[ii] == "-f" :
 #           print(" ARG -f")
            iarg = iarg+1
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
            CheckArgumentExists(len(theHisFileNames),ii)
            
        elif sys.argv[ii] == "-err" :
            bErrors = int(GetValue(ii+1))
            iarg = iarg+1
            bParamTypeFound = True
            ii = ii+1
            # print("SET ERROR " ,bErrors)
            
        elif sys.argv[ii] == "-n" :
            bNorm = int(GetValue(ii+1))
            iarg = iarg+1
            bParamTypeFound = True
            ++iarg 
            ii = ii+1
        elif sys.argv[ii] == "-log" :
            bLog = bool(int(sys.argv[ii+1]))
        elif sys.argv[ii] == "-fit" :
            theFitType =  sys.argv[ii+1]
        elif sys.argv[ii] == "-bFitPlot" :
            bFitPlot = int(int(sys.argv[ii+1]))
        elif sys.argv[ii] == "-comp" :
            theCompType =  sys.argv[ii+1]
        elif sys.argv[ii] == "-compXMin" :
            theCompXMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compXMax" :
            theCompXMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYMin" :
            theCompYMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYMax" :
            theCompYMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-compYErr" :
            theCompYError = float(sys.argv[ii+1])
        elif sys.argv[ii] == "-gindex" :
            theGIndexPC = float(sys.argv[ii+1])/100.
            theGIndexDist = float(sys.argv[ii+2])
            ii = ii+1
#            print("GAMMA INDEX ",theGIndexPC, theGIndexDist)
        elif sys.argv[ii] == "-giindex" :
            theGIndexPC = float(sys.argv[ii+1])/100.
            theGIndexDist = float(sys.argv[ii+2])
            ii = ii+1
#            print("GAMMA INDEX ",theGIndexPC, theGIndexDist)
        elif sys.argv[ii] == "-giXMin" :
            theGIndexXMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-giXMax" :
            theGIndexXMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-giMin" :
            theGIndexGIMin =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-giMax" :
            theGIndexGIMax =  float(sys.argv[ii+1])
        elif sys.argv[ii] == "-giAbsPCMax" :
            bAbsPCMax = bool(int(sys.argv[ii+1]))
#            print("bAbsPCMax",bAbsPCMax)
        elif sys.argv[ii] == "-fLabels" :
            labelFN = sys.argv[ii+1]
   
    if theCompType != "" and len(theHisFileNames) != 2 :
        print("!ERROR ONLY TWO FILES MAY BE COMPARED WITH chi2 ")
        sys.exit()

    if theGIndexPC != 0. and len(theHisFileNames) != 2 :
        print("!ERROR ONLY TWO FILES MAY BE COMPARED With gindex",theGIndexPC,len(theHisFileNames))
        sys.exit()
        
    if not bParamTypeFound and len(sys.argv) == 3:
        for ii in range(1,len(sys.argv)) :        
            theHisFileNames.append(sys.argv[ii])

    labels = []
    if labelFN != "" :
        finLab = open(labelFN)
        lines = finLab.readlines()
        for line in lines :
            line = line.rstrip()
            labels.append(line)
            
    print("N HisFileNames",len(theHisFileNames))

    ### BUILD LIST OF HISTOGRAM NAMES
    for fileName in theHisFileNames:
        hFile = csvHistoReader(fileName)
        theHisFiles.append(hFile)
        #print("NHISTOS 1D=",len(hFile.fHistos1D))
        for his1 in hFile.fHistos1D:
            #                if not his1.name in theHisNames :
    #        if not theHisNames.find(hFile) :
            theHisNames.add(his1.name)
    if(verbose >= 3) : print(len(theHisFileNames),"N histo Names ",len(theHisNames),theHisNames)
        
    chi2Total = 0
    for hisn in theHisNames :
        bFound = False
        if(verbose >= 3) : print("start loop",hisn,len(theHisFiles))
        ## LOOK FOR HISTO NAME IN EACH FILE
        iih = 0
        hisList = []
        ax1 = plt.subplot2grid((4,1),(0,0),rowspan=3)
        for hfile in theHisFiles :
            lcolor = GetColor(iih)
            print(iih,hfile.fileName,"GETCOLOR",lcolor) #GDEB
            if(verbose >= 3) : print("loop N histo in file",len(hfile.fHistos1D))
            for his1 in hfile.fHistos1D :
                if(verbose >= 4) : print("CHECK name ",hisn,"=?=",his1.name)
                if his1.name == hisn :
                    hisList.append(his1)
                    if verbose >= 2 : print(len(hisList),"HISTO FOUND ",his1.name)
                    if verbose >= 1 and len(hisList) == 2 : print(len(hisList),"2 HISTOS FOUND ",his1.name)
                    # check equal 
                    if not bFound :
                        histo1st = his1
                        bFound = True
                    if bNorm == 1 :
                        his1.norm(bNorm)
                    elif bNorm > 1 :
                        if iih == 1142 :
                            his1.norm(bNorm*his1.nbin*27.16966672084123/21.859126777493724)
                        elif iih == 1070:
                            his1.norm(bNorm*his1.nbin*19.18905872432657/13.3920292089707)
                            print("MAX2",his1.name,iih)
                        elif iih == 1228 :
                            his1.norm(bNorm*his1.nbin*19.78495393492635/17.765690513993448/19.784953934926353*19.55726679615669)
                        else :
                            his1.norm(bNorm*his1.nbin)
                    print(iih,his1.name, "MAX",max(his1.data))
                    hX = his1.Xbins()
                    hY = his1.data
                    #    print(" HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",hX)
                    xStep = his1.Xstep()
                    plt.xlim(float(his1.xmin),float(his1.xmax))
                    plt.xlabel(his1.name)
                    ax1.grid()
                    if bLog : 
                        plt.yscale('log')
                    if bErrors == 1 :
                        ax1.bar(hX,hY, color='None', edgecolor=lcolor, yerr=his1.dataErr, width=xStep)
                    elif bErrors == 0 :
                        if(verbose >= -4) : print("HX ",len(hX)," HY ",len(hY))
                        if(verbose >= 4) : print("HX ",hX," HY ",hY)
                        #   ax1.plot(hX,hY, color=lcolor)   #'black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                        ax1.set_xlabel("transversal position (mm)")
                        ax1.set_ylabel("") #LET (keV/($\mu)m)")
                        if labelFN == "" :
                            ax1.plot(hX,hY, color=lcolor) 
                        else :
                            ax1.plot(hX,hY, color=lcolor,label=labels[iih]) 
                            ax1.legend()

                    if theFitType != "" :
                        if theFitType != "Gaussian" :
                            print("!!! ERROR: only fit Gaussian is currently supported, not ",theFitType)
                            sys.exit()
                        param,cov = his1.GaussianFit(iih, bFitPlot)
                        if param[0] != 220 :
                            print("GAUSSIAN_FIT PARAM ",hfile.fileName,his1.name,param[0],param[1],param[2])
                            if verbose >= 2 :
                                print("GAUSSIAN_FIT COV ",hfile.fileName,his1.name,cov)
                        else :
                            print("GAUSSIAN_FIT PARAM ",hfile.fileName,his1.name,param)
                            if verbose >= 2 :
                                print("GAUSSIAN_FIT COV ",hfile.fileName,his1.name,cov)              
                            
            iih += 1
        if theCompType == "chi2" and len(hisList) == 2:
            if verbose >= 3 : print(hisn,"LOOP HISLIST ",hisList[0].name,len(hisList[0].data))#GDEB
            chi2 = 0
            dof = 0
            his1 = hisList[0]
            his2 = hisList[1]
            Xbins1 = his1.Xbins()
            hXComp = []
            hYComp = []
            hYErrComp = []
            if verbose >= 3 : print("len(Xbins1",len(Xbins1))#GDEB
            for ii in range(len(Xbins1)) :
                xval1 = Xbins1[ii]
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
                        if yval1 < theCompYMin or yval1 > theCompYMax:
                            if verbose >= 3 : print("COMP NOT USED xyal1 out",ii,theCompYMin,yval1,theCompYMax)
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
                hYComp.append(chi2_1)
                chi2 += chi2_1
                if verbose >= 3 : print(his1.name,"chi2",chi2,xval1,yval1,yval2,yerr1,yerr2,yerr)
            pval = 0.
            chi2 /= dof
            if verbose >= 1 : print(his1.name,"CHI2N",chi2," dof",dof,chi2*dof)
            chi2Total += chi2
            ax1.legend()
            xPos = his1.xmax*(0.5)
            yPos = his1.maximumY()*(0.7-0.2*int(theFitType!=""))
            ax1.text(xPos,yPos,"chi2= "+"{:.4f}".format(chi2), color=lcolor)
            yPos = his1.maximumY()*(0.65-0.2*int(theFitType!=""))
            ax1.text(xPos,yPos,"p-val= "+"{:.4f}".format(pval), color=lcolor)
            #t            ax1.draw()
            
            ax2 = plt.subplot2grid((4,1),(3,0),rowspan=1)
            #            ax2.bar(hXComp,hYComp, color='None', edgecolor='black', yerr=hYErrComp) #, width=xStep)
            if bLog : 
                plt.yscale('log')
            ax2.plot(hXComp,hYComp)
            #t            ax2.draw()

        if verbose >= 3 : print("test GAMMA INDEX ",theGIndexPC, theGIndexDist,len(hisList))
        if theGIndexPC != 0. and len(hisList) == 2:
            if verbose >= 3 : print(hisn,"agi LOOP HISLIST ",hisList[0].name,len(hisList[0].data))
            nGImore1 = 0
            nGI = 0
            GIAver = 0.
            his1 = hisList[0]
            his2 = hisList[1]
            Xbins1 = his1.Xbins()
            hXGI = []
            hYGI = []
            hYErrGI = []
            xStep = his1.Xstep()
            ## GET how many around to llok
            yMax1 = max(his1.data)
            for ii in range(len(Xbins1)) :
                xval1 = Xbins1[ii]
                if xval1 < theGIndexXMin or xval1 > theGIndexXMax:
                    if verbose >= 3 : print("GAMMA INDEX NOT USED xval1 out of limits",ii,theGIndexXMin,xval1,theGIndexXMax)
                    hXGI.append(Xbins1[ii])
                    hYGI.append(0.)
                    continue
             #   print(his1.name,ii,len(his1.data)) #GDEB
                yval1 = his1.data[ii]
                yerr1 = his1.dataErr[ii]
                # GET PC: COMPARE WITH SAME xval
                if xval1 < his2.xmin or xval1 > his2.xmax :
                    yval2,yerr2 = None,None
                else :
                    yval2,yerr2 = his2.GetValueErr(xval1)
                if yval2 == None : # different limits in the two histograms
                    continue 
                #print("yval2",yval2,xval1,his2.xmin,his2.xmax) 
                yaver = (yval1+yval2)/2.
                if bAbsPCMax :
                    PCCurrent = abs((yval2-yval1)/yMax1)/theGIndexPC
                    if verbose >= 3 : print(ii,":",xval1,Xbins1[0],(ii-0.5)*xStep,"GI: ",xval1," AbsMaxPCCurrent",PCCurrent,yval2,"-",yval1,"=",yval2-yval1,"/",yMax1,"/",theGIndexPC)
                else :
                    if yaver != 0 : 
                        PCCurrent = abs((yval2-yval1)/yaver)/theGIndexPC
                    else :
                        PCCurrent = 0.                        
                    if verbose >= 3 : print(ii,":",Xbins1[0]+(ii-0.5)*xStep,"GI: ",xval1," PCCurrent",PCCurrent,yval2,"-",yval1,"=",yval2-yval1,"/",yaver,"/",theGIndexPC)
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
                        for ix2 in range(ii-iDist,ii+iDist+1) :
                            if ix2 < 0 : continue
                            if ix2 >= his1.nbin : continue
                            yval2,yerr2 = his2.GetValueErr(Xbins1[ix2])
                            if yval2 == None : # different limits in the two histograms
                                continue
                            #PCCurrent = abs((yval2-yval1)/yMax1)/theGIndexPC
                            yaver = (yval1+yval2)/2.
                            if bAbsPCMax :
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
                    
                    if gammaMin < theGIndexGIMin or gammaMin > theGIndexGIMax:
                        if verbose >= 3 : print("GAMMA INDEX NOT USED gamma out of limits",ii,theGIndexGIMin,gammaMin,theGIndexGIMax)
                        hXGI.append(Xbins1[ii])
                        hYGI.append(0.)
                        continue
                else :
                    xval2 = his2.GetClosestX(yval1,xval1)
                    gamma1 = np.abs(xval2-xval1)
                    gammaMin = min(gammaMin,gamma1)
                    if verbose >= 3 : print(ii,"GI: PCCurrent/dist1",gamma1,PCCurrent,xval2,"-",xval1)

                if verbose >= 3 : print(ii,":",Xbins1[0]+(ii-0.5)*xStep,"FINAL GI=",gammaMin)                
                hXGI.append(Xbins1[ii])
                hYGI.append(gammaMin)
                GIAver += gammaMin
                nGI += 1
                if gammaMin > 1. : nGImore1 += 1
                if verbose >= 2 : print(ii,his1.name,"GI",gammaMin,xval1,yval1,yval2,PCCurrent)
            GIAver /= nGI

            ax1.legend()
            xPos = his1.xmax*(0.3)
            yPos = his1.maximumY()*(0.65-0.2*int(theFitType!=""))
            ax1.text(xPos,yPos,"GI= "+"{:.4f}".format(GIAver), color=lcolor)
            yPos = his1.maximumY()*(0.6-0.25*int(theFitType!=""))
            ax1.text(xPos,yPos,"nGI>1= "+str(nGImore1)+"/"+str(nGI), color=lcolor)
            #t            ax1.draw()
           
            ax2 = plt.subplot2grid((4,1),(3,0),rowspan=1)
            #            ax2.bar(hXGI,hYGI, color='None', edgecolor='black') #, width=xStep)            
            if bLog : 
                plt.yscale('log')
            ax2.plot(hXGI,hYGI)
            #t            ax2.draw()

        if bFound :
#            if not bPlot2 or len(hisList) == 2 :
            if not bPlot2 or len(hisList) != 999 :
#            if not bPlot2 or len(hisList) >= 1 :
                print("SAVING hist"+hisn+".jpg")    
                plt.savefig("hist"+hisn+".jpg")    
                plt.clf()

    if theCompType == "chi2" and verbose >= 1 : print("CHI2_TOTAL",chi2Total)
                
#    plt.show()
#    plt.savefig("histcomparison_"+types+".jpg")    
#    plt.savefig("hist1.jpg")    
#    print('xvalues:',xvalues,' yvalues:',yvalues)     
sys.exit()
    

