import tkinter as tk
from tkinter import ttk
from csvHistoReader import *
import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg,NavigationToolbar2TkAgg
                                             
class UI(tk.Frame):
    """Docstring."""
    verbose = 1
    onevar = 0
    fHistos1D = []
    fHistoFiles = []
    fHisto2Histofile = {}
    fFileListLinePos = {}
    bHistoShown = {}
    theSubPlotArea = None
    theCanvas = None
    resiz = 0
    histShown = False
    thePanedWindow = None
    theFrameHL = None
    theFrameHF = None
    figure = None
    #    photo = PhotoImage(file="cat.png")
#    def __init__(self, parent=None, *args):
    def __init__(self, parent, *args):
        # Bind the function to configure the parent window
        parent.bind("<Configure>", self.ResizeListBoxAndImage)

        #---- Define frames and pannedWindow
        tk.Frame.__init__(self, parent,bg='grey', width=437, height=798, borderwidth=10, relief='sunken')#, pady=3)
        self.parent = parent #??
        self.pack(fill="both",expand=True)
        self.columnconfigure(0, weight = 1)
        self.rowconfigure(0, weight = 1)        
        
        self.thePanedWindow = ttk.PanedWindow(self,orient="horizontal")
        self.thePanedWindow.pack(fill="both",expand=True) # Window,height=798,width=437, background="red")
        self.theFrameHL = tk.Frame(self.thePanedWindow,height=798,width=437, background="blue")
        self.thePanedWindow.add(self.theFrameHL)
        self.theFrameHL.columnconfigure(0, weight=1)
        self.theFrameHL.rowconfigure(1, weight=1)

        self.theFrameHF = tk.Frame(self.thePanedWindow,height=700,width=700, background="red")
        self.thePanedWindow.add(self.theFrameHF)
        self.theFrameHF.columnconfigure(0, weight=1)
        self.theFrameHF.rowconfigure(1, weight=1)

        self.ReadHistoFiles( args )

        self.CreateWidgets()

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def ReadHistoFiles( self, args ): 

        if self.verbose >= 3 : print("N ARGS ",len(args),"  ",args)
        iihf = 1
        iih = 0
        for hflist in args:
            if self.verbose >= 3 : print("ARG= ",len(hflist),"  ",hflist)
            for hfName in hflist:
                if hfName == sys.argv[0]: continue
                if self.verbose >= 2 : print("READING HISTO FILE: ",hfName)
                self.fHistoFiles.append(hfName)
                self.bHistoShown[iihf] = True
                histoFile = csvHistoReader(hfName)
                #                self.fHistos1D.extend(histoFile.fHistos1D)
                for his1 in histoFile.fHistos1D :
                    self.fHistos1D.append(his1)
                    self.fHisto2Histofile[iih] = iihf
                    if self.verbose >= 2 : print(iih," READ fHistos1D: ",his1.name," fHisto2Histofile ",self.fHisto2Histofile[iih])
                    iih += 1
                iihf += 1

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def CreateWidgets(self):
#        self.oneChkBut = tk.Checkbutton(self, text="One", variable=self.onevar, onvalue=True)
#        self.oneChkBut.grid(column=1, row=0, columnspan=3, rowspan=2)
#        self.oneChkBut.grid(column=0, row=0)
        ##### LIST OF HISTOGRAMS
        #        tk.Label(self.theFrameHL, text='Find what:').grid(column=2, row=2, sticky=tk.W)
        lbSBV = tk.Scrollbar(self.theFrameHL,orient=tk.VERTICAL)#,command = self.theFrameHL.histList.yview)
        lbSBV.grid(column=1, row=0, sticky=tk.N+tk.S)
        lbSBH = tk.Scrollbar(self.theFrameHL,orient=tk.HORIZONTAL)#,command = self.theFrameHL.histList.yview)
        lbSBH.grid(column=0, row=1, sticky=tk.W+tk.E)
        hlWidthMax = 50
        hlWidth = 0
        for his in self.fHistos1D :
            hl1 = len(his.name)
            hlWidth = max(hlWidth,hl1)
            #            print("hlWidth ",hlWidth,"  ",hl1)
            if hlWidth > hlWidthMax :
                hlWidth = hlWidthMax
                break
        hlHeightMax = 30
        hlHeight = min(len(self.fHistos1D)+len(self.fHistoFiles),hlHeightMax)
        hlHeight = 40
        
        self.histList = tk.Listbox(self.theFrameHL,height=hlHeight,width=hlWidth
                                   ,yscrollcommand=lbSBV.set,xscrollcommand=lbSBH.set)# selectmode='extended'),)
        ### INSERT HISTOGRAMS IN LIST
        self.FillListBox()
#        self.histList.config(yscrollcommand=lbSB)
        lbSBV['command'] = self.histList.yview
        lbSBH['command'] = self.histList.xview
        
        #### QUIT button
        self.quitBut = tk.Button(self.theFrameHL, text="Quit", command=self.quit, font= ('Helvetica 14 bold italic'))
        self.quitBut.grid(column=0, row=2)

#        if self.verbose >= -3 : print("FRAME SIZE",self.parent.winfo_height(), self.parent.winfo_width())
#        if self.verbose >= -3 : print("histList SIZE", self.histList.winfo_height(),  self.histList.winfo_width())
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
            self.fFileListLinePos[iil] = -iihf
            self.histList.insert(iil,hisFile1+str(iihf))
            iil += 1
            iih = 0
            for his1 in self.fHistos1D:
                if self.bHistoShown.get(iihf) :     
                #            print("HISTO1: ",his1)
                    if self.fHisto2Histofile.get(iih) == iihf:
                        if self.verbose >= 2 : print(iil, " INSERT HISTO: ",his1.name," === ",self.fHisto2Histofile.get(iih)," == ",iih)
                        self.histList.insert(iil,"  "+his1.name+str(iih))
                        self.fFileListLinePos[iil] = iih
                        if self.verbose >= 3 : print(" SET self.fFileListLinePos ",iih, " = ",self.fFileListLinePos[iil])
                        iil+=1
                iih = iih+1      # index of histogram in fHistos1D
            iihf += 1
        self.histList.grid(column=0, row=0, sticky=tk.E+tk.W+tk.N+tk.S)#, sticky=tk.W)
        self.histList.bind('<<ListboxSelect>>', self.SelectHist)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def SelectHist(self,event):
        selected_index = int(event.widget.curselection()[0])
        iil = self.fFileListLinePos.get(selected_index)
        if self.verbose >= 3 : print("@@ selectHist ",selected_index," : ",iil)
        print(" READ self.fFileListLinePos ",selected_index, " = ",self.fFileListLinePos.get(selected_index))
        if iil < 0 : # show / unshow histograms in file
            self.bHistoShown[abs(iil)] = not self.bHistoShown.get(abs(iil))
            self.FillListBox()
                
        else: # select histogram
            #        selected_value = event.widget.get(selected_index)
            if self.verbose >= 1 : print("@@@@ SELECTING ",self.fHistos1D[iil].name)
            self.showHist(iil)

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def showHist(self,ih):

        self.histShown = True
        self.figure = Figure(figsize = (7, 7), dpi = 100)
        # adding the subplot
        self.theSubPlotArea = self.figure.add_subplot(111)
        his1 = self.fHistos1D[ih]
        hX = his1.Xbins(his1)
        hY = his1.data
        if self.verbose >= 3 : print(" PLOT HIS1 ",his1,":",his1.data,"+-",his1.dataErr," Xs=",hX)
        xStep = his1.Xstep(his1)
 #       plot1.xlim(float(his1.xmin),float(his1.xmax))
 #       plot1.xlabel(his1.name)
        self.theSubPlotArea.bar(hX,hY, color='None', edgecolor='black', yerr=his1.dataErr, width=xStep)
        self.theCanvas = FigureCanvasTkAgg(self.figure, master = self.theFrameHF)
        self.theCanvas.show()
        #       theCanvas.get_tk_widget().pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        self.theCanvas.get_tk_widget().grid(column=0, row=0, sticky=tk.E+tk.W+tk.N+tk.S)
        toolbarFrame = tk.Frame(master=self.theFrameHF)
        toolbarFrame.grid(column=0,row=2, sticky=tk.E+tk.W+tk.N+tk.S)
        toolbar = NavigationToolbar2TkAgg(self.theCanvas, toolbarFrame)
        #        if self.verbose >= -3 : print("theSubPlotArea SIZE", self.theSubPlotArea.winfo_height(),  self.theSubPlotArea.winfo_width())
        #if self.verbose >= -3 : print("theCanvas SIZE", self.theCanvas.height(),  self.theCanvas.width())
#        ROOT.geometry("1137x798")

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    def ResizeListBoxAndImage(self,event):
        #        if self.theCanvas != None :             return
#        self.grid_remove()
#        self.grid(sticky=tk.N+tk.S+tk.W+tk.E) # histList SIZE 404 724    364  618 
#        if self.verbose >= -3 : print("FRAME SIZE",self.theFrameHL.winfo_width(), self.theFrameHL.winfo_height())
        if self.parent.winfo_width() == 404 :
            return
        if self.parent.winfo_height() == 724 :
            return
        if self.theFrameHL.winfo_height() != 778 :
            if self.histShown == False : 
                width = int((self.parent.winfo_width()-33)/8)
            else : 
                width = int((self.parent.winfo_width()-733)/8)                
            self.histList.config(height = int((self.parent.winfo_height()-74)/18) )
                #            self.histList.config(height = 30)
#            self.histList.grid_remove()
#            self.histList.grid(column=0, row=0, sticky=tk.E+tk.W+tk.N+tk.S)#, sticky=tk.W)
            if self.verbose >= -3 : print("NEW histList SIZE", self.histList.winfo_width(),self.histList.winfo_height(),self.theFrameHL.winfo_height())        
#....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
if __name__ == "__main__":
    ROOT = tk.Tk()
    ROOT.title("GAMOS Python Histogram Browser")
    ROOT.geometry("")  #437 798")
    ROOT.columnconfigure(0, weight=1)
    ROOT.rowconfigure(0, weight=1)
    APP = UI(ROOT,sys.argv)
#    APP = UI(sys.argv[1])
    APP.mainloop()
    ROOT.destroy()
