import tkinter as tk
from tkinter import *
from csvHistoReader import *
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.colors import LogNorm
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg,NavigationToolbar2Tk
import numpy as np
sys.path.append('PATH')
from PIL import Image as im
from tkinter import BooleanVar
import tkinter.font as tkFont
from functools import reduce

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
class UI(tk.Frame):
    """Docstring."""
    verbose = 1
    histList = None
    fHistos1D = []
    fHistos2D = []
    fHistoFiles = []
    fHisto2Histo1file = {}
    fHisto2Histo2file = {}
    fFileListLinePos = {}
    bHistoShown = {}
    histShown = False
    thePanedWindow = None
    theFrameHL = None
    theFrameCheckBoxHL = None
    theFrameHF = None
    theFrameCheckBoxHF = None
    theHistoFigure = None
    theCanvas = None
    theSubPlotArea = None
    theCanvasCwid = None
    bPlotErrors = None
    bPlotSame = None
    bPlotNorm = None
    bPlotLogX = None
    bPlotLogY = None
    b1DPlotType = None
    b2DPlotType = None
    lastHisto1 = None
    prevHisto1N = None

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
#    def __init__(self, parent=None, *args):
    def __init__(self, parent, *args):
        #---- Define frames and pannedWindow
        tk.Frame.__init__(self, parent,bg='grey', width=637, height=898, borderwidth=10, relief='sunken')#, pady=3)
        self.parent = parent
        self.grid(column=0, row=0, sticky=tk.NSEW)

        self.columnconfigure(0, weight = 1)
        self.rowconfigure(0, weight = 1)        
        
        self.thePanedWindow = tk.PanedWindow(self, orient="horizontal", sashwidth=5)
        self.thePanedWindow.grid(column=0, row=0, sticky=tk.NSEW)

        self.theFrameHL = tk.Frame(self.thePanedWindow, width=460)
        self.thePanedWindow.add(self.theFrameHL, minsize=360, width=460)
        self.theFrameHL.columnconfigure(0, weight=1)
        self.theFrameHL.rowconfigure(1, weight=1)

        self.theFrameHF = tk.Frame(self.thePanedWindow)
        self.thePanedWindow.add(self.theFrameHF, minsize=300, stretch="always")
        self.theFrameHF.columnconfigure(0, weight=1)
        self.theFrameHF.rowconfigure(1, weight=1)

        self.theFrameCheckBoxHF = tk.Frame(self.theFrameHF)
        self.theFrameCheckBoxHF.grid(column=0, row=0, sticky=tk.EW)

        self.ReadHistoFiles( args )

        self.CreateWidgets()


    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def ReadHistoFiles( self, args ): 

        if self.verbose >= 3 : print("N ARGS ",len(args),"  ",args)
        iihf = 1
        iih1 = 0
        iih2 = 0
        for hflist in args:
            if self.verbose >= 3 : print("ReadHistoFiles ARG= ",len(hflist),"  ",hflist)
            for hfName in hflist:
                if self.verbose >= 3 : print("hfName",hfName)
                if hfName == sys.argv[0]: continue
                if self.verbose >= 2 : print("ReadHistoFiles READING HISTO FILE: ",hfName)
                self.fHistoFiles.append(hfName)
                self.bHistoShown[iihf] = True
                histoFile = csvHistoReader(hfName)
                for his1 in histoFile.fHistos1D :
                    self.fHistos1D.append(his1)
                    self.fHisto2Histo1file[iih1] = iihf
                    if self.verbose >= 2 : print("ReadHistoFiles",iih1," READ fHistos1D: ",his1.name," fHisto2Histo1file ",self.fHisto2Histo1file[iih1])
                    iih1 += 1
                for his2 in histoFile.fHistos2D :
                    self.fHistos2D.append(his2)
                    self.fHisto2Histo2file[iih2] = iihf
                    if self.verbose >= 2 : print("ReadHistoFiles",iih2," READ fHistos2D: ",his2.name," fHisto2Histo2file ",self.fHisto2Histo2file[iih2])
                    iih2 += 1
                iihf += 1

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def CreateWidgets(self):
        ##### LIST OF HISTOGRAMS
        self.theFrameCheckBoxHL = tk.Frame(self.theFrameHL,height=100,width=437)#, background="blue")
        self.theFrameCheckBoxHL.grid(column=0, row=0, sticky=tk.W)

        bPlotErrors = tk.BooleanVar(value=False)
        self.CBError = tk.Checkbutton(self.theFrameCheckBoxHL, text="error",
                                      variable=bPlotErrors,onvalue=True,offvalue=False)#,command=self.showCheckButtons)
        self.CBError.grid(column=0, row=0, sticky=tk.W)
        self.bPlotErrors = bPlotErrors

        bPlotSame = tk.BooleanVar()
        self.CBSame = tk.Checkbutton(self.theFrameCheckBoxHL, text="same",
                                     variable=bPlotSame,state=tk.DISABLED) #,command=self.showCheckButtons)
        self.CBSame.grid(column=1, row=0, sticky=tk.W)
        self.bPlotSame = bPlotSame

        bPlotNorm = tk.BooleanVar()
        self.CBNorm = tk.Checkbutton(self.theFrameCheckBoxHL, text="norm",
                                     variable=bPlotNorm,state=tk.DISABLED) #,command=self.showChecknButtons)
        self.CBNorm.grid(column=2, row=0, sticky=tk.W)
        self.bPlotNorm = bPlotNorm

        ### Keep the listbox and its scrollbars in their own grid.  The fit/quit
        ### buttons must not share these grid columns: their widths were pushing
        ### the vertical scrollbar far away from the visible listbox.
        self.theFrameList = tk.Frame(self.theFrameHL)
        self.theFrameList.grid(column=0, row=1, sticky=tk.NSEW)
        self.theFrameList.columnconfigure(0, weight=1)
        self.theFrameList.rowconfigure(0, weight=1)

        lbSBV = tk.Scrollbar(self.theFrameList, orient=tk.VERTICAL)
        lbSBV.grid(column=1, row=0, sticky=tk.NS)
        lbSBH = tk.Scrollbar(self.theFrameList, orient=tk.HORIZONTAL)
        lbSBH.grid(column=0, row=1, sticky=tk.EW)

        hlWidth = 42
        hlHeight = 40
        self.histList = tk.Listbox(
            self.theFrameList, height=hlHeight, width=hlWidth,
            yscrollcommand=lbSBV.set, xscrollcommand=lbSBH.set
        )
        self.histList.grid(column=0, row=0, sticky=tk.NSEW)
        self.FillListBox()
        lbSBV.configure(command=self.histList.yview)
        lbSBH.configure(command=self.histList.xview)

        ### HISTOGRAMS logX logY
        bPlotLogX = tk.BooleanVar()
        self.CBLogX = tk.Checkbutton(self.theFrameCheckBoxHF, text="logX",
                                     variable=bPlotLogX,state=tk.NORMAL) #,command=self.showCheckButtons)
        self.CBLogX.grid(column=2, row=0, sticky=tk.W)
        self.bPlotLogX = bPlotLogX

        bPlotLogY = tk.BooleanVar()
        self.CBLogY = tk.Checkbutton(self.theFrameCheckBoxHF, text="logY",
                                     variable=bPlotLogY,state=tk.NORMAL) #,command=self.showCheckButtons)
        self.CBLogY.grid(column=3, row=0, sticky=tk.W)
        self.bPlotLogY = bPlotLogY

        bPlotLogZ = tk.BooleanVar()
        self.CBLogZ = tk.Checkbutton(self.theFrameCheckBoxHF, text="logZ",
                                     variable=bPlotLogZ,state=tk.NORMAL) #,command=self.showCheckButtons)
        self.CBLogZ.grid(column=4, row=0, sticky=tk.W)
        self.bPlotLogZ = bPlotLogZ

        self.LabelPlotType = tk.Label(
            self.theFrameCheckBoxHF,
            text="1D type:"
        )
        self.LabelPlotType.grid(column=5, row=0, sticky=tk.W, padx=(20, 0))
        b1DPlotType = tk.StringVar(value="Line")
        self.CB1DPlotType = tk.OptionMenu(
            self.theFrameCheckBoxHF,
            b1DPlotType,
            "Line","Bar","Point"
        )
        self.CB1DPlotType.grid(column=6, row=0, sticky=tk.W)
        self.b1DPlotType = b1DPlotType

        self.LabelPlotType = tk.Label(
            self.theFrameCheckBoxHF,
            text="2D type:"
        )
        self.LabelPlotType.grid(column=7, row=0, sticky=tk.W, padx=(20, 0))
        b2DPlotType = tk.StringVar(value="Colormap")
        self.CB2DPlotType = tk.OptionMenu(
            self.theFrameCheckBoxHF,
            b2DPlotType,
            "Colormap", "Lego", "Contour"
        )
        self.CB2DPlotType.grid(column=8, row=0, sticky=tk.W)
        self.b2DPlotType = b2DPlotType

        self.showStat()

        ### Buttons live in a separate frame so they cannot affect the list/scrollbar geometry.
        self.theFrameButtons = tk.Frame(self.theFrameHL)
        self.theFrameButtons.grid(column=0, row=2, sticky=tk.EW, pady=(4, 0))

        self.quitBut = tk.Button(
            self.theFrameButtons, text="Quit", command=self.quit,
            font=('Helvetica 12 bold italic')
        )
        self.quitBut.pack(side=tk.LEFT, padx=(0, 4))

        self.fitBut = tk.Button(
            self.theFrameButtons, text="Gaussian Fit", command=self.GaussianFit,
            font=('Helvetica 12 bold italic')
        )
        self.fitBut.pack(side=tk.LEFT, padx=4)

#        self.fitBut2 = tk.Button(
#            self.theFrameButtons, text="2Gaussian Fit",
#            command=self.DoubleGaussianCenters0Fit,
#            font=('Helvetica 12 bold italic'), state=tk.DISABLED
#        )
#        self.fitBut2.pack(side=tk.LEFT, padx=4)

        return

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def FillListBox(self):
        self.histList.delete(0,tk.END)
        self.fFileListLinePos.clear()
        iihf = 1 # index of file
        iil = 0 # index of line in histList
        for hisFile1 in self.fHistoFiles:
            if self.verbose >= 2 : print("FillListBox",iil, " INSERT HISTO FILE: ",hisFile1)
            self.fFileListLinePos[iil] = (0,iihf)
            self.histList.insert(iil,hisFile1) # +str(iihf))
            iil += 1
            iih = 0
            for his1 in self.fHistos1D:
                if self.bHistoShown.get(iihf) :     
                    if self.fHisto2Histo1file.get(iih) == iihf:
                        if self.verbose >= 2 : print("FillListBox",iil, " INSERT HISTO1D: ",his1.name," === ",self.fHisto2Histo1file.get(iih)," == ",iih)
                        self.histList.insert(iil,"  "+his1.name) #+str(iih))
                        self.fFileListLinePos[iil] = (1,iih)
                        if self.verbose >= 3 : print("FillListBox SET self.fFileListLinePos ",iih, " = ",self.fFileListLinePos[iil])
                        iil+=1
                iih = iih+1      # index of histogram in fHistos1D

            iih = 0
            for his2 in self.fHistos2D:
                if self.bHistoShown.get(iihf) :     
                #            print("HISTO2: ",his2)
                    if self.fHisto2Histo2file.get(iih) == iihf:
                        if self.verbose >= 2 : print("FillListBox",iil, " INSERT HISTO2D: ",his2.name," === ",self.fHisto2Histo2file.get(iih)," == ",iih)
                        self.histList.insert(iil,"  "+his2.name) #+str(iih))
                        self.fFileListLinePos[iil] = (2,iih)
                        if self.verbose >= 3 : print("FillListBox SET self.fFileListLinePos ",iih, " = ",self.fFileListLinePos[iil])
                        iil+=1
                iih = iih+1      # index of histogram in fHistos1D
            iihf += 1
        self.histList.grid(column=0, row=0, sticky=tk.NSEW)
        self.histList.bind('<<ListboxSelect>>', self.SelectHist)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def SelectHist(self,event):
        selection = event.widget.curselection()
        if not selection:
            return

        selected_index = int(event.widget.curselection()[0])
        iil = self.fFileListLinePos.get(selected_index)
        if self.verbose >= 3 : print("SelectHist",selected_index," : ",iil)
        if iil[0] == 0 : # show / unshow histograms in file
            self.bHistoShown[abs(iil[1])] = not self.bHistoShown.get(abs(iil[1]))
            self.FillListBox()
                
        else: # select histogram
            if self.verbose >= 2 : print("SelectHist @@ SELECTING ",self.fHistos1D[iil[1]].name)
            self.showHist(iil[0],iil[1])

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def showHist(self,it,ih):
        self.textYpos = 0.85

        ### Remove the previous canvas and toolbar before placing a new plot.
        previous_width = self.theHistoFigure.get_figwidth() if self.theHistoFigure is not None else 7.0
        previous_height = self.theHistoFigure.get_figheight() if self.theHistoFigure is not None else 5.0
        if getattr(self, "theCanvasCwid", None) is not None:
            try:
                self.theCanvasCwid.destroy()
            except tk.TclError:
                pass
        if getattr(self, "theToolbarFrame", None) is not None:
            try:
                self.theToolbarFrame.destroy()
            except tk.TclError:
                pass
        self.theCanvas = None
        self.theCanvasCwid = None
        self.theToolbarFrame = None

        if it == 1:
            his1 = self.fHistos1D[ih]
#            print("@@ SHOWHIST ",ih,his1,self.histShown) # GDEB
            self.histShown = True
            self.CBSame.config(state=tk.NORMAL)
            self.CBNorm.config(state=tk.NORMAL)
            self.CBLogX.config(state=tk.NORMAL)
            self.CBLogY.config(state=tk.NORMAL)
            self.fitBut.config(state=tk.NORMAL)
            figWidth = previous_width
            figHeight = previous_height
            self.theHistoFigure = Figure(figsize=(figWidth, figHeight), dpi=100)
            ### adding the subplot
            self.theSubPlotArea = self.theHistoFigure.add_subplot(111)
            self.theSubPlotArea.set_xlabel(his1.name)
            if self.verbose >= 3 : print(" PLOT HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",his1.Xbins())

            ### Choose logarithmic or linear scale
            if self.bPlotLogX.get() :
                self.theSubPlotArea.set_xscale('log')
            else : 
                self.theSubPlotArea.set_xscale('linear')
            if self.bPlotLogY.get() :
                self.theSubPlotArea.set_yscale('log')
            else : 
                self.theSubPlotArea.set_yscale('linear')

            #### PLOT HISTOGRAM
            if self.b1DPlotType.get() == "Line" :
                markerType = 'none'
                linestyleType='-'
                drawstyleType='default'
                fmtType='-o' # for error
            elif self.b1DPlotType.get() == "Bar" :
                markerType = 'none'
                linestyleType='-'
                drawstyleType='steps-mid'
                fmtType='-o' # for error
            elif self.b1DPlotType.get() == "Point" :
                markerType = 'o'
                linestyleType='none'
                drawstyleType='default'
                fmtType='o' # for error

            ### PLOT WITH ERRORS
            if self.bPlotErrors.get() :
                ### PLOT WITH ERRORS AND SAME
                if self.bPlotSame.get() :
                    self.prevHisto1N = Histo1D()
                    self.prevHisto1N.copy(self.lastHisto1)
                    hisCurrentN = Histo1D()
                    hisCurrentN.copy(his1)
                    if self.bPlotNorm.get() :
                        self.prevHisto1N.normToValue(1.)
                        hisCurrentN.normToValue(1.)
                    hy = self.prevHisto1N.data
                    self.theSubPlotArea.set_xlim(self.prevHisto1N.xmin, self.prevHisto1N.xmax)
                    self.theSubPlotArea.errorbar(self.prevHisto1N.Xbins(),hy, color='black', yerr=self.prevHisto1N.dataErr,fmt=fmtType,markersize=3,drawstyle=drawstyleType)
                    self.theSubPlotArea.errorbar(hisCurrentN.Xbins(),hisCurrentN.data,yerr=hisCurrentN.dataErr,color='red',fmt=fmtType,markersize=3,drawstyle=drawstyleType)
                    xStep = self.prevHisto1N.Xstep()
                    xStep = his1.Xstep()
                    for ii in range(hisCurrentN.nbin):
                        self.theSubPlotArea.scatter(hisCurrentN.xmin+xStep*(ii+0.5)
                                                   ,hisCurrentN.data[ii],color='red',s=10)
                    self.printStatistics(self.prevHisto1N)
                    self.textYpos -= 0.01
                    self.printStatistics(hisCurrentN)
                else :
                ### PLOT WITH ERRORS AND NOT SAME
                    hx = his1.Xbins()
                    hy = his1.data
                    self.theSubPlotArea.set_xlim(his1.xmin, his1.xmax)
                    self.theSubPlotArea.errorbar(hx,hy, color='black', yerr=his1.dataErr,fmt=fmtType,markersize=3,drawstyle=drawstyleType)
                    xStep = his1.Xstep()
                    self.printStatistics(his1)

            ### PLOT WITH NO ERRORS 
            else :  
                if self.bPlotSame.get() :                    
                ### PLOT WITH NO ERRORS AND SAME
                    self.prevHisto1N = Histo1D()
                    self.prevHisto1N.copy(self.lastHisto1)
                    hisCurrentN = Histo1D()
                    hisCurrentN.copy(his1)
                    if self.bPlotNorm.get() :
                        self.prevHisto1N.normToValue(1.)
                        hisCurrentN.normToValue(1.)
                    self.theSubPlotArea.set_xlim(self.prevHisto1N.xmin, self.prevHisto1N.xmax)
                    self.theSubPlotArea.plot(self.prevHisto1N.Xbins(),self.prevHisto1N.data,color='black',marker=markerType,markersize=3,linestyle=linestyleType,drawstyle=drawstyleType) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    self.theSubPlotArea.plot(hisCurrentN.Xbins(),hisCurrentN.data,color='red',marker=markerType,markersize=3,linestyle=linestyleType,drawstyle=drawstyleType) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    self.printStatistics(self.prevHisto1N)
                    self.textYpos -= 0.01
                    self.printStatistics(hisCurrentN)
                else:
                ### PLOT WITH NO ERRORS AND NOT SAME
                    self.theSubPlotArea.set_xlim(his1.xmin, his1.xmax)
                    hisCurrentN = Histo1D()
                    hisCurrentN.copy(his1)
                    if self.bPlotNorm.get() :
                        self.prevHisto1N.normToValue(1.)
                        hisCurrentN.normToValue(1.)
                    self.theSubPlotArea.plot(hisCurrentN.Xbins(),hisCurrentN.data,color='black',marker=markerType,markersize=3,linestyle=linestyleType,drawstyle=drawstyleType) #, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep) # !!! Does not resize well
                    self.printStatistics(his1)
                    
            # Embed the figure in the Tkinter frame
            self.theCanvas = FigureCanvasTkAgg(self.theHistoFigure, master = self.theFrameHF)
            self.theCanvas.draw()
            self.theCanvasCwid = self.theCanvas.get_tk_widget()
            self.theCanvasCwid.grid(column=0, row=1, sticky=tk.E+tk.W+tk.N+tk.S)
            
            self.theToolbarFrame = tk.Frame(master=self.theFrameHF)
            self.theToolbarFrame.grid(column=0, row=2, sticky=tk.EW)
            self.theToolbar = NavigationToolbar2Tk(self.theCanvas, self.theToolbarFrame)
            self.lastHisto1 = his1

        elif it == 2:
            plt.clf()
            his2 = self.fHistos2D[ih]

            self.histShown = True

            ### Create the figure and axes
            figWidth = previous_width
            figHeight = previous_height
            self.theHistoFigure = Figure(figsize=(figWidth, figHeight), dpi=100)

            if self.b2DPlotType.get() == "Colormap" :
                ### adding the subplot
                self.theSubPlotArea = self.theHistoFigure.add_subplot(111)
                self.theSubPlotArea.set_xlabel(his2.name)
                
                #### Mask zero bins so that they appear white
                data_masked = np.ma.masked_equal(his2.dataNP, 0)
                hX = his2.Xbins()
                hY = his2.Ybins()
            
                ### Color palette
                mycmap = mpl.colormaps["rainbow"].copy()
                mycmap.set_bad("white")
                if self.bPlotLogX.get() :
                    self.theSubPlotArea.set_xscale('log')
                else : 
                    self.theSubPlotArea.set_xscale('linear')
                if self.bPlotLogY.get() :
                    self.theSubPlotArea.set_yscale('log')
                else : 
                    self.theSubPlotArea.set_yscale('linear')
                mynorm = None
                if self.bPlotLogZ.get():
                    valid_data = his2.dataNP[his2.dataNP > 0]
                    vmin = np.min(valid_data) if valid_data.size > 0 else 1e-3
                    vmax = np.max(his2.dataNP) if np.max(his2.dataNP) > 0 else 1.0
                    mynorm = LogNorm(vmin=vmin, vmax=vmax)

                img = self.theSubPlotArea.imshow(data_masked,extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax),cmap=mycmap,origin="lower", aspect="auto", norm=mynorm)
                ### Attach the colorbar to the same figure
                self.theHistoFigure.colorbar(
                    img, ax=self.theSubPlotArea,
                    orientation="vertical"
                )
                
            elif self.b2DPlotType.get() == "Lego" :
                # 1. Add a 3D subplot instead of standard 2D
                self.theSubPlotArea = self.theHistoFigure.add_subplot(111, projection='3d')
                self.theSubPlotArea.set_xlabel(his2.name)

                # 2. Calculate bin widths and setup grid coordinates for 3D bars
                dx_val = (his2.xmax - his2.xmin) / his2.xnbin
                dy_val = (his2.ymax - his2.ymin) / his2.ynbin
                
                x_edges = np.linspace(his2.xmin, his2.xmax - dx_val, his2.xnbin)
                y_edges = np.linspace(his2.ymin, his2.ymax - dy_val, his2.ynbin)
                xpos, ypos = np.meshgrid(x_edges, y_edges, indexing="ij")

                xpos = xpos.ravel()
                ypos = ypos.ravel()
                zpos = np.zeros_like(xpos)
            
                dx = np.full_like(zpos, dx_val)
                dy = np.full_like(zpos, dy_val)
                dz = his2.dataNP.ravel()

                # 3. Filter out zero bins to keep the Lego plot clean   
                mask = dz > 0
                if self.bPlotLogX.get() :
                    mask &= (xpos > 0) & ((xpos + dx_val) > 0)
                if self.bPlotLogY.get() :
                    mask &= (ypos > 0)
                    #mask &= (ypos > 0) & ((ypos + dy_val) > 0)

                xpos = xpos[mask]
                ypos = ypos[mask]
                zpos = zpos[mask]
                dx = dx[mask]
                dy = dy[mask]
                dz = dz[mask]

                if self.bPlotLogX.get() :
                    x_upper = xpos + dx_val
                    dx = np.log10(x_upper) - np.log10(xpos)
                    xpos = np.log10(xpos)

                if self.bPlotLogY.get() :
                    y_upper = ypos + dy_val
                    dy = np.log10(y_upper) - np.log10(ypos)
                    ypos = np.log10(ypos)

                # 4. Color the bars using a colormap based on bin height (counts)
                mynorm = None
                z_min = dz.min() if dz.size > 0 else 0
                z_max = dz.max() if dz.size > 0 else 1
                
                if self.bPlotLogZ.get():
                    valid_data = dz[dz > 0]
                    vmin = np.min(valid_data) if valid_data.size > 0 else 1e-3
                    vmax = np.max(dz) if np.max(dz) > 0 else 1.0
                    mynorm = LogNorm(vmin=vmin, vmax=vmax)
                else:
                    mynorm = mpl.colors.Normalize(vmin=z_min, vmax=z_max)

                if self.bPlotLogZ.get():
                    # Log scale cannot start at 0. Set a minimum floor for the bases:
                    valid_data = dz[dz > 0]
                    z_floor = np.min(valid_data) if valid_data.size > 0 else 1e-3
                    zpos = np.full_like(xpos, z_floor)
                    # Adjust heights relative to the floor if necessary
    
                mycmap = mpl.colormaps["rainbow"].copy()
                bar_colors = mycmap(mynorm(dz)) # <-- Apply mynorm here!
            
                # 5. Draw the 3D lego bars
                self.theSubPlotArea.bar3d(
                    xpos, ypos, zpos, dx, dy, dz, 
                    color=bar_colors, 
                    zsort='average', 
                    edgecolor='k', 
                    linewidth=0.2
                )

                if self.bPlotLogY.get():
                    y_min = max(his2.ymin, np.nextafter(0.0, 1.0))
                    y_max = his2.ymax
                    
                    if y_max > y_min:
                        # Powers of ten within the original Y range
                        y_ticks = 10.0 ** np.arange(
                            np.ceil(np.log10(y_min)),
                            np.floor(np.log10(y_max)) + 1
                        )
                        
                        self.theSubPlotArea.set_yticks(np.log10(y_ticks))
                        self.theSubPlotArea.set_yticklabels(
                            [f"{v:g}" for v in y_ticks]
                        )
                        self.theSubPlotArea.set_ylim(
                            np.log10(y_min), np.log10(y_max)
                        )

                # 6. Add the colorbar mapping to the 3D plot
                scalar_mappable = mpl.cm.ScalarMappable(norm=mynorm, cmap=mycmap)
                scalar_mappable.set_array(dz)
                self.theHistoFigure.colorbar(
                    scalar_mappable, 
                    ax=self.theSubPlotArea, 
                    orientation="vertical", 
                    shrink=0.5, 
                    aspect=10
                )
            
                # Labels and layout adjustments
                self.theSubPlotArea.set_zlabel("Counts")

            elif self.b2DPlotType.get() == "Contour" :
                ### adding the subplot
                self.theSubPlotArea = self.theHistoFigure.add_subplot(111)
                self.theSubPlotArea.set_xlabel(his2.name)
                
                #### Mask zero bins so that they appear white
                data_masked = np.ma.masked_equal(his2.dataNP, 0)
                hX = his2.Xbins()
                hY = his2.Ybins()
            
                ### Color palette
                mycmap = mpl.colormaps["rainbow"].copy()
                mycmap.set_bad("white")
                if self.bPlotLogX.get() :
                    self.theSubPlotArea.set_xscale('log')
                else : 
                    self.theSubPlotArea.set_xscale('linear')
                if self.bPlotLogY.get() :
                    self.theSubPlotArea.set_yscale('log')
                else : 
                    self.theSubPlotArea.set_yscale('linear')
                mynorm = None
                levels = np.linspace(data_masked.min(), data_masked.max(), 10)
                levels = np.array([float(f"{l:.3g}") for l in levels])
                if self.bPlotLogZ.get():
                    if data_masked.count() > 0:
                        vmin = data_masked.min() if data_masked.count() > 0 else 1.e-3
                        vmax = data_masked.max() if data_masked.count() > 0 else 0.
                    if vmax <= vmin:
                        vmax = vmin * 10
                    exp_min = np.floor(np.log10(vmin))
                    exp_max = np.ceil(np.log10(vmax))
                    nlev = int(exp_max - exp_min) 
                    nlev = min(10,int(np.floor(10/nlev))*nlev)+1
                    levels = np.logspace(exp_min, exp_max, nlev) 
                    levels = np.array([float(f"{l:.3g}") for l in levels])
                    mynorm = LogNorm(vmin=10**exp_min, vmax=10**exp_max)
                    # mynorm = LogNorm(vmin=vmin, vmax=vmax)
                    # levels = np.logspace(np.log10(vmin), np.log10(vmax), 10)

                contours = self.theSubPlotArea.contour(data_masked,extent=(his2.xmin,his2.xmax,his2.ymin,his2.ymax), levels=levels, cmap=mycmap, norm=mynorm, linewidths=0.5)   
                # 2. Use Matplotlib's built-in legend_elements() helper
                legend_handles, legend_labels = contours.legend_elements()
                
                # 3. Add the custom list as a legend on the side of the plot
                self.theSubPlotArea.legend(
                    handles=legend_handles, 
                    labels=legend_labels,
                    title="Contour Levels", 
                loc="center left", 
                bbox_to_anchor=(1.05, 0.5), # Places the list outside the right edge
                    frameon=True,
                    fontsize=8
                )
                            
            self.theHistoFigure.tight_layout()

            ### Embed the figure in the Tkinter frame
            self.theCanvas = FigureCanvasTkAgg(self.theHistoFigure, master = self.theFrameHF)
            self.theCanvas.draw()

            self.theCanvasCwid = self.theCanvas.get_tk_widget()
            self.theCanvasCwid.grid(column=0, row=1, sticky=tk.E+tk.W+tk.N+tk.S)
            self.theToolbarFrame = tk.Frame(master=self.theFrameHF)
            self.theToolbarFrame.grid(column=0, row=2, sticky=tk.EW)
            self.theToolbar = NavigationToolbar2Tk(self.theCanvas, self.theToolbarFrame)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def showStat(self,) :
        statLabel = tk.Label(self.theFrameCheckBoxHF,text="statistics")
        statLabel.grid(column=0, row=0, sticky=tk.E)
        
        fontList = tkFont.Font(family="Arial", size=11) #, weight="bold", slant="italic")
        self.StatText = tk.Text(self.theFrameCheckBoxHF,height=1,width=10,font=fontList)
        self.StatText.insert('1.0',"0")
        self.StatText.grid(column=1, row=0, sticky=tk.W)      

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def plotBestFormat(self,str,val) :
        if (abs(val) < 1000. and abs(val) > 0.001) or val == 0. :
            return str+"{:.3f}".format(val)  
        else:
            return str+"{:.3e}".format(val)  

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def printStatistics(self,histo) :
        optStat = self.StatText.get('1.0',tk.END).rstrip()
        if optStat == "" :
            return
        if int(optStat) == 0 :
            return
        bShowErr = 0
        iimax = len(optStat)
        ### CHECK
        for ii in range(iimax) :
            ista = int(optStat[ii])
            if ista >= 2 :
                bShowErr = 1
                
        for ii in range(iimax) :
            ista = int(optStat[ii])
            t2show = ""
            if ii == 0 :
                if ista >= 1 :
                    t2show += "Entri= "+str(int(histo.nent))
            elif ii == 1 :
                if ista >= 1 :
                    t2show += self.plotBestFormat("Mean=  ",histo.mean)
                if ista >= 2 :
                    t2show += self.plotBestFormat("+-",histo.meanErr)

            elif ii == 2 :
                if ista >= 1 :
                    t2show += self.plotBestFormat("RMS=   ",histo.RMS)
            elif ii == 3 :
                if ista >= 1 :
                    t2show += self.plotBestFormat("Over=  ",histo.over)
            elif ii == 4 :
                if ista >= 1 :
                    t2show += self.plotBestFormat("Under= ",histo.under)
                if ista >= 2 :
                    t2show += self.plotBestFormat("+-",histo.underErr)
            elif ii == 5 :
                dataSum = sum(histo.data)
                dataSumErr = sum([xx**2 for xx in histo.data])
                dataSumErr = sqrt(dataSumErr)
                if ista >= 1 :
                    t2show += self.plotBestFormat("Integr= ",dataSum)
                if ista >= 2 :
                    t2show += self.plotBestFormat("+-",dataSumErr)

            self.theHistoFigure.text(0.72-bShowErr*0.11,self.textYpos,t2show)
            if ista > 0 :
                self.textYpos -= 0.025

            if iimax == 0: return # do not loop 0 twice when range(0,0)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GaussianFit(self) :
        param,covar = self.lastHisto1.GaussianFit() 
        self.PlotGaussian(param,covar, self.lastHisto1, 1,11)# 10: print text but do not superimpose fit line
        if self.bPlotSame.get() :
            param,cov = self.prevHisto1N.GaussianFit()
            self.PlotGaussian(param,covar, self.prevHisto1N, 0,11)
        
    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def PlotGaussian( self, param, covariance, histo, idHis, iFitPlot ):
#        print("PLOTGAUSSIAN",param) #GDEB
        lcolor = 'black'
        if idHis == 0:
            lcolor = 'black'
        elif idHis == 1:
            lcolor = 'black'
        #        iFitPlot = 1
        if iFitPlot%10 == 1 :
            hxt = []
            hyFit = []
            hx = histo.Xbins()
            for xx in hx :
                hxt.append(xx)
                hyFit.append(param[0]*np.exp(-(xx-param[1])**2/(2*param[2]**2)))
#                print(xx,"GAUSSY",hyFit[len(hyFit)-1],(xx-param[1]),-(xx-param[1])**2/(2*param[2]**2))
            if idHis == 1:
                lcolor = 'grey'
            elif idHis == 0:
                lcolor = 'red'
#            self.theSubPlotArea.plot(hxt, hyFit, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
            self.theSubPlotArea.plot(hx, hyFit, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
#-            plt.draw()
        if int(iFitPlot/10) == 1 :
            stext=""
            if idHis == 0:
                lcolor = 'black'
                stext="Exp. sigma="
            elif idHis == 1:
                lcolor = 'red'
                stext="MC sigma="
            xPos = histo.xmin+(histo.xmax-histo.xmin)*0.6
            yPos = histo.maximumY()*(0.7-idHis*0.1)            
            mpl.rcParams.update({'font.size': 15})
        #    print("self.theSubPlotArea.text(",xPos,yPos,stext+" "+"{:.4g}".format(param[2]))
            self.theSubPlotArea.text(xPos,yPos,stext+" "+"{:.4g}".format(param[2]), color=lcolor)
            print("GAUSS RES",param[0],param[1],param[2]) 
            print("GAUSS COVARIANCE",covariance[0][0],covariance[0][1],covariance[0][2],covariance[1][0],covariance[1][1],covariance[1][2],covariance[2][0],covariance[2][1],covariance[2][2])
            self.theCanvas.draw()


    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def DoubleGaussianCenters0Fit(self) :
        param,cov = self.lastHisto1.DoubleGaussianCenters0Fit(1, 11) # 10: print text but do not superimpose fit line
        if self.bPlotSame.get() :
            param,cov = self.prevHisto1N.DoubleGaussianCenters0Fit(0, 11)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def PlotDoubleGaussian( self, param, covariance, histo, idHis, iFitPlot ):
        hyFit = []
        hx = histo.Xsteps()
        for hx1 in hx :
            hyFit.append(self.doubleGaussCenters0( hx1, param[0],param[1],param[2], param[3]))

        lcolor = 'black'
        if idHis == 0:
            lcolor = 'black'
        elif idHis == 1:
            lcolor = 'black'
        #        iFitPlot = 1
        if iFitPlot%10 == 1 :
            hxt = []
            hyt = []
            if idHis == 1:
                lcolor = 'green'
            elif idHis == 0:
                lcolor = 'red'
            self.theSubPlotArea.plot(hxt, hyt, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
            self.theSubPlotArea.plot(hx, hyFit, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
            chi2= 0.
            for ii in range(len(histo.data)) :                
                if hy[ii] >= 1. :
                    diff=hyFit[ii]-hy[ii]
                    chi2 += np.abs(diff)
            self.theCanvas.draw()
            
        #print("GaussianFit iFitPlot ",int(iFitPlot/10)) #GDEB
        if int(iFitPlot/10) == 1 :
            if idHis == 0:
                lcolor = 'black'
                stext="MC "
            elif idHis == 1:
                lcolor = 'red'
                stext="Exp. "
            # print("STEXT",idHis)#GDEB
            xPos = histo.xmin+(histo.xmax-histo.xmin)*0.6
            yPos = histo.maximumY()*(0.7-idHis*0.1)
            #print("SIGMA COLOR",lcolor,"xytext",xPos,yPos,) #GDEB
            mpl.rcParams.update({'font.size': 15})
            self.theSubPlotArea.text(xPos,yPos,stext+" "+"s1={:.4g}".format(param[1])+" s2={:.4g}".format(param[3]), color=lcolor)
            # print("GAUSS TEXT",xPos,yPos,"sigma1= "+"{:.4e}".format(param[1]),"sigma2= "+"{:.4e}".format(param[3]),"fraction_1= "+"{:.4e}".format(param[0]/(param[0]+param[2]))) #GDEB
            # print(self.name,"GAUSS RES",param[0],param[1],param[2],param[3]) #GDEB
            self.theCanvas.draw()

            #      print("doubleGaussCenters0 return", param)
        print(histo.name,"GAUSS RES","{:.4f}".format(param[0]),"{:.4f}".format(param[1]),"{:.4f}".format(param[2]),"{:.4f}".format(param[3])) #GDEB

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def TripleGaussianCenters0Fit(self) :
        #        print("start Gaussian Fit")
        param,cov = self.lastHisto1.TripleGaussianCenters0Fit(1, 11) # 10: print text but do not superimpose fit line
        if self.bPlotSame.get() :
            param,cov = self.prevHisto1N.TripleGaussianCenters0Fit(0, 11)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def PlotTripleGaussian( self, param, covariance, histo, idHis, iFitPlot ):
        hyFit = []
        for hx1 in hx :
            hyFit.append(self.tripleGaussCenters0( hx1, param[0],param[1],param[2],param[3],param[4], param[5]))

        lcolor = 'black'
        if idHis == 0:
            lcolor = 'black'
        elif idHis == 1:
            lcolor = 'black'
        if iFitPlot%10 == 1 :
            hxt = []
            hyt = []
            if idHis == 1:
                lcolor = 'green'
            elif idHis == 0:
                lcolor = 'red'
            self.theSubPlotArea.plot(hxt, hyt, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html
            self.theSubPlotArea.plot(hx, hyFit, label='fit', linestyle='dotted',color=lcolor) #(0, (1, 10)),color='black')  # https://matplotlib.org/stable/gallery/lines_bars_and_markers/linestyles.html

            chi2= 0.
            for ii in range(len(histo.data)) :                
                if hy[ii] >= 1. :
                    diff=hyFit[ii]-hy[ii]
                    chi2 += np.abs(diff)
            self.theCanvas.draw()
            
        if int(iFitPlot/10) == 1 :
            if idHis == 0:
                lcolor = 'black'
                stext="MC "
            elif idHis == 1:
                lcolor = 'red'
                stext="Exp. "
            xPos = histo.xmin+(histo.xmax-histo.xmin)*0.6
            yPos = histo.maximumY()*(0.7-idHis*0.1)
            mpl.rcParams.update({'font.size': 15})
            self.theSubPlotArea.text(xPos,yPos,stext+" "+"s1={:.4g}".format(param[1])+" s2={:.4g}".format(param[3])+" s3={:.4g}".format(param[5]), color=lcolor)
            # print("GAUSS TEXT",xPos,yPos,"sigma1= "+"{:.4e}".format(param[1]),"sigma2= "+"{:.4e}".format(param[3]),"fraction_1= "+"{:.4e}".format(param[0]/(param[0]+param[2]))) #GDEB
            # print(self.name,"GAUSS RES",param[0],param[1],param[2],param[3]) #GDEB
            self.theCanvas.draw()
            

#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
if __name__ == "__main__":
    ROOT = tk.Tk()
    ROOT.title("GAMOS Python Histogram Browser")
    ROOT.geometry("1200x850")
    ROOT.columnconfigure(0, weight=1)
    ROOT.rowconfigure(0, weight=1)
    APP = UI(ROOT,sys.argv)
    APP.mainloop()
    #ROOT.destroy()

