import tkinter as tk
from tkinter import *
from csvHistoReader import *
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg,NavigationToolbar2Tk
import numpy as np
sys.path.append('PATH')
from PIL import Image as im
from tkinter import BooleanVar
import tkinter.font as tkFont
from math import *

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
class UI(tk.Frame):
    """Docstring."""
    verbose = 1
    onevar = 0
    histList = None
    fHistos1D = []
    fHistos2D = []
    fHistoFiles = []
    fHisto2Histo1file = {}
    fHisto2Histo2file = {}
    fFileListLinePos = {}
    bHistoShown = {}
    resiz = 0
    histShown = False
    thePanedWindow = None
    theFrameHL = None
    theFrameCheckBoxHL = None
    theFrameHF = None
    theFrameCheckBoxHF = None
    theHistoFigure = None
    theCanvas = None
    theSubPlotArea = None
#-    theFigureAgg = None
    theCanvasCwid = None
    bPlotErrors = None
    bPlotSame = None
    bPlotNorm = None
    bPlotLogX = None
    bPlotLogY = None
    bResizing = False
    newWidth = 7.0
    parentWidth = 1192
#   parentHeight = 821
    lastHisto1 = None
    prevHisto1N = None
    #    photo = PhotoImage(file="cat.png")

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
#    def __init__(self, parent=None, *args):
    def __init__(self, parent, *args):
        # Bind the function to configure the parent window
        parent.bind("<Configure>", self.ResizeListBoxAndImage)
        self.parentWidth = 1192
        #---- Define frames and pannedWindow
        tk.Frame.__init__(self, parent,bg='grey', width=637, height=898, borderwidth=10, relief='sunken')#, pady=3)
        self.parent = parent #??
    #    self.pack(fill="both",expand=True)
        self.grid(column=0, row=0, sticky=tk.W)

        self.columnconfigure(0, weight = 1)
        self.rowconfigure(0, weight = 1)        
        
        self.thePanedWindow = tk.PanedWindow(self,orient="horizontal")
        self.thePanedWindow.grid(column=0, row=0, sticky=tk.W)
#        self.thePanedWindow.pack(fill="both",expand=True) # Window,height=798,width=437, background="red")
        
        self.theFrameHL = tk.Frame(self.thePanedWindow,height=898,width=437)#, background="blue")
        self.thePanedWindow.add(self.theFrameHL)
        self.theFrameHL.columnconfigure(0, weight=1)
        self.theFrameHL.rowconfigure(1, weight=1)
       
        self.theFrameHF = tk.Frame(self.thePanedWindow,height=700,width=700) #, background="red")
        self.thePanedWindow.add(self.theFrameHF)
        self.theFrameHF.columnconfigure(0, weight=1)
        self.theFrameHF.rowconfigure(1, weight=1)

        self.theFrameCheckBoxHF = tk.Frame(self.theFrameHF,height=100,width=700)#, background="blue")
        self.theFrameCheckBoxHF.grid(column=0, row=0, sticky=tk.W)

        self.ReadHistoFiles( args )

        self.CreateWidgets()


    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def ReadHistoFiles( self, args ): 

        if self.verbose >= 3 : print("N ARGS ",len(args),"  ",args)
        iihf = 1
        iih1 = 0
        iih2 = 0
        for hflist in args:
            if self.verbose >= 3 : print("ARG= ",len(hflist),"  ",hflist)
            for hfName in hflist:
        #    for hfName in hflist:
                if self.verbose >= 3 : print("hfName",hfName)
                if hfName == sys.argv[0]: continue
                if self.verbose >= 2 : print("READING HISTO FILE: ",hfName)
                self.fHistoFiles.append(hfName)
                self.bHistoShown[iihf] = True
                histoFile = csvHistoReader(hfName)
                #                self.fHistos1D.extend(histoFile.fHistos1D)
                for his1 in histoFile.fHistos1D :
                    self.fHistos1D.append(his1)
                    self.fHisto2Histo1file[iih1] = iihf
                    if self.verbose >= 2 : print(iih1," READ fHistos1D: ",his1.name," fHisto2Histo1file ",self.fHisto2Histo1file[iih1])
                    iih1 += 1
                for his2 in histoFile.fHistos2D :
                    self.fHistos2D.append(his2)
                    self.fHisto2Histo2file[iih2] = iihf
                    if self.verbose >= 2 : print(iih2," READ fHistos2D: ",his2.name," fHisto2Histo2file ",self.fHisto2Histo2file[iih2])
                    iih2 += 1
                iihf += 1

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    #def showCheckButtons(self):
    #   print("CBError",self.bPlotErrors.get())
    #  print("CBSame",self.bPlotSame.get())

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CreateWidgets(self):
        ##### LIST OF HISTOGRAMS
        self.theFrameCheckBoxHL = tk.Frame(self.theFrameHL,height=100,width=437)#, background="blue")
        self.theFrameCheckBoxHL.grid(column=0, row=0, sticky=tk.W)

        bPlotErrors = tk.BooleanVar(value=True)
        self.CBError = tk.Checkbutton(self.theFrameCheckBoxHL, text="error",
                                      variable=bPlotErrors,onvalue=True,offvalue=False)#,command=self.showCheckButtons)
        self.CBError.grid(column=0, row=0, sticky=tk.W)
        self.bPlotErrors = bPlotErrors
        # self.CBError.invoke()

        bPlotSame = tk.BooleanVar()
        self.CBSame = tk.Checkbutton(self.theFrameCheckBoxHL, text="same",
                                     variable=bPlotSame,state=tk.DISABLED) #,command=self.showCheckButtons)
        self.CBSame.grid(column=2, row=0, sticky=tk.W)
        self.bPlotSame = bPlotSame

        bPlotNorm = tk.BooleanVar()
        self.CBNorm = tk.Checkbutton(self.theFrameCheckBoxHL, text="Norm",
                                     variable=bPlotNorm,state=tk.DISABLED) #,command=self.showChecknButtons)
        self.CBNorm.grid(column=1, row=0, sticky=tk.W)
        self.bPlotNorm = bPlotNorm

        lbSBV = tk.Scrollbar(self.theFrameHL,orient=tk.VERTICAL)#,command = self.theFrameHL.histList.yview)
        lbSBV.grid(column=1, row=1, sticky=tk.N+tk.S)
        lbSBH = tk.Scrollbar(self.theFrameHL,orient=tk.HORIZONTAL)#,command = self.theFrameHL.histList.yview)
        lbSBH.grid(column=0, row=2, sticky=tk.W+tk.E)
        hlWidthMax = 80
        hlWidth = 0
        for hisf in self.fHistoFiles :
            hl1 = len(hisf)
            hlWidth = max(hlWidth,hl1)
            #            print("hlWidth ",hlWidth,"  ",hl1)
            if hlWidth > hlWidthMax :
                hlWidth = hlWidthMax
                break
        for his in self.fHistos1D :
            hl1 = len(his.name)
            hlWidth = max(hlWidth,hl1)
            #            print("hlWidth ",hlWidth,"  ",hl1)
            if hlWidth > hlWidthMax :
                hlWidth = hlWidthMax
                break    
        for his in self.fHistos2D :
            hl2 = len(his.name)
            hlWidth = max(hlWidth,hl2)
            #            print("hlWidth ",hlWidth,"  ",hl2)
            if hlWidth > hlWidthMax :
                hlWidth = hlWidthMax
                break
        hlHeightMax = 40
        hlHeight = min(len(self.fHistos1D)+len(self.fHistos2D)+len(self.fHistoFiles),hlHeightMax)
        hlHeight = 40
 #        hlHeight = (self.theFrameCheckBoxHF.height()-self.theFrameHF.height())/10.
       
        self.histList = tk.Listbox(self.theFrameHL,height=hlHeight,width=hlWidth
                                   ,yscrollcommand=lbSBV.set,xscrollcommand=lbSBH.set)# selectmode='extended'),)
        ### INSERT HISTOGRAMS IN LIST
        self.FillListBox()
#        self.histList.config(yscrollcommand=lbSB)
        lbSBV['command'] = self.histList.yview
        lbSBH['command'] = self.histList.xview

        ### HISTOGRAMS logX logY
        bPlotLogX = tk.BooleanVar()
        self.CBLogX = tk.Checkbutton(self.theFrameCheckBoxHF, text="logX",width=10,
                                     variable=bPlotLogX,state=tk.NORMAL) #,command=self.showCheckButtons)
        self.CBLogX.grid(column=2, row=0, sticky=tk.W)
        self.bPlotLogX = bPlotLogX

        bPlotLogY = tk.BooleanVar()
        self.CBLogY = tk.Checkbutton(self.theFrameCheckBoxHF, text="logY",width=60,
                                     variable=bPlotLogY,state=tk.NORMAL) #,command=self.showCheckButtons)
        self.CBLogY.grid(column=3, row=0, sticky=tk.W)
        self.bPlotLogY = bPlotLogY

        self.showStat()

        #### FIT button
        self.fitBut = tk.Button(self.theFrameHL, text="Gaussian Fit", command=self.GaussianFit, font= ('Helvetica 14 bold italic'), state=tk.DISABLED) 
        self.fitBut.grid(column=1, row=3)
      
        #### QUIT button
        self.quitBut = tk.Button(self.theFrameHL, text="Quit", command=self.quit, font= ('Helvetica 14 bold italic'))
        self.quitBut.grid(column=0, row=3)

#        if self.verbose >= 3 : print("FRAME SIZE",self.parent.winfo_height(), self.parent.winfo_width())
#        if self.verbose >= 3 : print("histList SIZE", self.histList.winfo_height(),  self.histList.winfo_width())
#        self.parent.geometry('{}x{}'.format(y, x))
        return

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def FillListBox(self):
        self.histList.delete(0,tk.END)
        self.fFileListLinePos.clear()
        iihf = 1 # index of file
        iil = 0 # index of line in histList
        for hisFile1 in self.fHistoFiles:
            if self.verbose >= 2 : print(iil, " INSERT HISTO FILE: ",hisFile1)
            self.fFileListLinePos[iil] = (0,iihf)
#            print(iil,"F fFileListLinePos ",self.fFileListLinePos[iil]) # GDEB
            self.histList.insert(iil,hisFile1) # +str(iihf))
            iil += 1
            iih = 0
            for his1 in self.fHistos1D:
                if self.bHistoShown.get(iihf) :     
#                    print("HISTO1: ",his1)
                    if self.fHisto2Histo1file.get(iih) == iihf:
                        if self.verbose >= 2 : print(iil, " INSERT HISTO1D: ",his1.name," === ",self.fHisto2Histo1file.get(iih)," == ",iih)
                        self.histList.insert(iil,"  "+his1.name) #+str(iih))
                        self.fFileListLinePos[iil] = (1,iih)
#                        print(iil,"1 fFileListLinePos ",self.fFileListLinePos[iil]) # GDEB
                        if self.verbose >= 3 : print(" SET self.fFileListLinePos ",iih, " = ",self.fFileListLinePos[iil])
                        iil+=1
                iih = iih+1      # index of histogram in fHistos1D

            iih = 0
            for his2 in self.fHistos2D:
                if self.bHistoShown.get(iihf) :     
                #            print("HISTO2: ",his2)
                    if self.fHisto2Histo2file.get(iih) == iihf:
                        if self.verbose >= 2 : print(iil, " INSERT HISTO2D: ",his2.name," === ",self.fHisto2Histo2file.get(iih)," == ",iih)
                        self.histList.insert(iil,"  "+his2.name) #+str(iih))
                        self.fFileListLinePos[iil] = (2,iih)
                        if self.verbose >= 3 : print(" SET self.fFileListLinePos ",iih, " = ",self.fFileListLinePos[iil])
                        iil+=1
                iih = iih+1      # index of histogram in fHistos1D
            iihf += 1
        self.histList.grid(column=0, row=1, sticky=tk.E+tk.W+tk.N+tk.S)#, sticky=tk.W)
        self.histList.bind('<<ListboxSelect>>', self.SelectHist)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def SelectHist(self,event):
        selected_index = int(event.widget.curselection()[0])
        iil = self.fFileListLinePos.get(selected_index)
        if self.verbose >= 3 : print("@@ selectHist ",selected_index," : ",iil)
#        print(" READ self.fFileListLinePos ",selected_index, " = ",self.fFileListLinePos.get(selected_index)) # GDEB
        if iil[0] == 0 : # show / unshow histograms in file
            self.bHistoShown[abs(iil[1])] = not self.bHistoShown.get(abs(iil[1]))
            self.FillListBox()
                
        else: # select histogram
            #        selected_value = event.widget.get(selected_index)
            if self.verbose >= 2 : print("@@@@ SELECTING ",self.fHistos1D[iil[1]].name)
            self.showHist(iil[0],iil[1])

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def showHist(self,it,ih):
        self.textYpos = 0.85

        if it == 1:
            his1 = self.fHistos1D[ih]
#            print("@@#@#@@ SHOWHIST ",ih,his1,self.histShown) # GDEB
            self.histShown = True
            self.CBSame.config(state=tk.NORMAL)
            self.CBNorm.config(state=tk.NORMAL)
            self.CBLogX.config(state=tk.NORMAL)
            self.CBLogY.config(state=tk.NORMAL)
            self.fitBut.config(state=tk.NORMAL)
            #            if not self.bPlotSame :
            figWidth = 7
            figHeight = 7
            if  self.theHistoFigure != None:
                figWidth = self.theHistoFigure.get_figwidth()
                figHeight = self.theHistoFigure.get_figheight()
                
            self.theHistoFigure = plt.figure(figsize = (figWidth,figHeight), dpi = 100, )
            #@#@ adding the subplot
            self.theSubPlotArea = self.theHistoFigure.add_subplot(111)
            self.theSubPlotArea.set_xlabel(his1.name)
            if self.verbose >= 3 : print(" PLOT HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",his1.Xbins())
            # plot1.xlim(float(his1.xmin),float(his1.xmax))
            #       plot1.xlabel(his1.name)
            #         print("bPlotErrors ",self.bPlotErrors.get())# GDEB
            #@#@ Choose logarithmic or linear scale
            if self.bPlotLogX.get() :
                plt.xscale('log')
            else : 
                plt.xscale('linear')
            if self.bPlotLogY.get() :
                plt.yscale('log')
            else : 
                plt.yscale('linear')

            #@#@#@ PLOT HISTOGRAM
            #@#@ PLOT WITH ERRORS
            if self.bPlotErrors.get() :
                #@#@ PLOT WITH ERRORS AND SAME
                if self.bPlotSame.get() :
                    self.prevHisto1N = Histo1D()
                    self.prevHisto1N.copy(self.lastHisto1)
                    hisCurrentN = Histo1D()
                    hisCurrentN.copy(his1)
#                    print("hisCurrentN ",hisCurrentN.nbin,hisCurrentN.xmin,hisCurrentN.xmax)
#                    print("his1 ",his1.nbin,his1.xmin,his1.xmax)
                    if self.bPlotNorm.get() :
                        self.prevHisto1N.norm(1.)
                        hisCurrentN.norm(1.)
                    #                   print("N hisCurrentN ",hisCurrentN.nbin,hisCurrentN.xmin,hisCurrentN.xmax,hisCurrentN.Xbins())
                    #                  print("N his1 ",his1.nbin,his1.xmin,his1.xmax,his1.Xbins())
                    #                    self.theSubPlotArea.bar(self.prevHisto1N.Xbins(),self.prevHisto1N.data, color='None', errcolor='blue', yerr=self.prevHisto1N.dataErr, width=xStep,linewidth=0)
                    hy = self.prevHisto1N.data
                    plt.xlim(self.prevHisto1N.xmin, self.prevHisto1N.xmax)
                    self.theSubPlotArea.errorbar(self.prevHisto1N.Xbins(),hy, color='black', yerr=self.prevHisto1N.dataErr)
                    self.theSubPlotArea.errorbar(hisCurrentN.Xbins(),hisCurrentN.data,yerr=hisCurrentN.dataErr,color='red')
                    xStep = self.prevHisto1N.Xstep(self.prevHisto1N)
                    for ii in range(self.prevHisto1N.nbin):
                        self.theSubPlotArea.scatter(self.prevHisto1N.xmin+xStep*(ii+0.5)
                                                    ,self.prevHisto1N.data[ii],color='black',s=10)
                    xStep = his1.Xstep(his1)
                    for ii in range(hisCurrentN.nbin):
                        self.theSubPlotArea.scatter(hisCurrentN.xmin+xStep*(ii+0.5)
                                                   ,hisCurrentN.data[ii],color='red',s=10)
                    self.printStatistics(self.prevHisto1N)
                    self.textYpos -= 0.01
                    self.printStatistics(hisCurrentN)
                else :
                #@#@ PLOT WITH ERRORS AND NOT SAME
#                    self.theSubPlotArea.bar(his1.Xbins(),his1.data, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep,linewidth=0)
                    hx = his1.Xbins()
                    hy = his1.data
                    plt.xlim(his1.xmin, his1.xmax)
                    self.theSubPlotArea.errorbar(hx,hy, color='black', yerr=his1.dataErr)
                    xStep = his1.Xstep(his1)
                    for ii in range(his1.nbin):
#                        self.theSubPlotArea.scatter(his1.xmin+xStep*(ii+0.5)
                        self.theSubPlotArea.scatter(hx[ii]
                                                ,hy[ii],color='black',s=10)
                        #print(ii,"POINT ",his1.xmin+xStep*(ii+0.5),his1.data[ii])
                    self.printStatistics(his1)

            #@#@ PLOT WITH NO ERRORS 
            else :  
                if self.bPlotSame.get() :
                #@#@ PLOT WITH NO ERRORS AND SAME
                    self.prevHisto1N = Histo1D()
                    self.prevHisto1N.copy(self.lastHisto1)
                    hisCurrentN = Histo1D()
                    hisCurrentN.copy(his1)
                    if self.bPlotNorm.get() :
                        self.prevHisto1N.norm(1.)
                        hisCurrentN.norm(1.)
                 #   print("hisCurrentN ",hisCurrentN.nbin,hisCurrentN.xmin,hisCurrentN.xmax)
                 #   print("his1 ",his1.nbin,his1.xmin,his1.xmax)
                    plt.xlim(self.prevHisto1N.xmin, self.prevHisto1N.xmax)
                                        
                    self.theSubPlotArea.plot(self.prevHisto1N.Xbins(),self.prevHisto1N.data,color='black') #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    self.theSubPlotArea.plot(hisCurrentN.Xbins(),hisCurrentN.data,color='red') #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    self.printStatistics(self.prevHisto1N)
                    self.textYpos -= 0.01
                    self.printStatistics(hisCurrentN)
                else:
                #@#@ PLOT WITH NO ERRORS AND NOT SAME
                    plt.xlim(his1.xmin, his1.xmax)
                    self.theSubPlotArea.plot(his1.Xbins(),his1.data,color='black') #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    self.printStatistics(his1)
                    
            self.theCanvas = FigureCanvasTkAgg(self.theHistoFigure, master = self.theFrameHF)
            self.theCanvas.draw()
            #n           self.theCanvas.show()
            self.theCanvasCwid = self.theCanvas.get_tk_widget()
            self.theCanvasCwid.grid(column=0, row=1, sticky=tk.E+tk.W+tk.N+tk.S)
            
            toolbarFrame = tk.Frame(master=self.theFrameHF)
            toolbarFrame.grid(column=0,row=2, sticky=tk.E+tk.W+tk.N+tk.S)
            toolbar = NavigationToolbar2Tk(self.theCanvas, toolbarFrame)
            #        if self.verbose >= 3 : print("theSubPlotArea SIZE", self.theSubPlotArea.winfo_height(),  self.theSubPlotArea.winfo_width())
            #if self.verbose >= 3 : print("theCanvas SIZE", self.theCanvas.height(),  self.theCanvas.width())
            #        ROOT.geometry("1137x798")
            self.lastHisto1 = his1

        elif it == 2:
            his2 = self.fHistos2D[ih]
            #           print("@@#@#@@ SHOWHIST ",ih,his2) # GDEB

#  >>> img = Image.new('1', (50, 50))
#>>> pixels = img.load()
#>>> for i in range(img.size[0]):
#...    for j in range(img.size[1]):
#...        pixels[i, j] = data[i][j]
#>>> img.show()
#>>> img.save('/tmp/image.bmp')

            self.histShown = True
            self.theHistoFigure = plt.figure(figsize = (his2.xnbin,his2.ynbin), dpi = 100)
#t            self.theHistoFigure = plt.figure(figsize = (7, 7), dpi = 10)
            # adding the subplot
            self.theSubPlotArea = self.theHistoFigure.add_subplot(111)
            self.theSubPlotArea.set_xlabel(his2.name)
            hX = his2.Xbins(his2)
            hY = his2.Ybins(his2)
            #            hY = his2.Ybins()
            hZ = his2.data
            #            if self.verbose >= 3 : print(" PLOT HIS2 ",his2,":",his2.data,"+-",his2.dataErr," Xs=",hX)
            #            print(his2.name,"DATA ARRAY PLOTTED ",his2.dataArray)
            #https://www.freesion.com/article/7746457300/
            cmap = mpl.cm.get_cmap("Blues").copy()
            self.theSubPlotArea.imshow(his2.dataArray,origin="lower",extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),aspect='auto',interpolation='none', cmap=cmap) #,norm=LogNorm(vin=0.001,vmax=1))
            self.theCanvas = FigureCanvasTkAgg(self.theHistoFigure, master = self.theFrameHF)
            self.theCanvas.draw()
            self.theCanvasCwid = self.theCanvas.get_tk_widget()
            self.theCanvasCwid.grid(column=0, row=0, sticky=tk.E+tk.W+tk.N+tk.S)
            toolbarFrame = tk.Frame(master=self.theFrameHF)
            toolbarFrame.grid(column=0,row=2, sticky=tk.E+tk.W+tk.N+tk.S)
            toolbar = NavigationToolbar2Tk(self.theCanvas, toolbarFrame)
            

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def ResizeListBoxAndImage(self,event):
        return
        if self.verbose >= 3 : print("#@#@#@### ENTER  ResizeListBoxAndImage ",self.bResizing)
        if self.bResizing :
            return
        #        if self.theCanvas != None :             return
        #        self.grid_remove()
        #        self.grid(sticky=tk.N+tk.S+tk.W+tk.E) # histList SIZE 404 724    364  618 
#        if self.verbose >= 3 : print("ResizeListBoxAndImage :FRAME SIZE",self.theFrameHF.winfo_width(), self.theFrameHF.winfo_height(), self.parent.winfo_width(),self.parent.winfo_height())
        if self.verbose >= 3 : print("ResizeListBoxAndImage :FRAME SIZE",self.parentWidth,"?",self.parent.winfo_width(),self.parentHeight,"=?",self.parent.winfo_height())
#        if self.parent.winfo_width() == 1192 :
        if self.parent.winfo_width() == self.parentWidth :
            return
#        if self.parent.winfo_height() == 821 :
        if self.parent.winfo_height() == self.parentHeight :
            return
#        if self.parent.winfo_height() != 821 :
        if self.parent.winfo_height() != self.parentHeight :
#        if self.theFrameHL.winfo_height() != 778 :
            if self.verbose >= 3 : print("ResizeListBoxAndImage: CHANGE? histList ", self.histList,self.histShown)        
            ### Change histList if no figure (else figure frame change changes histList frame)
            if self.histList != None :
                if self.histShown == False : 
                    width = int((self.parent.winfo_width()-33)/8)
                else : 
                    width = int((self.parent.winfo_width()-733)/8)                
                    self.histList.config(height = int((self.parent.winfo_height()-74)/18) )
                #            self.histList.config(height = 30)
                #            self.histList.grid_remove()
                #            self.histList.grid(column=0, row=0, sticky=tk.E+tk.W+tk.N+tk.S)#, sticky=tk.W)
                if self.verbose >= 3 : print("ResizeListBoxAndImage: NEW histList SIZE", self.histList.winfo_width(),self.histList.winfo_height(),self.theFrameHL.winfo_height())        

        #----- Resize image
        if self.histShown :
        # if self.theHistoFigure :
            self.bResizing = True
            if self.verbose >= 3 : print("ResizeListBoxAndImage: FIGURE SIZE", self.theHistoFigure.get_size_inches()," in ",self.theFrameHF.winfo_width(),self.theFrameHF.winfo_height())
            factor = self.theFrameHF.winfo_width()/100/self.newWidth
            self.newWidth = self.theFrameHF.winfo_width()/100
            newHeight = (self.theFrameHF.winfo_height()-34)/100
            if self.verbose >= 3 : print("ResizeListBoxAndImage: NEW WIDTH HEIGHT",self.newWidth,newHeight," FACTOR= ",factor,self.theHistoFigure.get_figwidth(),self.theHistoFigure.get_figheight())
            self.theHistoFigure.set_figwidth(self.newWidth)
            self.theHistoFigure.set_figheight(newHeight)
            if self.verbose >= 3 : print("ResizeListBoxAndImage: 1CHANGED FIGURE SIZE", self.theHistoFigure.get_size_inches())
            if factor != 1.0 :
                self.theSubPlotArea.xaxis.label.set_fontsize(self.theSubPlotArea.xaxis.label.get_fontsize()*factor)
            
                #            self.frameHF.refresh()
                print("WIDTH ",self.newWidth,newHeight*100)
                self.theCanvasCwid.configure(width=self.newWidth, height=newHeight*100)
            self.theCanvasCwid.update_idletasks()
            if self.verbose >= 3 : print("ResizeListBoxAndImage: 2CHANGED FIGURE SIZE", self.theHistoFigure.get_size_inches())
            
            self.bResizing = False
            self.parentWidth = self.parent.winfo_width()
            self.parentHeight = self.theFrameHF.winfo_height()
            return
                
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def showStat(self,) :
        statLabel = tk.Label(self.theFrameCheckBoxHF,text="statistics")
        statLabel.grid(column=0, row=0, sticky=tk.E)
        
        fontList = tkFont.Font(family="Arial", size=11) #, weight="bold", slant="italic")
        self.StatText = tk.Text(self.theFrameCheckBoxHF,height=1,width=10,font=fontList)
        self.StatText.insert('1.0',"0")
        #print("STAT TEXT ",StatText.get('1.0',tk.END))
        self.StatText.grid(column=1, row=0, sticky=tk.W)

        

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def printStatistics(self,histo) :
        optStat = self.StatText.get('1.0',tk.END).rstrip()
        if int(optStat) == 0 :
            return
        #  print(optStat,"STA",len(optStat))
        bShowErr = 0
        iimax = len(optStat)
        ### CHECK
        for ii in range(iimax) :
            ista = int(optStat[ii])
            if ista >= 2 :
                bShowErr = 1
                
        for ii in range(iimax) :
            ista = int(optStat[ii])
            # print(ii,"ISTA",ista)
            t2show = ""
            if ii == 0 :
                if ista >= 1 :
                    t2show += "Entri= "+"{:.2e}".format(histo.nent)  
            elif ii == 1 :
                if ista >= 1 :
                    t2show += "mean=  "+"{:.2e}".format(histo.mean)                    
                if ista >= 2 :
                    t2show += "+-"+"{:.2e}".format(histo.meanErr)
            elif ii == 2 :
                if ista >= 1 :
                    t2show += "RMS=   "+"{:.2e}".format(histo.RMS)
                if ista >= 2 :
                    t2show += "+-"+"{:.2e}".format(histo.RMSErr)
            elif ii == 3 :
                if ista >= 1 :
                    t2show += "Over=  "+"{:.2e}".format(histo.over)
                if ista >= 2 :
                    t2show += "+-"+"{:.2e}".format(histo.overErr)
            elif ii == 4 :
                if ista >= 1 :
                    t2show += "Under= "+"{:.2e}".format(histo.under)
                if ista >= 2 :
                    t2show += "+-"+"{:.2e}".format(histo.underErr)

            self.theHistoFigure.text(0.72-bShowErr*0.11,self.textYpos,t2show)
            if ista > 0 :
                self.textYpos -= 0.025

            if iimax == 0: return # do not loop 0 twice when range(0,0)

        
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GaussianFit(self) :
        #        print("start Gaussian Fit")
        param,cov = self.lastHisto1.GaussianFit(0, True)
        if self.bPlotSame.get() :
            param,cov = self.prevHisto1N.GaussianFit(1,True)

            
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
if __name__ == "__main__":
    ROOT = tk.Tk()
    ROOT.title("GAMOS Python Histogram Browser")
    ROOT.geometry("")  #437 798")
    ROOT.columnconfigure(0, weight=1)
    ROOT.rowconfigure(0, weight=1)
    #- sys.argv.append("F:/0FromDisk/pythonUtilities/dose_RTPSPDoseHistos.exercise4b.Dose.csv")
    APP = UI(ROOT,sys.argv)
#    APP = UI(sys.argv[1])
    APP.mainloop()
    ROOT.destroy()
