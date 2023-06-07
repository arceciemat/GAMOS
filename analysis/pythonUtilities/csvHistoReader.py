import sys
import csv

from MyHistos import Histo1D, Histo2D
class csvHistoReader:
    theFileName = ""
    fHistos1D = []
    fHistos2D = []
    def __init__(self, csvFileName):
        self.theFileName = csvFileName
        self.fHistos1D = []
        self.fHistos2D = []
        try :
            with open(csvFileName, 'r') as csvFile:
                histoData = csv.reader(csvFile)
                #            print(csvFileName+" histoData ")
                for row in histoData:
                    if len(row) == 0: continue
                    if row[0] == '1D' or row[0] == 'Prof1D' :
                        #                    print("csvHistoReader 1D HISTOGRAM",len(row))    
                        his1 = Histo1D(row)
                        self.fHistos1D.append(his1)
                        #                    print("csvHistoReader READ ",his1)
                    elif row[0] == '2D' or row[0] == 'Prof2D' :
                        #                    print("csvHistoReader 2D HISTOGRAM",len(row))
                        his2 = Histo2D(row)
                        self.fHistos2D.append(his2)
            #                   print("csvHistoReader READ ",his2)
            #            print("csvHistoReader N READ ",len(self.fHistos1D)," fHistos1D's")
            #           print("csvHistoReader N READ ",len(self.fHistos2D)," fHistos2D's")
        
        except IOError :
             print("!!!ERROR FILE NOT FOUND",csvFileName)
             sys.exit()
             
