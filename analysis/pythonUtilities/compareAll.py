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

verbose = 3
###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()

listofDict=[]
# histype= sys.argv[1] #'Profile_X_at_Braggpeak'
# path='/home/natalia/gamos/GAMOS.6.3.0/Scripts/RADPROTIM_20ene/'
# files=sys.argv[2:]#[path+'analyseSqdose_xLETDose_80.2_1001_1000000_32.csv',path+'analyseSqdose_xLETDose_90.1_1001_1000000_32.csv']
#añadir un modificador para que lo entienda como tipo de variable
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
    bNorm = 0
    theFitType = ""
    bFitPlot = True
    theCompType = ""
    theCompXMin = -sys.float_info.max
    theCompXMax = sys.float_info.max
    theCompYMin = -sys.float_info.max                   
    theCompYMax = sys.float_info.max
    bPlot2 = True
    theCompYError = sys.float_info.max          
    
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
            
        elif sys.argv[ii] == "-e" :
            bErrors = int(GetValue(ii+1))
            iarg = iarg+1
            bParamTypeFound = True
            ii = ii+1
            
        elif sys.argv[ii] == "-n" :
            bNorm = int(GetValue(ii+1))
            iarg = iarg+1
            bParamTypeFound = True
            ++iarg 
            ii = ii+1

        elif sys.argv[ii] == "-fit" :
            theFitType =  sys.argv[ii+1]
        elif sys.argv[ii] == "-bFitPlot" :
            bFitPlot = bool(int(sys.argv[ii+1]))
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
    
    if theCompType != "" and len(theHisFileNames) != 2 :
        print("!ERROR ONLY TWO FILES MAY BE COMPARED ")
        sys.exit()
        
    if not bParamTypeFound and len(sys.argv) == 3:
        for ii in range(1,len(sys.argv)) :        
            theHisFileNames.append(sys.argv[ii])

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
        if(verbose >= 3) : print("start loop",hisn)
        ## LOOK FOR HISTO NAME IN EACH FILE
        ii = 0
        hisList = []
        ax1 = plt.subplot2grid((4,1),(0,0),rowspan=3)
        for hfile in theHisFiles :
            for his1 in hfile.fHistos1D :
                if(verbose >= 4) : print("CHECK name ",his1.name,"=?=",hisn)
                if his1.name == hisn :
                    hisList.append(his1)
                    print("ADDED to HISLIST ",his1.name,len(hisList[len(hisList)-1].data))#GDEB
                    if(verbose >= 1) : print("HISTO FOUND ",his1.name)
                    # check equal 
                    if not bFound :
                        histo1st = his1
                        bFound = True
                    if bNorm > 0 :
                        his1.norm(bNorm)
                    hX = his1.Xbins()
                    hY = his1.data
                    #    print(" HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",hX)
                    xStep = his1.Xstep(his1)
                    plt.xlim(float(his1.xmin),float(his1.xmax))
                    plt.xlabel(his1.name)
                    ax1.grid()
                    if bErrors == 1 :
                        ax1.bar(hX,hY, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep)
                    elif bErrors == 0 :
                        if(verbose >= 4) : print("HX ",hX," HY ",hY)
                        ax1.plot(hX,hY) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    if theFitType != "" :
                        if theFitType != "Gaussian" :
                            print("!!! ERROR: only fit Gaussian is currently supported, not ",theFitType)
                            sys.exit()
                        param,cov = his1.GaussianFit(ii, bFitPlot)
                        if verbose >= 0 :
                            print("GAUSSIAN_FIT PARAM ",hfile.theFileName,his1.name,param)
                            print("GAUSSIAN_FIT COV ",hfile.theFileName,his1.name,cov)
            
            ii += 1
            
        if theCompType == "chi2" and len(hisList) == 2:
            print(hisn,"LOOP HISLIST ",hisList[0].name,len(hisList[0].data))#GDEB
            chi2 = 0
            dof = 0
            his1 = hisList[0]
            his2 = hisList[1]
            Xbins1 = his1.Xbins()
            hXComp = []
            hYComp = []
            hYErrComp = []
            print("len(Xbins1",len(Xbins1))#GDEB
            for ii in range(len(Xbins1)) :
                xval1 = Xbins1[ii]
             #   print(his1.name,ii,len(his1.data)) #GDEB
                yval1 = his1.data[ii]
                yerr1 = his1.dataErr[ii]
                yval2,yerr2 = his2.GetValueErr(xval1)
                hXComp.append(Xbins1[ii])
                bContinue = False
                if yval2 == None :
                    print("COMP NOT USED yval2",ii,yval2)
                    bContinue = True
                else :
                    #                 print("COMP",xval1,theCompXMin,xval1,theCompXMax) #GDEB
                    if xval1 < theCompXMin or xval1 > theCompXMax:
                        print("COMP NOT USED xval1 out",ii,theCompXMin,xval1,theCompXMax)
                        bContinue = True
                    else :
                        if yval1 < theCompYMin or yval1 > theCompYMax:
                            print("COMP NOT USED xyal1 out",ii,theCompYMin,yval1,theCompYMax)
                            bContinue = True
                        else : 
                            if theCompYError == sys.float_info.max and yerr1 + yerr2 == 0. :
                                print("COMP NOT USED yerr",ii,yerr1,yerr2)
                                bContinue = True
                print("BbContinue",bContinue)
                if bContinue:
                    hYComp.append(0.)
                    hYErrComp.append(0.)
                    continue
    
                if yval1 != 0. and yval2 != 0. :
                    dof += 1                    
                print("COMP",yval1,yval2,dof)#GDEB
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
                if verbose >= -4 : print(his1.name,"chi2",chi2,xval1,yval1,yval2,yerr1,yerr2,yerr)
            pval = 0.
            chi2 /= dof
            if verbose >= 1 : print(his1.name,"CHI2N",chi2," dof",dof,chi2*dof)
            chi2Total += chi2
            ax1.legend()
            xPos = his1.xmax*(0.5)
            yPos = his1.maximumY()*(0.7-0.2*int(theFitType!=""))
            lcolor = 'black'
            ax1.text(xPos,yPos,"chi2= "+"{:.4f}".format(chi2), color=lcolor)
            yPos = his1.maximumY()*(0.65-0.2*int(theFitType!=""))
            ax1.text(xPos,yPos,"p-val= "+"{:.4f}".format(pval), color=lcolor)
            #t            ax1.draw()
            
            ax2 = plt.subplot2grid((4,1),(3,0),rowspan=1)
            #            ax2.bar(hXComp,hYComp, color='None', edgecolor='black', yerr=hYErrComp) #, width=xStep)
            ax2.plot(hXComp,hYComp)
            #t            ax2.draw()
            
        if bFound :
            if not bPlot2 or len(hisList) == 2 :
#            if not bPlot2 or len(hisList) >= 1 :
                plt.savefig("hist"+hisn+".jpg")    
                plt.clf()

    if verbose >= 1 : print("CHI2_TOTAL",chi2Total)
                
#    plt.show()
#    plt.savefig("histcomparison_"+types+".jpg")    
#    plt.savefig("hist1.jpg")    
#    print('xvalues:',xvalues,' yvalues:',yvalues)     
sys.exit()
    
