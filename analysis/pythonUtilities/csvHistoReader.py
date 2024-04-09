import sys
import csv

from MyHistos import Histo1D, Histo2D
class csvHistoReader:
    fHistos1D = []
    fHistos2D = []
    def __init__(self, csvFileName):
        self.fHistos1D = []
        self.fHistos2D = []
        self.fileName = csvFileName
        try :
            with open(csvFileName, 'r') as csvFile:
                histoData = csv.reader(csvFile)
                #!! with this it doesn't work                print(csvFileName+" histoData ",len(list(histoData)))
                for row in histoData :
                    if len(row) == 0: continue
                    if row[0] == '1D' or row[0] == 'Prof1D' :
                        # print("csvHistoReader 1D HISTOGRAM",len(row))    
                        his1 = Histo1D(row)
                        self.fHistos1D.append(his1)
                        #                    print("csvHistoReader READ ",his1)
                    elif row[0] == '2D' or row[0] == 'Prof2D' :
                        #                    print("csvHistoReader 2D HISTOGRAM",len(row))
                        his2 = Histo2D(row)
                        self.fHistos2D.append(his2)
                        # print("csvHistoReader READ ",his2.name)
            #            print("csvHistoReader N READ ",len(self.fHistos1D)," fHistos1D's")
            #           print("csvHistoReader N READ ",len(self.fHistos2D)," fHistos2D's")
        
        except IOError :
             print("!!!ERROR FILE NOT FOUND",csvFileName)
             sys.exit()
             

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetHisto1(self, hisName ) :
        for his in self.fHistos1D :
            if his.name == hisName :
                return his
        return None

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetHisto2(self, hisName ) :
        for his in self.fHistos2D :
            if his.name == hisName :
                return his
        return None
