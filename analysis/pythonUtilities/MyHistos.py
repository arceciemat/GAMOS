from collections.abc import MutableMapping
import numpy as np
import math
import sys
from functools import reduce
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import matplotlib
from scipy.interpolate import interp1d, griddata,bisplrep,interp2d

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
def CleanName(name):
    newName = name.replace('"','')
    newName = name.replace('/','-')
    return newName

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
def sum2(val1,err1,val2,err2) :

    newData = []
    if err1 == 0. or err2 == 0.:
        newData.append((val1+val2)/2.)
        newData.append(math.sqrt(err1*err1+err2*err2))
    else :
        den = 1./(err1*err1)+1./(err2*err2)
        newData.append( (val1/(err1*err1)+val2/(err2*err2))/den ) # average value
        newData.append( math.sqrt(1./den) ) # average err
    return newData

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
class Histo1D(MutableMapping):
    type = "1D"
    name = ""
    nbin = int(0)
    xmin = float(0)
    xmax = float(0)
    under = 0
    underErr = 0
    data = np.zeros([1])
    dataErr = np.zeros([1])
    over = 0
    overErr = 0  
    nent = 0
    mean = 0
    meanErr = 0
    RMS = 0
    RMSErr = 0
    nFiles = 1 # count if histogram is the sum of several histograms from different files
    #    def __init__(self, *args, **kwargs):
        #args -- tuple of anonymous arguments
        #kwargs -- dictionary of named arguments

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __init__(self, args=[]):
        if len(args) == 0 :
            return
        #        print("MyHistos args",args," N=",len((args)))#GDEB
        self.type = args[0]
        self.name = CleanName(args[1])
        self.nbin = int(args[2])
        self.data = np.zeros([self.nbin])
        self.dataErr = np.zeros([self.nbin])
        if len(args) != 2*self.nbin+14 :
            print("!! ERROR: Histo1D ",self.name," should have nbin ",2*self.nbin+14," and it has ",len(args))
            sys.exit()
        self.xmin = float(args[3])
        self.xmax = float(args[4])
        self.under = float(args[5])
        self.underErr = float(args[6])
        for id in range(0,self.nbin) : # LOOP TO DATA
            self.data[id] = float(args[7+2*id])
            self.dataErr[id] = float(args[7+2*id+1])
        self.over = float(args[2*self.nbin+7])
        self.overErr = float(args[2*self.nbin+8])
        self.nent = float(args[2*self.nbin+9])
        self.mean = float(args[2*self.nbin+10])
        self.meanErr = float(args[2*self.nbin+11])
        self.RMS = float(args[2*self.nbin+12])
        self.RMSErr = float(args[2*self.nbin+13])
        
        #        self.num_holes = kwargs.get('num_holes',random_holes())

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def FillFromLine(self,line):
        self.type = "1D"
        self.name = ""
        self.nbin = len(line)
        step = (line[len(line)-1][0]-line[0][0])/(self.nbin-1)
        self.xmin = line[0][0]-step/2.
        self.xmax = line[len(line)-1][0]+step/2.
#        print("STEP ",step,self.xmin,self.xmax) #GDEB
        self.under = 0.
        self.underErr = 0.
        self.data = np.zeros([self.nbin])
        self.dataErr = np.zeros([self.nbin])
        for id in range(0,self.nbin) : # LOOP TO DATA
            self.data[id] = line[id][1]
            self.dataErr[id] = line[id][2]
            # print(len(line),self.nbin,"FILL ",id," ",self.data)#GDEB
        self.over = 0. 
        self.overErr = 0.
        self.nent = self.nbin
        self.mean = 0.
        self.meanErr = 0.
        self.RMS = 0.
        self.RMSErr = 0.
        
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def FillFromArrays(self,data,dataErr,minX,maxX) :
        self.nbin = len(data)
        sumX = np.zeros(nX)
        sumY = np.zeros(nY)
        sumXerr2 = np.zeros(nX)
        sumYerr2 = np.zeros(nY)
        xbins = self.Xbins()
        nent = 0
        sumx = 0
        sumx2 = 0
        sumxerr = 0
        sumxerr2 = 0
        self.data = np.zeros([self.nbin])
        self.dataErr = np.zeros([self.nbin])
        for ix in range(self.nbin) :
            val = data[ix]
            err = dataErr[ix]
            self.data[id] = val
            self.dataErr[id] = err
            sumx += val
            sumx2 += val*val
            sumxerr += err
            sumxerr2 += err*err
            self.nent += val

        if sumx != 0 :
            self.mean = sumx/self.nent
            self.meanErr = np.sqrt(sumxerr2)/sumx
            #    self.xRMS = np.sqrt(sumx2/sumX.sum()-sumx*sumx)
        else :
            print("!!! WARNING: empty histogram ",self.name," range_X ",ix,"  out of",range(nX))
        self.RMS = 0.
        self.RMSErr = 0.

        self.xunder = 0.
        self.xunderErr = 0.
        self.over = 0.
        self.overErr = 0.

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def copy(self, his1orig):
        self.data = his1orig.data
        self.dataErr = his1orig.dataErr
        self.type = his1orig.type
        self.name = his1orig.name
        self.nbin = his1orig.nbin
        self.xmin = his1orig.xmin
        self.xmax = his1orig.xmax
        self.under = his1orig.under
        self.underErr = his1orig.underErr
        self.over = his1orig.over
        self.overErr = his1orig.overErr
        self.nent = his1orig.nent
        self.mean = his1orig.mean
        self.meanErr = his1orig.meanErr
        self.RMS = his1orig.RMS
        self.RMSErr = his1orig.RMSErr
        #        self.num_holes = kwargs.get('num_holes',random_holes())

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    ### NORM TO INTEGRAL
    def norm(self,normValue):
        dataSum = reduce(lambda nn,mm: nn+mm,self.data)
#        dataSum2 = sum(self.data)
#        print("DATASUM",dataSum,dataSum2)
        if dataSum != 0 :
            factor = normValue/dataSum
        else :
            factor = 1.
        print(self.name,"NORM=",normValue,"factor=", factor,"dataSum=",dataSum)
        self.data *= factor
        self.dataErr *= factor

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    ### NORM TO VALUE AT A POINT
    def normToValue(self,normValue, normX):
        xstep = self.Xstep()
        ix = int((normX - self.xmin)/xstep)
        dataIx = self.data[ix]
        if dataIx == 0. :
            return
        # print("normToValue",normValue,normX," ix= ",ix," dataIx= ",dataIx)  # GDEB
        self.data *= normValue/dataIx
        self.dataErr *= normValue/dataIx
        #t        numpy.prod(self.data)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __setitem__(self, k, v):
        setattr(self, k, v)
    def __getitem__(self, k):
        getattr(self, k)
    def __len__(self):
        return 2
    def __delitem__(self, k):
        self[k] = None
    def __iter__(self):
        yield self.name
        yield self.nbin
        yield self.xmin
        yield self.xmax
        yield self.under
        yield self.data
        yield self.over
        yield self.underErr
        yield self.dataErr
        yield self.overErr
        yield self.nent
        yield self.mean
        yield self.meanErr
        yield self.RMS
        yield self.RMSErr
    def __str__(self):
        return ("Histo1D: "+self.type+" "+self.name+" "+str(self.nbin)
                +" "+str(self.xmin)+" "+str(self.xmax)
                +" U="+str(self.under)+"+-"+str(self.underErr)
                +" O="+str(self.over)+"+-"+str(self.overErr)
                +" NENT="+str(self.nent)
                +" MEAN="+str(self.mean)+"+-"+str(self.meanErr)
                +" RMS="+str(self.RMS)+"+-"+str(self.RMSErr)
#                +" "+str(self.data)
#                +" "+str(self.dataErr)
               )
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Xstep(self):
        xStep = (float(self.xmax)-float(self.xmin))/int(self.nbin)
        return xStep
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Xbins(self):
        Xbins = []
        xStep = (float(self.xmax)-float(self.xmin))/int(self.nbin)
        for ii in range(0,int(self.nbin)):
            Xbins.append(float(self.xmin)+(0.5+ii)*xStep)
            #print(ii,"XBINS",float(self.xmin)+(0.5+ii)*xStep,float(self.xmin),"+",(0.5+ii),"*",xStep)
        return Xbins
    
   #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetBinCenter(self,ix):
        return self.xmin + self.Xstep()*(ix+0.5)
    
   #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetBinCentre(self,ix):
        return self.xmin + self.Xstep()*(ix+0.5)
    
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CheckEqual(self,obj):
#        if self.name != obj.name :
#            print("!!! WARNING: two histograms with different name ",
#                  self.name," !=? ", obj.name)

        if self.nbin != obj.nbin or self.xmin != obj.xmin or self.xmax != obj.xmax :
            print("!!! ERROR: two histograms with same name do not have same data ",
                  self.name,",NBIN:",self.nbin," !=? ", obj.nbin,
                  "XMIN:",self.xmin," !=? ",obj.xmin,
                  "XMAX:",self.xmax," !=? ",obj.xmax)
            return False
            # sys.exit()
        return True
            
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Add(self,obj,bCheckEqual=True):

        if bCheckEqual and not self.CheckEqual(obj) :
            return self

        newData = sum2(self.under,self.underErr,obj.under,obj.underErr)
        self.under = self.under+obj.under
        self.underErr = np.sqrt(pow(self.underErr,2)+pow(obj.underErr,2))
        self.data += obj.data
        self.dataErr = np.sqrt(np.power(self.dataErr,2)+np.power(obj.dataErr,2))
        self.over = self.over+obj.over
        self.overErr = np.sqrt(pow(self.overErr,2)+pow(obj.overErr,2))
        self.nent += obj.nent

        newData = sum2(self.mean,self.meanErr,obj.mean,obj.meanErr)
  #        print("HISTO1 Add mean",newData[0],newData[1],"<-",self.mean,self.meanErr)
        self.mean = newData[0]
        self.meanErr = newData[1]

        newData = sum2(self.RMS,self.RMSErr,obj.RMS,obj.RMSErr)
        self.RMS = newData[0]
        self.RMSErr = newData[1]

        return self

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Average(self,obj,bCheckEqual=True):

        if bCheckEqual and not self.CheckEqual(obj) :
            return self
        
        newData = sum2(self.under,self.underErr,obj.under,obj.underErr)
        self.under = newData[0]
        self.underErr = newData[1]
        # !!! MASK != 0
        weiself = np.zeros(self.nbin)
        weiobj = np.zeros(self.nbin)
        for ii in range(0,self.nbin) :
            newData = sum2(self.data[ii],self.dataErr[ii],obj.data[ii],obj.dataErr[ii])
            self.data[ii] = newData[0]
            self.dataErr[ii] = newData[1]
        # gives error if bin error is 0     weidat = 1. / np.power(self.dataErr,2) 
        # gives error if bin error is 0     weiobj = 1. / np.power(obj.dataErr,2)
         # gives error if bin error is 0    self.data = (self.data * weidat + obj.data * weiobj ) / (weidat+weiobj)
        # gives error if bin error is 0     self.dataErr = np.sqrt(1. / (weidat+weiobj) )
        newData = sum2(self.over,self.overErr,obj.over,obj.overErr)
        self.over = newData[0]
        self.overErr = newData[1]
        self.nent += obj.nent

        if not math.isnan(self.meanErr) and not math.isnan(obj.meanErr) :
            newData = sum2(self.mean,self.meanErr,obj.mean,obj.meanErr)
            #print("HISTO1 Average mean",newData[0],newData[1],"<-",self.mean,self.meanErr)
            self.mean = newData[0]
            self.meanErr = newData[1]
        else :
            self.mean = (self.mean+obj.mean)/2.
            self.meanErr = 0.
        newData = sum2(self.RMS,self.RMSErr,obj.RMS,obj.RMSErr)
        self.RMS = newData[0]
        self.RMSErr = newData[1]

        return self

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def DisplaceX(self,disp,orderInterp = 1):
        xstep = self.Xstep()
        dispInBin = disp%xstep
        ixLeft = int(disp/xstep)+int((np.sign(disp)-1)/2)
        ixRight = ixLeft+1
        # print("DisplaceX Disp",disp," inbin",dispInBin," ix",ixLeft,"<>",ixRight," xstep",xstep, "   ",disp/xstep,int(disp/xstep),int((np.sign(disp)-1)/2)) #GDEB
        nData = len(self.data)
        dataNew = []
        for ix in range(0,nData) :
            ix1 = max(0,ix+ixLeft)
            ix1 = min(nData-1,ix1)
            dataPre = self.data[ix1]
            ix2 = max(0,ix+ixRight)
            ix2 = min(nData-1,ix2)
            dataPost = self.data[ix2]
            dataNew1 = dataPre*(xstep-dispInBin)/xstep+dataPost*(dispInBin)/xstep
            dataNew.append(dataNew1)
            # print(" DisplaceX ",ix,ix1,ix2,dataNew1,dataPre,dataPost) #GDEB

        for ix in range(0,nData) :
            self.data[ix] = dataNew[ix]

        return self
            
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def AddDisp(self,obj):
        
        newData = sum2(self.under,self.underErr,obj.under,obj.underErr)
        self.under = newData[0]
        self.underErr = newData[1]
        hXold = self.Xbins()
        hXnew = obj.Xbins()
        for ix in range(0,len(hXold)) :
            hx = hXold[ix]
            if hx in hXnew :
                ix2 = hXnew.index(hx)
                newData = sum2(self.data[ix],self.dataErr[ix],obj.data[ix2],obj.dataErr[ix2])
            # if self.data[ix] != 0. : print(ix,ix2,"AddDisp BEF",self.data[ix],obj.data[ix2]) //GDEB
            self.data[ix] = newData[0]
            self.dataErr[ix] = newData[1]
            # if self.data[ix] != 0. : print(ix,"AddDisp AFT",self.data[ix]) # GDEB
        newData = sum2(self.over,self.overErr,obj.over,obj.overErr)
        self.over = newData[0]
        self.overErr = newData[1]
        self.nent += obj.nent

        newData = sum2(self.mean,self.meanErr,obj.mean,obj.meanErr)
        self.mean = newData[0]
        self.meanErr = newData[1]

        newData = sum2(self.RMS,self.RMSErr,obj.RMS,obj.RMSErr)
        self.RMS = newData[0]
        self.RMSErr = newData[1]

        return self

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Mult(self,factor):

        self.data *= factor
        self.dataErr *= factor
        self.mean *= factor
        self.meanErr *= factor
        self.RMS *= factor
        self.RMSErr *= factor

        return self

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def findstr(self,name,element):
        result=[]
        offset=-1
        while True:
            try:
                offset = name.index(element,offset+1)
            except ValueError:
                return result
            result.append(offset)
            
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Write(self,fOut):
        fOut.write('"'+self.type+'"')
        fOut.write(',"'+self.name+'"')
        fOut.write(","+str(self.nbin))
        fOut.write(","+str(self.xmin))
        fOut.write(","+str(self.xmax))
        fOut.write(","+str(self.under))
        fOut.write(","+str(self.underErr))
        for id in range(0,self.nbin) : 
            fOut.write(","+str(self.data[id])+","+str(self.dataErr[id]))
        fOut.write(","+str(self.over))
        fOut.write(","+str(self.overErr))
        fOut.write(","+str(self.nent))
        fOut.write(","+str(self.mean))
        fOut.write(","+str(self.meanErr))
        fOut.write(","+str(self.RMS))
        fOut.write(","+str(self.RMSErr))
        fOut.write("\n")

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Print(self):
        print('"'+self.type+'"')
        print(',"'+self.name+'"')
        print(","+str(self.nbin))
        print(","+str(self.xmin))
        print(","+str(self.xmax))
        print(","+str(self.under))
        print(","+str(self.underErr))
        print(","+str(self.over))
        print(","+str(self.overErr))
        print(","+str(self.nent))
        print(","+str(self.mean))
        print(","+str(self.meanErr))
        print(","+str(self.RMS))
        print(","+str(self.RMSErr))
        print("\n")

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GaussianFit(self, idHis, bFitPlot) :
#        print("GaussianFit bFitPlot ",bFitPlot)
        hx = self.Xbins()
        hy = self.data
        hxysum = 0
        hxsum = 0
        hysum = 0
        for ii in range(0,len(hy)):
            hxysum += hx[ii]*hy[ii]
            hxsum += hx[ii]
            hysum += hy[ii]
            #            print("hxysum",hxysum,hx[ii],"*",hy[ii]) # GDEB
        if hysum != 0. :
            mean = hxysum/hysum
        else :
            param = [0.,0.,0.]
            covariance = [ [0.,0.,0.], [0.,0.,0.], [0.,0.,0.]]
            return param,covariance
        #        mean = sum(hx * hy) / sum(hy)
        hxysum = 0
        for ii in range(0,len(hy)):
            hxysum += hy[ii]*(hx[ii]-mean)**2
#            print("2hxysum",hxysum,hy[ii],"*(",hx[ii],"-",mean) #GDEB
        sigma = np.sqrt(hxysum / hysum)
        #         A, x0, sigma = self.gauss_fit(hx, hy)
        #        sigma = np.sqrt(sum(hy * (hx - mean) ** 2) / sum(hy))
        #        print("CALL curve_fit gauss",hy)
        try:
            param, covariance = curve_fit(self.gauss, hx, hy, p0=[max(hy), mean, sigma])
        except RuntimeError:
            print("Error - Gaussian fit cannot be done")
            param = [0.,0.,0.]
            covariance = [ [0.,0.,0.], [0.,0.,0.], [0.,0.,0.]]
            return param,covariance

        # #        parameters, covariance = curve_fit(self.Gauss, hX, hY)
        hyFit = self.gauss(hx,param[0],param[1],param[2])
#        print("GaussianFit par",param) #GDEB
#        print("GaussianFit cov",covariance) #GDEB
        #        print("hyFit ",hyFit)
        #-      plt.plot(hx,*self.gauss(hx,param[0],param[1],param[2],param[3]))
        #        plt.plot(hx, self.gauss(hx, *self.gauss_fit(hx, hy)), '--r', label='fit')
        lcolor = 'black'
        if idHis == 0:
            lcolor = 'black'
        elif idHis == 1:
            lcolor = 'black'
        #        bFitPlot = 1
        if bFitPlot%10 == 1 :
            hxt = []
            hyt = []
            if idHis == 1:
                lcolor = 'grey'
            elif idHis == 0:
                lcolor = 'red'
            #t DO NOT PLOT FIT
            plt.plot(hxt, hyt, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
            plt.plot(hx, hyFit, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
            plt.draw()
            #            plt.legend()
        #print("GaussianFit bFitPlot ",int(bFitPlot/10)) #GDEB
        if int(bFitPlot/10) == 1 :
            if idHis == 0:
                lcolor = 'blue'
                stext="MC sigma="
            elif idHis == 1:
                lcolor = 'orange'
                stext="Exp. sigma="
            xPos = self.xmax*0.2
            yPos = self.maximumY()*(0.7-idHis*0.1)
            #            plt.text(xPos,yPos,"mean= "+"{:.4e}".format(param[1]), color=lcolor)
            #            yPos = self.maximumY()*(0.65-idHis*0.1)
            #print("SIGMA COLOR",lcolor,"xytext",xPos,yPos,) #GDEB
            matplotlib.rcParams.update({'font.size': 15})
            plt.text(xPos,yPos,stext+" "+"{:.4g}".format(param[2]), color=lcolor)
            print("GAUSS TEXT",xPos,yPos,"sigma= "+"{:.4e}".format(param[2])) #GDEB
            print("GAUSS RES",param[0],param[1],param[2]) #GDEB
            plt.draw()

        return param,covariance

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def gauss(self,x, A, x0, sigma):
         #print(self.name,x0, sigma,"gauss", A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))) # GDEB
        return A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))
#    def gauss_fit(self,x, y):
#        xysum = 0
#        xsum = 0
#        ysum = 0
#        for ii in range(0,len(y)):
#            xysum += x[ii]*y[ii]
#            xsum += x[ii]
#            ysum += y[ii]
#        mean = xysum/ysum
##        mean = sum(x * y) / sum(y)
#        xysum = 0
#        for ii in range(0,len(y)):
#            xysum += y[ii]*(x[ii]-mean)**2
#        sigma = np.sqrt(xysum / ysum)
##        mean = sum(x * y) / sum(y)
##        sigma = np.sqrt(sum(y * (x - mean) ** 2) / sum(y))
#        popt, pcov = curve_fit(self.gauss, x, y, p0=[max(y), mean, sigma])
#        return popt

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def maximumY(self) :
        return np.max(self.data)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def minimumY(self) :
        return np.min(self.data)


#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def reduceLimits(self,minX,maxX,bNSteps=True) :
        xstep = self.Xstep()
        idMinX = int((minX-self.xmin)/xstep) # bin corresponding to minX
        if bNSteps :
            #            print( "idMinX ", idMinX, (minX-self.xmin),xstep,minX,self.xmin) # GDEB
            if (minX-self.xmin)/xstep - idMinX != 0 : # not integer
                minX = self.xmin + xstep*(idMinX-1) # decrease to bin lower value
                idMinX -= 1
             #   print("newMinX",minX) # GDEB
            idMaxX = int((maxX-self.xmin)/xstep) # bin corresponding to maxX
#            print( "idMaxX ", idMaxX,(maxX-self.xmin)/xstep,xstep,maxX,self.xmin) # GDEB
            if (maxX-self.xmin)/xstep - idMaxX != 0 : # not integer
                maxX = self.xmin + xstep*(idMaxX+1) # increase to bin higher value
#                print("newMaxX ",maxX) # GDEB
            nStepNew = int((maxX-minX)/xstep)
            idMaxX += 1

        dataOld = self.data
        dataErrOld = self.dataErr
        self.data = []
        self.dataErr = []
        self.nbin = idMaxX-idMinX
        for ii in range(self.nbin) :
            self.data.append(dataOld[ii+idMinX])
            self.dataErr.append(dataErrOld[ii+idMinX])
                    
        self.xmin = minX
        self.xmax = maxX
        
        return 

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetBin( self, xval, bIsOK ) :
        ibin = (xval-self.xmin)/self.Xstep()
        if bIsOK :
            if ibin < 0 or ibin >= self.nbin :
                print("!! WARNING MyHistos::GetBin: value ",xval," is out of X axis =(",self.xmin,",",self.xmax)
                return -1
            return int(ibin)
        
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetValueErr(self, xval) :
        ibin = self.GetBin(xval,True)
        if ibin != -1 :
            return(self.data[ibin],self.dataErr[ibin])
        else :
            return(None,None)
#@@ START GetClosestX:  13038.780740348151 34.239999999999995
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetClosestX(self, yval, xval) :
        # print("@@ START GetClosestX: ",yval,xval)
        distY = sys.float_info.max
        prevY = sys.float_info.max
        xbins = self.Xbins()
        xstep = self.Xstep()
        Xclosest = xbins[0]
        #print("@@ GetClosestX: =xbins[0] ",Xclosest) #GDEB
        for ii in range(1,len(self.data)) :
            #print("@@ GetClosestX: check ",self.data[ii-1],"&",self.data[ii],"X", xbins[ii-1],"&",xbins[ii]) #GDEB
            if self.data[ii] == yval :
                cX1 = xbins[ii]
                if np.abs(cX1-xval) < np.abs(Xclosest-xval) :
                    Xclosest = cX1
                #print("@@ GetClosestX: =xbins[ii] ",Xclosest,np.abs(cX1-xval)) #GDEB
            elif (self.data[ii-1] < yval and self.data[ii] > yval) or (self.data[ii-1] > yval and self.data[ii] < yval) :
                cX1 = xbins[ii-1] + xstep*np.abs(yval-self.data[ii-1])/np.abs(self.data[ii]-self.data[ii-1])
                #print("checkii",np.abs(cX1-xval),"<", Xclosest,cX1,xval)
                if np.abs(cX1-xval) < np.abs(Xclosest-xval) :
                    Xclosest = cX1
                #print("@@ GetClosestX: between bins",Xclosest,np.abs(cX1-xval),self.data[ii-1],"&",self.data[ii],"X", xbins[ii-1],"&",xbins[ii],":",np.abs(yval-self.data[ii-1]),np.abs(self.data[ii]-self.data[ii-1])) #GDEB
        return Xclosest

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.

    def GetIdInterpolatedValue(self, xval) :
        xBins = self.Xbins()
        ibin = -1 
        for ib1 in range(len(xBins)) :
            if xBins[ib1] == xval :
                ibin = ib1
                break
        if ibin != -1 :
            return ibin
        else :
 #           idBigger = [ii for ii, value in enumerate(xBins) if value > xval]
            idBigger = next(xx for xx, val in enumerate(xBins)
                            if val > xval)
            return idBigger

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetLinearInterpolatedValue(self, xval) :
        xBins = self.Xbins()
        if xval in xBins :
            xid = xBins.index(xval)
            print(xid,xval,"HHHPredictPDD:hyval", self.data[xid])

#        print(xval,"GIV",self.name,self.xmin,"=",xBins[0],xBins[len(xBins)-1])
        ibin = -1 
        for ib1 in range(len(xBins)) :
            if xBins[ib1] == xval :
                ibin = ib1
                break
        if ibin != -1 :
  #          print(ibin,"HHHPredictPDD:hyval",self.data[ibin],"FROM ",xval)
            return(self.data[ibin],self.dataErr[ibin])
        else :
            ibin = next(xx for xx, val in enumerate(xBins) if val > xval)
 #           print("idBigger",ibin,xval)
            ibinPrev = ibin-1
            xStep = self.Xstep()
            hyval = self.data[ibinPrev]+(self.data[ibin]-self.data[ibinPrev])*(xval-xBins[ibinPrev])/xStep
            hyvalErr = self.dataErr[ibinPrev]+(self.dataErr[ibin]-self.dataErr[ibinPrev])*(xval-xBins[ibinPrev])/xStep
#            print(ibin,xval,"HHHPredictPDD:hyval",hyval,self.data[ibinPrev],"+(",self.data[ibin],"-",self.data[ibinPrev],")*(",xval,"-",xBins[ibinPrev],")/",xStep," x1=",xBins[ibin])
            return hyval,hyvalErr

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetInterpolatedValue(self, xval) :
        xBins = self.Xbins()
        if xval in xBins :
            xid = xBins.index(xval)
            return self.data[xid],self.dataErr[xid]

        xarr = np.array(xBins)
        yarr = self.data
        if xval < xarr[0] :
#            print("GetInterpolatedValue: "+str(xval)+" is smaller than minimum"+str(xarr[0]))
            raise ValueError("GetInterpolatedValue: "+str(xval)+" is smaller than minimum"+str(xarr[0]))
        if xval > xarr[-1] :
            raise ValueError("GetInterpolatedValue: "+str(xval)+" is bigger than maximum"+str(xarr[-1]))
        # linear         yval = np.interp(xval,xarr,yarr)
        
        # Perform spline interpolation
        spline_interpolation = interp1d(xarr, yarr, kind='cubic')  # 'cubic' for cubic spline
        # Interpolate the value at xval
        yval = spline_interpolation(xval)
        yarrErr = self.dataErr
        spline_interpolationErr = interp1d(xarr, yarrErr, kind='cubic')  # 'cubic' for cubic spline
        yvalErr = spline_interpolationErr(xval)

        print("GETIV",yval,yvalErr)
        return yval,yvalErr

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetXDistSameY(self, yval, xval) :
        print("START GetXDistSameY", yval, xval) #GDEB
        xclosest = sys.float_info.max
        xValue = None
        Xbins1 = self.Xbins()
        xstep = self.Xstep()
        for ii in range(self.nbin) :
            if self.data[ii] == yval :
            #    print(ii,"GetXDistSameY y1 == y2",self.data[ii]) #GDEG
                return Xbins1[ii]
            # interpolate linearly y's of this bin and next bin
            if ii != self.nbin-1 :
                data1 = self.data[ii]
                data2 = self.data[ii+1]
                if max(data1,data2) < yval :
                    #print(ii,"GetXDistSameY yval too high",data1,data2,"<",yval) #GDEB
                    continue
                if min(data1,data2) > yval :
                    #print(ii,"GetXDistSameY yval too small",data1,data2,">",yval) #GDEB
                    continue
                xValue = Xbins1[ii] + xstep*(abs(yval-data1)/abs(data2-data1))
                # print("GetXDistSameY interpol",xValue, Xbins1[ii],xstep,yval,data1,data2,xstep*(abs(yval-data1)/abs(data2-data1))) #GDEB
                xclosest = min(abs(xValue-xval),xclosest)
                # print("GetXDistSameY xclosest",xclosest) #GDEB

        if xclosest == sys.float_info.max :
            return None
        else :
            return xclosest
   
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CalculateMeanAndRMS(self) :
        return #t
        Xbins = self.Xbins()
        self.mean = (sum(Xbins[ii] * self.data[ii] )/ sum(self.data[ii]) for ii in range(len(Xbins)))
        #    average = numpy.average(values, weights=weights)
        # Fast and numerically precise:
#t        self.mean = np.average(Xbins, weights=self.data)
  #t      self.RMS = np.sqrt(np.average((Xbins-self.mean)**2, weights=self.data))
        #   self.RMS = DescrStatsW(Xbins, weights=self.data, ddof=1).std
        #    print("MEAN",self.mean,"RMS",self.RMS,"=",self.RMS/DescrStatsW(Xbins, weights=self.data, ddof=1).std)
        self.meanErr = 0.
        self.RMSErr = 0.

        
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Histo2D(MutableMapping):
    type = "2D"
    name = ""
    xnbin = int(0)
    xmin = float(0)
    xmax = float(0)
    ynbin = int(0)
    ymin = float(0)
    ymax = float(0)
    xunderV = [] # left row
    xunderErrV = [] # left row
    yunderV = [] # down row
    yunderErrV = [] # down row
    xunder = 0
    yunder = 0
    xunderErr = 0
    yunderErr = 0
    xunderyunder = 0 # left down bin
    xunderyunderErr = 0 # left down bin
    xunderyover = 0 # right down bin
    xunderyoverErr = 0 # left down bin
    xoverV = [] # right row
    yoverV = [] # up row
    xoverErrV = [] # right row
    yoverErrV = [] # up row
    xover = 0
    yover = 0
    xoverErr = 0
    yoverErr = 0
    xoveryunder = 0 # left up bin
    xoveryunderErr = 0 # left up bin
    xoveryover = 0 # right up bin
    xoveryoverErr = 0 # right up bin
    dataNP = np.zeros([1, 1])
    dataErrNP = np.zeros([1, 1])
    nent = 0
    xmean = 0
    xmeanErr = 0
    xRMS = 0
    xRMSErr = 0
    ymean = 0
    ymeanErr = 0
    yRMS = 0
    yRMSErr = 0
 #    def __init__(self, *args, **kwargs):
        #args -- tuple of anonymous arguments
        #kwargs -- dictionary of named arguments

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __init__(self, args):
#        print("MyHistos args",args," N=",len((args)))
        self.xunderV = []
        self.yunderV = []
        self.xunderErrV = []
        self.yunderErrV = []
        self.xoverV = []
        self.yoverV = []
        self.xoverErrV = [] 
        self.yoverErrV = [] 
        if len(args) == 0 :
            return
        self.type = args[0]
        self.name = CleanName(args[1])
        self.xnbin = int(args[2])
        self.xmin = float(args[3])
        self.xmax = float(args[4])
        self.ynbin = int(args[5])
        self.ymin = float(args[6])
        self.ymax = float(args[7])
        self.xunderyunder = float(args[8]) # XOVERYOVER
        self.xunderyunderErr = float(args[9])
        #        print("xunderyunder",self.xunderyunder,"+-",self.xunderyunderErr) 
        self.dataNP = np.zeros([self.ynbin,self.xnbin])
        self.dataErrNP = np.zeros([self.ynbin,self.xnbin])
        id1 = 9 + 1
        for iiy in range(0,self.ynbin) : # LOOP TO OUNDER 
            val = float(args[id1+2*iiy])
            err = float(args[id1+2*iiy+1])
            self.xunderV.append(val)
            self.xunderErrV.append(err)
            self.xunder += val
            self.xunderErr += err*err
            # print(iiy,self.xunder,"+-",self.xunderErr," add xunderV ",val,"+-",err)
        self.xunderErr = math.sqrt(self.xunderErr)
        # print(iiy," xunder= ",self.xunder,"+-",self.xunderErr)
        id1 += self.ynbin*2 
        self.xunderyover = float(args[id1]) # XUNDERYOVER
        self.xunderyoverErr = float(args[id1+1])
        # print("xunderyover",self.xunderyover,"+-",self.xunderyoverErr)
        id1 += 2
        jj=0
        for iix in range(0,self.xnbin) : # LOOP TO BIN[X,Y] DATA
            val = float(args[id1])  # APPEND YUNDER 
            err = float(args[id1+1])
            self.yunderV.append(val)
            self.yunder += val
            self.yunderErrV.append(err)
            self.yunderErr += err*err
            # print(id,self.yunder,"+-",self.yunderErr," add yunderV ",val,"+-",err)
            id1 += 2
#            for iiy in range(self.ynbin-1,-1,-1) : # LOOP TO BIN[X,Y] DATA
            for iiy in range(0,self.ynbin) : # LOOP TO BIN[X,Y] DATA
                #print(iix,iiy,id1,id1+iiy*2,"fill 2D data2",len(args))
                val = float(args[id1+iiy*2])
                #                print("FILL DATA",jj,iix,iiy,val)#GDEB
                #                iiyInv = self.ynbin-iiy-1 # y must be reversed
                #                self.dataNP[iiyInv,iix] = val
                #                self.dataErrNP[iiyInv,iix] = val
                self.dataNP[iiy,iix] = val
                self.dataErrNP[iiy,iix] = val
                jj = jj+1
#                if( val != 0 ) :  print(self.name,iix,iiy,iiyInv,self.dataNP[iiyInv,iix] ) # GDEB
                      
            id1 += 2*self.ynbin
            val = float(args[id1])  # APPEND XOVER
            err = float(args[id1+1])
            self.yoverV.append(val)
            self.yover += val
            self.yoverErrV.append(err)
            self.yoverErr += err*err
            # print(id,self.yover,"+-",self.yoverErr," add yoverV ",val,"+-",err)
            id1 += 2
        #5 0 755
        
        self.yunderErr = math.sqrt(self.yunderErr)
        self.yoverErr = math.sqrt(self.yoverErr)
            
        self.xoveryunder = float(args[id1]) # XOVERYUNDER
        self.xoveryunderErr = float(args[id1+1])
        id1 += 2
        for id in range(0,self.ynbin) : # LOOP TO XOVER
            val = float(args[id1+id*2])
            err = float(args[id1+id*2+1])
            self.xoverV.append(val)
            self.xoverErrV.append(err)
            self.xover += val
            self.xoverErr += err*err
        self.xoverErr = math.sqrt(self.xoverErr)
        id1 += self.ynbin*2
        self.xoveryover = float(args[id1])  # XOVERYOVER
        self.xoveryoverErr = float(args[id1+1])
        id1 += 2
        self.nent = float(args[id1])
        self.xmean = float(args[id1+1])
        self.xmeanErr = float(args[id1+2])
        self.xRMS = float(args[id1+3])
        self.xRMSErr = float(args[id1+4])
        self.ymean = float(args[id1+5])
        self.ymeanErr = float(args[id1+6])
        self.yRMS = float(args[id1+7])
        self.yRMSErr = float(args[id1+8])
        #        self.num_holes = kwargs.get('num_holes',random_holes())
        #        print("HISTO2_INIT_FROM_LINE N",len(self.data),self.xmin,self.xmax)
#        print(self.name,"DATANP",self.dataNP)


    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def FillFromLists(self,name,minX,maxX,minY,maxY,nX,nY,data,dataErr) :
        #print("fillHisto2FromArray: ",self.name)
        if len(self.dataNP) != 0 :
            self.dataNP = np.zeros([nY,nX])
            self.dataErrNP = np.zeros([nY,nX])
            self.xunderV = []
            self.xunderErrV = []
            self.xoverV = []
            self.xoverErrV = []
            self.yunderV = []
            self.yunderErrV = []
            self.yoverV = []
            self.yoverErrV = []
        self.name = name
        self.xnbin = nX
        self.ynbin = nY
        self.xmin = minX
        self.xmax = maxX
        self.ymin = minY
        self.ymax = maxY
        sumX = np.zeros(nX)
        sumY = np.zeros(nY)
        sumXerr2 = np.zeros(nX)
        sumYerr2 = np.zeros(nY)
        xbins = self.Xbins()
        ybins = self.Ybins()
        nent = 0    
        for ix in range(nX) :
            for iy in range(nY) :
                val = data[ix+iy*nX]
                err = dataErr[ix+iy*nX]
                #print(len(self.data),"fillHisto2FromArray",ix+iy*nX,ix,iy,val)
                self.dataNP[iy,ix] = val
                self.dataErrNP[iy,ix] = err
                ix = int(ixy/nY)
                iy = ixy%nY
                sumX[ix] += val
                sumY[iy] += val
                sumXerr2[ix] += err*err
                sumYerr2[iy] += err*err
                nent += val
        self.nent = nent

        sumx = 0.
        sumx2 = 0.
        sumxerr2 = 0.
        for ix in range(nX) :
            sumx += sumX[ix]*xbins[ix]
            sumx2 += sumX[ix]*sumX[ix]*xbins[ix]*xbins[ix]
            sumxerr2 += sumXerr2[ix]*sumXerr2[ix]*xbins[ix]*xbins[ix]
        if sumX.sum() != 0 :
            self.xmean = sumx/sumX.sum()
            self.xmeanErr = np.sqrt(sumxerr2)/sumX.sum()
            #    self.xRMS = np.sqrt(sumx2/sumX.sum()-sumx*sumx)
        else :
            print("!!! WARNING: empty histogram ",self.name," range_X ",ix,"  out of",range(nX))
        self.xRMS = 0.
        self.xRMSErr = 0.

        sumy = 0.
        sumy2 = 0.
        sumyerr2 = 0.
        for iy in range(nY) :
            sumy += sumY[iy]*ybins[iy]
        if sumY.sum() != 0 :
            sumy2 += sumY[iy]*sumY[iy]*ybins[iy]*ybins[iy]
            sumyerr2 += sumYerr2[iy]*sumYerr2[iy]*ybins[iy]*ybins[iy]
        else :
            print("!!! WARNING: empty histogram ",self.name," range_Y ",iy,"  out of",range(nY))
            self.ymean = sumy/sumY.sum()
            #        self.ymeanErr = np.sqrt(sumyerr2)/sumY.sum()
            #    self.yRMS = np.sqrt(sumy2/sumY.sum()-sumy*sumy)
        self.yRMS = 0.
        self.yRMSErr = 0.

        for iiy in range(0,self.ynbin) : # LOOP TO OUNDER 
            self.xunderV.append(0.)
            self.xunderErrV.append(0.)
            self.xoverV.append(0.)
            self.xoverErrV.append(0.)
        for iix in range(0,self.xnbin) : # LOOP TO BIN[X,Y] DATA
            self.yunderV.append(0.)
            self.yunderErrV.append(0.)
            self.yoverV.append(0.)
            self.yoverErrV.append(0.)

        
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def FillFromNPArrays(self,name,minX,maxX,minY,maxY,dataNP,dataErrNP) : 
       #print("fillFromNPMats: ",name)
        self.name = name
        self.xnbin = dataNP.shape[1]
        self.xmin = minX
        self.xmax = maxX
        self.ynbin = dataNP.shape[0]
        self.ymin = minY
        self.ymax = maxY
        self.dataNP = dataNP
        self.dataErr = dataErrNP
        print("DATANPARRAY N",len(self.dataNP),self.xmin,self.xmax)
        if len(self.dataNP) != 0 :
            self.xunderV = []
            self.xunderErrV = []
            self.xoverV = []
            self.xoverErrV = []
            self.yunderV = []
            self.yunderErrV = []
            self.yoverV = []
            self.yoverErrV = []
        for iiy in range(0,self.ynbin) : # LOOP TO OUNDER 
            self.xunderV.append(0.)
            self.xunderErrV.append(0.)
            self.xoverV.append(0.)
            self.xoverErrV.append(0.)
        for iix in range(0,self.xnbin) : # LOOP TO BIN[X,Y] DATA
            self.yunderV.append(0.)
            self.yunderErrV.append(0.)
            self.yoverV.append(0.)
            self.yoverErrV.append(0.)
        self.nent = self.xnbin*self.ynbin

        sumX = np.zeros(self.xnbin)
        sumY = np.zeros(self.ynbin)
        sumXerr2 = np.zeros(self.xnbin)
        sumYerr2 = np.zeros(self.ynbin)
        xbins = self.Xbins()
        ybins = self.Ybins()
        nent = 0
        for iy in range(self.ynbin) :
            for ix in range(self.xnbin) :
                val = dataNP[iy,ix]
                err = dataErrNP[iy,ix]
                #print(len(self.dataNP),"FillFromNPMats",ix+iy*self.xnbin,ix,iy,val)
                sumX[ix] += val
                sumY[iy] += val
                sumXerr2[ix] += err*err
                sumYerr2[iy] += err*err
                nent += val
        self.nent = self.xnbin*self.ynbin

        sumx = 0.
        sumx2 = 0.
        sumxerr2 = 0.
        for ix in range(self.xnbin) :
            sumx += sumX[ix]*xbins[ix]
            sumx2 += sumX[ix]*sumX[ix]*xbins[ix]*xbins[ix]
            sumxerr2 += sumXerr2[ix]*sumXerr2[ix]*xbins[ix]*xbins[ix]
        if sumX.sum() != 0 :
            self.xmean = sumx/sumX.sum()
            self.xmeanErr = np.sqrt(sumxerr2)/sumX.sum()
            #    self.xRMS = np.sqrt(sumx2/sumX.sum()-sumx*sumx)
        else :
            print("!!! WARNING: empty histogram ",self.name," range_X ",ix,"  out of",range(self.xnbin))
        self.xRMS = 0.
        self.xRMSErr = 0.

        sumy = 0.
        sumy2 = 0.
        sumyerr2 = 0.
        for iy in range(self.ynbin) :
            sumy += sumY[iy]*ybins[iy]
        if sumY.sum() != 0 :
            sumy2 += sumY[iy]*sumY[iy]*ybins[iy]*ybins[iy]
            sumyerr2 += sumYerr2[iy]*sumYerr2[iy]*ybins[iy]*ybins[iy]
        else :
            print("!!! WARNING: empty histogram ",self.name," range_Y ",iy,"  out of",range(self.ynbin))
            self.ymean = 0. #sumy/sumY.sum()
            #        self.ymeanErr = np.sqrt(sumyerr2)/sumY.sum()
            #    self.yRMS = np.sqrt(sumy2/sumY.sum()-sumy*sumy)
        self.yRMS = 0.
        self.yRMSErr = 0.

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __setitem__(self, k, v):
        setattr(self, k, v)
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __getitem__(self, k):
        getattr(self, k)
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __len__(self):
        return 2
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __delitem__(self, k):
        self[k] = None
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __iter__(self):
        yield self.name
        yield self.xnbin
        yield self.xmin
        yield self.xmax
        yield self.under
        yield self.dataNP
        yield self.over
        yield self.underErr
        yield self.dataErrNP
        yield self.overErr
        yield self.nent
        yield self.mean
        yield self.meanErr
        yield self.RMS
        yield self.RMSErr

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def __str__(self):
        return ("Histo2D: "+self.type+" "+self.name
                +" X:"+ str(self.xnbin)
                +" "+str(self.xmin)+" "+str(self.xmax)
                +" Y:"+ str(self.ynbin)
                +" "+str(self.ymin)+" "+str(self.ymax)
                +" xUyU "+str(self.xunderyunder)+"+-"+str(self.xunderyunderErr)
                +" xU="+str(self.xunder)+"+-"+str(self.xunderErr)
                +" xUyO "+str(self.xunderyover)+"+-"+str(self.xunderyoverErr)
                +" yU="+str(self.yunder)+"+-"+str(self.yunderErr)
                +" yO="+str(self.yover)+"+-"+str(self.yoverErr)
                +" xOyU "+str(self.xoveryunder)+"+-"+str(self.xoveryunderErr)
                +" xO="+str(self.xover)+"+-"+str(self.xoverErr)
                +" xOyO "+str(self.xoveryover)+"+-"+str(self.xoveryoverErr)
                +" NENT="+str(self.nent)
                +" xMEAN="+str(self.xmean)+"+-"+str(self.xmeanErr)
                +" xRMS="+str(self.xRMS)+"+-"+str(self.xRMSErr)
                +" yMEAN="+str(self.ymean)+"+-"+str(self.ymeanErr)
                +" yRMS="+str(self.yRMS)+"+-"+str(self.yRMSErr)
                )

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Xstep(obj):
        xStep = (float(obj.xmax)-float(obj.xmin))/int(obj.xnbin)
        return xStep
    def Ystep(obj):
        yStep = (float(obj.ymax)-float(obj.ymin))/int(obj.ynbin)
        return yStep
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Xbins(self):
        #print("XBINS_NAME",self.name,self.xnbin,self.xmin,self.xmax)
        Xbins = []
        xStep = (float(self.xmax)-float(self.xmin))/int(self.xnbin)
        for ii in range(0,int(self.xnbin)):
            Xbins.append(float(self.xmin)+(0.5+ii)*xStep)
            #print(ii,"ADD Xbin",float(self.xmin)+(0.5+ii)*xStep,":",self.xmin,"+(0.5+",ii,")*",xStep)
        return Xbins
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Ybins(self):
        Ybins = []
        yStep = (float(self.ymax)-float(self.ymin))/int(self.ynbin)
        for ii in range(0,int(self.ynbin)):
            Ybins.append(float(self.ymin)+(0.5+ii)*yStep)
        return Ybins 

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CheckEqual(self,obj):
#        if self.name != obj.name :
#            print("!!! ERROR: two histograms with different name ",
#                  self.name," !=? ", obj.name)

        if self.xnbin != obj.xnbin or self.xmin != obj.xmin or self.xmax != obj.xmax or self.ynbin != obj.ynbin or self.ymin != obj.ymin or self.ymax != obj.ymax :
            print("!!! ERROR: two histograms with same name do not have same data ",
                  self.name,"XNBIN:",self.xnbin," !=? ", obj.xnbin,
                  "XMIN:",self.xmin," !=? ",obj.xmin,
                  "XMAX:",self.xmax," !=? ",obj.xmax,
                  "YNBIN:",self.ynbin," !=? ", obj.ynbin,
                  "YMIN:",self.ymin," !=? ",obj.ymin,
                  "YMAX:",self.ymax," !=? ",obj.ymax)
            return False
            # sys.exit()
        return True

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Add(self,obj,bCheckEqual=True):
        if bCheckEqual and not self.CheckEqual(obj) :
            return self

        newData = sum2(self.xunderyunder,self.xunderyunderErr,obj.xunderyunder,obj.xunderyunderErr)
        self.xunderyunder = newData[0]
        self.xunderyunderErr = newData[1]
        
        for iiy in range(0,self.ynbin) : # LOOP TO OUNDER 
            newData = sum2(self.xunderV[iiy],self.xunderErrV[iiy],obj.xunderV[iiy],obj.xunderErrV[iiy])
            self.xunderV[iiy] = newData[0]
            self.xunderErrV[iiy] = newData[1]
        newData = sum2(self.xunderyover,self.xunderyoverErr,obj.xunderyover,obj.xunderyoverErr)
        self.xunderyover = newData[0]
        self.xunderyoverErr = newData[1]

        for iix in range(0,self.xnbin) : # LOOP TO BIN[X,Y] DATA
            newData = sum2(self.yunderV[iix],self.yunderErrV[iix],obj.yunderV[iix],obj.yunderErrV[iix])
            self.yunderV[iix] = newData[0]
            self.yunderErrV[iix] = newData[1]
            for iiy in range(0,self.ynbin) : # LOOP TO BIN[X,Y] DATA
                iixy = iix+iiy*self.xnbin
                newData = sum2(self.dataNP[iiy,iix],self.dataErrNP[iiy,iix],obj.dataNP[iiy,iix],obj.dataErrNP[iiy,iix])
#                print(iix,iiy,"NEWDATA ",newData[0],self.data[iixy],obj.data[iixy])
                self.dataNP[iiy,iix] = newData[0]
                self.dataErrNP[iiy,iix] = newData[1]

            newData = sum2(self.yoverV[iix],self.yoverErrV[iix],obj.yoverV[iix],obj.yoverErrV[iix])
            self.yoverV[iix] = newData[0]
            self.yoverErrV[iix] = newData[1]

        newData = sum2(self.xoveryunder,self.xoveryunderErr,obj.xoveryunder,obj.xoveryunderErr)
        self.xoveryunder = newData[0]
        self.xoveryunderErr = newData[1]
        for iiy in range(0,self.ynbin) : # LOOP TO OOVER 
            newData = sum2(self.xoverV[iiy],self.xoverErrV[iiy],obj.xoverV[iiy],obj.xoverErrV[iiy])
            self.xoverV[iiy] = newData[0]
            self.xoverErrV[iiy] = newData[1]

        newData = sum2(self.xoveryover,self.xoveryoverErr,obj.xoveryover,obj.xoveryoverErr)
        self.xoveryover = newData[0]
        self.xoveryoverErr = newData[1]
        
        self.nent += obj.nent

        newData = sum2(self.xmean,self.xmeanErr,obj.xmean,obj.xmeanErr)
        #        print("ADD MEAN ",self.xmean,self.xmeanErr,obj.xmean,obj.xmeanErr)
        self.xmean = newData[0]
        self.xmeanErr = newData[1]
        newData = sum2(self.xRMS,self.xRMSErr,obj.xRMS,obj.xRMSErr)
        self.xRMS = newData[0]
        self.xRMSErr = newData[1]

        newData = sum2(self.ymean,self.ymeanErr,obj.ymean,obj.ymeanErr)
        self.ymean = newData[0]
        self.ymeanErr = newData[1]
        newData = sum2(self.yRMS,self.yRMSErr,obj.yRMS,obj.yRMSErr)
        self.yRMS = newData[0]
        self.yRMSErr = newData[1]

        return self
        
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Mult(self,factor):

        self.dataNP = self.dataNP*factor
        self.dataErrNP = self.dataErrNP*factor

#        self.RMS = newData[0]
 #       self.RMSErr = newData[1]

        return self
    
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CentrePoint(self):
        centre = np.array([(self.xmin+self.xmax)/2.,(self.ymin+self.ymax)/2.])
        return centre

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CrossLine(self,angle,step,bCentred=True,bInterpolate=True):
        centre = self.CentrePoint()+np.array([0.01,0.01])
        hwidthX = (self.xmax-self.xmin)/2.
        hwidthY = (self.ymax-self.ymin)/2.
        #print(np.rad2deg(angle),"@@@ CrossLine hwidths ",centre,hwidthX,hwidthY) # GDEB
        cosang = math.cos(angle)
        sinang = math.sin(angle)
        if cosang != 0. :
            tanang = math.tan(angle)
        else :
            tanang = sys.float_info.max/10.
        if tanang != 0. :
            cotanang = 1./tanang
        else :
            cotanang = sys.float_info.max/10.
            #        print(np.rad2deg(angle),"@@@ CrossLine tan",tanang,cotanang) # GDEB
        ### Get first and last points in one of the borders of the histogram
        angsign = np.sign(np.deg2rad(180.)-angle%np.rad2deg(360.))
        angleN = angle%np.deg2rad(180.)
        if angleN < np.deg2rad(45.) :
            lstart = np.array([self.xmin,centre[1]-hwidthX*tanang])
            lend   = np.array([self.xmax,centre[1]+hwidthX*tanang])
        elif angleN < np.deg2rad(90.) :
            lstart = np.array([centre[0]-hwidthY*cotanang,self.ymin])
            lend   = np.array([centre[0]+hwidthY*cotanang,self.ymax])
        elif angleN < np.deg2rad(135.) :
            lstart = np.array([centre[0]-hwidthY*cotanang,self.ymin])
            lend = np.array([centre[0]+hwidthY*cotanang,self.ymax])
        elif angleN < np.deg2rad(180.) :
            lstart = np.array([self.xmax,centre[1]+hwidthX*tanang])
            lend   = np.array([self.xmin,centre[1]-hwidthX*tanang])
        lstart += np.array([0.01,0.01])
        lend += np.array([0.01,0.01])
        ldir = np.array([cosang,sinang])
        #print(np.rad2deg(angle),np.rad2deg(angleN),"00CrossLine",ldir,lstart,lstart[0],lstart[1],lend,angsign,step)  # GDEB
        if angsign <= 0 : # 180 -> angsign=0.
            lstartn = lend
            lend = lstart
            lstart = lstartn
#        print(np.rad2deg(angle),np.rad2deg(angleN),"CrossLine",ldir,lstart,lend)  # GDEB
        hwidthXY = np.sqrt(hwidthX*hwidthY)
        nPoints = int(np.sqrt((lend-lstart).dot(lend-lstart))/step)
        # print("nPOINTS ",nPoints,lend-lstart,(lend-lstart).dot(lend-lstart)) #GDEB
        line = [] 
        #     for ss in np.arange(0.,hwidthXY,step) :
        #         pos = self.GetVoxel(lstart+ldir*ss)
        for ss in range(nPoints) :
            dist = step*(ss+0.5)
            voxID = self.GetVoxel(lstart+ldir*dist)
            if voxID == (-1,-1) :
                continue
            linepos = dist-(nPoints)/2.*step
            if not bInterpolate : 
                # print("voxID",voxID,lstart+ldir*dist,lstart,"+",ldir,"*",step,"*(",ss+0.5) # GDEB
                lineval = np.array([linepos,self.dataNP[voxID[1],voxID[0]],self.dataErrNP[voxID[1],voxID[0]]])
                line.append(lineval)
                # print(voxID,ss,"LINE APPEND",lineval,len(line),lstart+ldir*step*(ss+0.5)) # GDEB
                #print("CentrePoint LINE ",line) # GDEB
            else :
                ### average of values of 9 ID
                distSum = 0.
                valAver = 0.
                valErrAver = 0.
                idX = voxID%self.xnbin
                idY = int(voxID/self.xnbin)
                xstep = self.Xstep()
                ystep = self.Ystep()
                linepos2D = lstart+ldir*dist
                for ix in range(idX-1,idX+2) : 
                    if ix < 0 or ix >= self.xnbin :
                        continue
                    for iy in range(idY-1,idY+2) :
                        if iy < 0 or iy >= self.ynbin :
                            continue
                        xpos = self.xmin+xstep*(0.5+ix) # position of voxel center
                        ypos = self.ymin+ystep*(0.5+iy)
                        dist = math.sqrt(math.pow(xpos-linepos2D[0],2)+math.pow(ypos-linepos2D[1],2))
                       #print("DIST",dist,xpos,"-",linepos2D[0],"+",ypos,"-",linepos2D[1]) #GDEB
                        dist2 = math.pow(dist,2)
                        voxID1 = ix+iy*self.xnbin
                        val = self.data[voxID1[1],ID1[0]]
                        err = self.dataErr[voxID1[1],ID1[0]]
                        valAver += val/dist2
                        valErrAver += pow(err/dist2,2)
                        distSum += 1./dist2
                        #print(ix,iy,"INTERPOLATE",val,dist2,valAver,valErrAver,distSum) #GDEB
                valAver /= distSum
                valErrAver = math.sqrt(valErrAver/pow(distSum,2))
                #rint("INTERPOLATE FINAL",valAver,valErrAver) # GDEB
                lineval = np.array([linepos,valAver,valErrAver])
                #rint(voxID,ss,"LINE APPEND",lineval,len(line),lstart+ldir*step*(ss+0.5)) # GDEB
                line.append(lineval)
                        
        return line

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def DisplaceXY(self,dispX,dispY,orderInterp = 1):
        xstep = self.Xstep()
        dispInBinX = dispX%xstep
        ixLeft = int(dispX/xstep)+int((np.sign(dispX)-1)/2)
        ixRight = ixLeft+1
        #print("DisplaceX DispX",dispX," inbin",dispInBinX," ix",ixLeft,"<>",ixRight," xstep",xstep, "   ",dispX/xstep,int(dispX/xstep),int((np.sign(dispX)-1)/2)) #GDEB
        ystep = self.Ystep()
        dispInBinY = dispY%ystep
        iyLeft = int(dispY/ystep)+int((np.sign(dispY)-1)/2)
        iyRight = iyLeft+1
       # print("DisplaceY DispY",dispY," inbin",dispInBinY," iy",iyLeft,"<>",iyRight," ystep",ystep, "   ",dispY/ystep,int(dispY/ystep),int((np.sign(dispY)-1)/2)) #GDEB
        nData = np.shape(self.dataNP)
        dataNew = np.zeros([self.ynbin,self.xnbin])
        for ix in range(nData[1]) : #!?!?!
            for iy in range(nData[0]) :
                ix1 = int(max(0,ix+ixLeft))
                ix1 = int(min(self.xnbin-1,ix1))
                ix2 = int(max(0,ix+ixRight))
                ix2 = int(min(self.xnbin-1,ix2))
                iy1 = int(max(0,iy+iyLeft))
                iy1 = int(min(self.ynbin-1,iy1))
                iy2 = int(max(0,iy+iyRight))
                iy2 = int(min(self.ynbin-1,iy2))
                # interpolate 4 bins
                # print("ixy's",ix1,ix2,iy1,iy2) #GDEB
                data00 = self.dataNP[iy1*self.xnbin,ix1]
                data01 = self.dataNP[iy2*self.xnbin,ix1]
                data10 = self.dataNP[iy1*self.xnbin,ix2]
                data11 = self.dataNP[iy2*self.xnbin,ix2]
                # distance to centre of bin
                dist00 = (xstep-dispInBinX)*(ystep-dispInBinY)
                dist01 = (xstep-dispInBinX)*(dispInBinY)
                dist10 = (dispInBinX)*(ystep-dispInBinY)
                dist11 = (dispInBinX)*(dispInBinY)
                distTot = dist00+dist01+dist10+dist11
                #print(" DisplaceXY dist ",dist00,dist01,dist10,dist11,distTot,"data",data00,data01,data10,data11) #GDEB
                dataNew1 = (data00*dist00+data01*dist01+data10*dist10+data11*dist11)/distTot
                dataNew.append(dataNew1)
                #print(" DisplaceXY ",ix,ix1,ix2,iy,iy1,iy2,dataNew1) # GDEB
                            
        self.data = dataNew

        return self
            

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetVoxel(self,pos) :
    #        print("GETVOXEL ",pos) #GDEB
        xstep = self.Xstep()
        ystep = self.Ystep()
        idX = int((pos[0]-self.xmin)/xstep)
        idY = int((pos[1]-self.ymin)/ystep)
        #print("GetVoxel x",pos,idX,"=int((",pos[0],"-",self.xmin,")/",xstep,(pos[0]-self.xmin)/xstep) # GDEB
        #print("GetVoxel y",pos,idY,"=int((",pos[0],"-",self.ymin,")/",ystep,(pos[0]-self.ymin)/ystep) #GDEB
        if idX >= 0 and idY >= 0 and idX < self.xnbin and idY < self.ynbin :
            return int(idX,idY)
        else :
            return (-1,-1)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def Write(self,fOut):
        fOut.write('"'+self.type+'"')
        fOut.write(',"'+self.name+'"')
        fOut.write(","+str(self.xnbin))
        fOut.write(","+str(self.xmin))
        fOut.write(","+str(self.xmax))
        fOut.write(","+str(self.ynbin))
        fOut.write(","+str(self.ymin))
        fOut.write(","+str(self.ymax))
        fOut.write(","+str(self.xunderyunder))
        fOut.write(","+str(self.xunderyunderErr))
        for iiy in range(0,self.ynbin) : # LOOP TO OUNDER 
            fOut.write(","+str(self.xunderV[iiy]))
            fOut.write(","+str(self.xunderErrV[iiy]))
        fOut.write(","+str(self.xunderyover))
        fOut.write(","+str(self.xunderyoverErr))
        for iix in range(0,self.xnbin) : # LOOP TO BIN[X,Y] DATA
            fOut.write(","+str(self.yunderV[iix]))
            fOut.write(","+str(self.yunderErrV[iix]))
            for iiy in range(0,self.ynbin) : # LOOP TO BIN[X,Y] DATA 
                iixy = iiy+iix*self.ynbin
                fOut.write(","+str(self.dataNP[iiy,iix]))
                fOut.write(","+str(self.dataErrNP[iiy,iix]))
                #print(iixy,iix,iiy,"WRITE NEWDATA ",(self.data[iixy]))
                # print(iix,iiy,"WRITE NEWDATA ",(self.dataErr[iix]))

            fOut.write(","+str(self.yoverV[iix]))
            fOut.write(","+str(self.yoverErrV[iix]))

        fOut.write(","+str(self.xoveryunder))
        fOut.write(","+str(self.xoveryunderErr))
        for iiy in range(0,self.ynbin) : # LOOP TO OOVER 
            fOut.write(","+str(self.xoverV[iiy]))
            fOut.write(","+str(self.xoverErrV[iiy]))
        fOut.write(","+str(self.xoveryover))
        fOut.write(","+str(self.xoveryoverErr))
            
        fOut.write(","+str(self.nent))
        fOut.write(","+str(self.xmean))
        fOut.write(","+str(self.xmeanErr))
        fOut.write(","+str(self.xRMS))
        fOut.write(","+str(self.xRMSErr))
        fOut.write(","+str(self.ymean))
        fOut.write(","+str(self.ymeanErr))
        fOut.write(","+str(self.yRMS))
        fOut.write(","+str(self.yRMSErr))
        fOut.write("\n")

#    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
#    def SquareHisto(self) :
#        xBins = self.Xbins()
#        yBins = self.Ybins()
#        if len(xBins) == len(yBins) :
#            return
#        
#        xarr = np.array(xBins)
#        yarr = np.array(yBins)        
#        if len(yarr) < len(xarr) :
#            ystep = self.Ystep(self)
#            for ix in range(0,self.xnbin) : 
#                for iy in range(0,self.xnbin) :
#                    ixy = ix*self.ynbin + iy
#                    if iy < self.ynbin :
#                        val = self.dataNP[iy,ix]
#                    else :
#                        #print(ix,iy,"VALFILL",len(self.data), ix*self.ynbin +self.ynbin-1)
#                        val = self.dataNP[iy-1,ix] # copy last row
#                        
#                    dataNPT.append(val)
#                    self.dataNP[iy,ix] = val
#                    self.dataErrNP[iy,ix] = self.dataErrNP[ iy*self.xnbin +self.xnbin-1])
#            for ii in range(len(yarr),len(xarr)) :
#                #            np.append(yarr,yarr[-1])
#                yBins.append(yBins[-1]*1.00001)
#            yarr = np.array(yBins)
#            self.ymax += (len(yarr)-len(xarr))*ystep
#            
#        if len(xarr) < len(yarr) :
#            xstep = self.Xstep()
#            for ix in range(0,self.ynbin) : 
#                for iy in range(0,self.ynbin) :
#                    ixy = ix*self.ynbin + iy
#                    if ix < self.xnbin :
#                        val = self.data[ixy]
#                    else :
#                        #print(ix,iy,"VALFILL",len(self.data), ix*self.ynbin +self.ynbin-1)
#                        val = self.data[ iy*self.xnbin +self.xnbin-1] # copy last row
#                    dataNPT.append(val)
#                    self.data.append(val)
#                    self.dataErr.append(self.dataErr[ iy*self.xnbin +self.xnbin-1])
#            for ii in range(len(xarr),len(yarr)) :
#                #            np.append(yarr,yarr[-1])
#                xBins.append(xBins[-1]*1.00001)
#            xarr = np.array(xBins)
#            self.xmax += (len(xarr)-len(yarr))*xstep
#
#        self.dataNP = dataNPT
#        self.xnbin = len(xBins)
#        self.ynbin = len(yBins)
#        print("SquareHisto",self.dataNP.shape)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetInterpolatedValue(self, xval,yval) :
        xBins = self.Xbins()
        yBins = self.Ybins()
        #print("XBINS",xBins)
        #print("YBINS",yBins)        
        if xval in xBins and yval in yBins :
            xid = xBins.index(xval)
            yid = yBins.index(yval)
            return self.dataNP[yid,xid]
        #        else :
        #            print("!!NOT BINS ",xval,yval)

#        xid = next((ii for ii, value in enumerate(xBins) if value > xval), None)

 #       xid = [ii for ii, value in enumerate(xBins) if value >= xval][0]
  #      yid = [ii for ii, value in enumerate(yBins) if value >= yval][0]
        #print(xid,yid,"GetInterpolatedValue DATA")

        xarr = np.array(xBins)
        yarr = np.array(yBins)        

        if xval < xarr[0] :
            print("XARR",xarr)
            raise ValueError("GetInterpolatedValue x : "+str(xval)+" is smaller than minimum "+str(xarr[0]))
        if xval > xarr[-1] :
            raise ValueError("GetInterpolatedValue x : "+str(xval)+" is bigger than maximum "+str(xarr[-1]))
        if yval < yarr[0] :
            print("XARR",xarr)
            print("YARR",yarr)
            raise ValueError("GetInterpolatedValue y: "+str(yval)+" is smaller than minimum "+str(yarr[0]))
        if yval > yarr[-1] :
            raise ValueError("GetInterpolatedValue y:"+str(yval)+" is bigger than mayimum "+str(yarr[-1]))
       
 #       # Create a grid of (x, y) pairs where you want to interpolate z
#        x_grid, y_grid = np.meshgrid(np.linspace(xarr.min(), xarr.max(), 100), np.linspace(yarr.min(), yarr.max(), 100))

        # Perform the interpolation
#        print("GETINTERP XARR",xarr)
#        print("GETINTERP YARR",yarr)

        interp = interp2d(xarr, yarr, self.dataNP, kind='cubic')
 #       x_grid, y_grid = np.meshgrid(np.linspace(min(xBins), max(xBins), self.xnbin), np.linspace(min(yBins), max(yBins), self.ynbin))
#        interpF = interp(np.array(x_grid)[0],np.array(y_grid)[:, 0]) 
        # Interpolate the data on the new grid
#        fig = plt.figure()
#        ax = fig.add_subplot(111, projection='3d')
#        # Plot the surface
#        ax.plot_surface(x_grid, y_grid, interpF, cmap='viridis')
#        plt.show()

        zval = interp(xval,yval)

#        # Perform 2D interpolation        
#        # Interpolate z values for specific (x, y) pairs
#        zval = griddata((xarr, yarr), zval, (xval, yval), method='cubic')     # Perform spline interpolation

        return zval

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetLinearInterpolatedValue(self, xval,yval) :
        xBins = self.Xbins()
        yBins = self.Ybins()
        print("START GetLinearInterpolatedValue", xval,yval,"min=",xBins[0],yBins[0])
        if (xval - xBins[0]) < 0 :
            xval = xBins[0]
        if (xval - xBins[0]) < -2e-3 :
            print("XBINS",xBins)
            raise ValueError("GetInterpolatedValue x : "+str(xval)+" is smaller than minimum "+str(xBins[0])+" : "+str(xval - xBins[0]))
        if xval > xBins[-1] :
            raise ValueError("GetInterpolatedValue x : "+str(xval)+" is bigger than maximum "+str(xBins[-1]))
        if (yval - yBins[0]) < 0 :
            yval = yBins[0]
        if (yval - yBins[0]) < -2e-3 :
            print("YBINS",yBins)
            raise ValueError("GetInterpolatedValue y: "+str(yval)+" is smaller than minimum "+str(yBins[0])+" : "+str(yval - yBins[0]))
        if yval > yBins[-1] :
            raise ValueError("GetInterpolatedValue y:"+str(yval)+" is bigger than mayimum "+str(yBins[-1]))
       
        xid = -1
        yid = -1
        if xval in xBins :
            xid = xBins.index(xval) 
        if yval in yBins :
            yid = yBins.index(yval) 

        if xid != -1 and yid != -1 :
            print("DAT in bins",self.dataNP[yid,xid])
            return self.data[yid,xid]
        
        ibinx = next(xx for xx, val in enumerate(xBins) if val > xval)
        ibiny = next(yy for yy, val in enumerate(yBins) if val > yval)

        dataSum = 0.
        distSum = 0.
        xstep = self.Xstep()
        ystep = self.Ystep()
        for ix in range(ibinx-1,ibinx+1) :
            xb = self.xmin + xstep*(ix+0.5)
            for iy in range(ibiny-1,ibiny+1) :
                if iy < 0:
                    continue
                yb = self.ymin + ystep*(iy+0.5)
                dist = np.sqrt(pow(xval-xb,2)+pow(yval-yb,2))
                distSum += dist
                dataSum += self.dataNP[iy,ix]*dist
                print(ix,iy,iy*self.xnbin+ix,"LinearInterp",dist,distSum,"data",self.data[iy,ix],dataSum
                      ,self.xmin+xstep*ix,self.ymin+ystep*iy)
        dat = dataSum / distSum
        print("DAT",dat,dataSum,distSum)

        return dat
    
 
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetInterpolationFunction(self) :
        xBins = self.Xbins()
        yBins = self.Ybins()

        xarr = np.array(xBins)
        yarr = np.array(yBins)        

#        mask = self.dataNP != 0        
        interp = interp2d(xarr, yarr, self.dataNP, kind='cubic')
#        interp = interp2d(xarr[mask], yarr[mask], self.dataNP[mask], kind='cubic')

        return interp
    

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetIdInterpolatedValue(self, xval,yval) :
        xBins = self.Xbins()
        ibinx = -1 
        for ib1 in range(len(xBins)) :
            if xBins[ib1] == xval :
                ibinx = ib1
                break
        if ibinx == -1 :
 #           idBigger = [ii for ii, value in enumerate(xBins) if value > xval]
            idBigger = next(xx for xx, val in enumerate(xBins)
                            if val > xval)
            ibinx = idBigger

        yBins = self.Ybins()
        ibiny = -1 
        for ib1 in range(len(yBins)) :
            if yBins[ib1] == yval :
                ibiny = ib1
                break
        if ibiny == -1 :
 #           idBigger = [ii for ii, value in enumerate(yBins) if value > yval]
            idBigger = next(yy for yy, val in enumerate(yBins)
                            if val > yval)
            ibiny = idBigger

        return ibinx,ibiny
    
    ### NORM TO INTEGRAL
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def norm(self,normValue):
        dataSum = np.sum(self.dataNP)
#        dataSum2 = sum(self.data)
        print("DATASUM",dataSum)
        if dataSum != 0 :
            factor = normValue/dataSum
        else :
            factor = 1.
            #        print(self.name,"NORM ",len(self.data),self.nbin)
        self.dataNP = factor * self.dataNP
        self.dataErrNP = factor * self.dataErrNP
        
