import sys
import csv
import math

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
                    if row[0] in ('1D','Prof1D','2D','Prof2D') :
                        self.CheckFinite(row, histoData.line_num)
                    if row[0] == '1D' or row[0] == 'Prof1D' :
                        # print("csvHistoReader 1D HISTOGRAM",len(row))  #GDEB
                        his1 = Histo1D(row)
                        self.fHistos1D.append(his1)
                        #                    print("csvHistoReader READ ",his1)  #GDEB
                    elif row[0] == '2D' or row[0] == 'Prof2D' :
                        #                    print("csvHistoReader 2D HISTOGRAM",len(row))  #GDEB
                        his2 = Histo2D(row)
                        self.fHistos2D.append(his2)
                        # print("csvHistoReader READ ",his2.name)  #GDEB
                        # print("csvHistoReader N READ ",len(self.fHistos1D)," fHistos1D's")  #GDEB
                        # print("csvHistoReader N READ ",len(self.fHistos2D)," fHistos2D's")  #GDEB
        
        except IOError :
             print("csvHistoReader.py !!!ERROR FILE NOT FOUND",csvFileName)
             sys.exit()
             

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def CheckFinite(self, row, lineNum ) :
        # GAMOS .csv histograms are always written with errors (GmHistoWriterCSV): check the number of fields
        try :
            if row[0] in ('1D','Prof1D') :
                nExpected = 2*int(float(row[2]))+14
            else :
                nExpected = 2*(int(float(row[2]))+2)*(int(float(row[5]))+2)+17
        except (ValueError, IndexError) :
            raise ValueError("csvHistoReader.py: file "+self.fileName+" line "+str(lineNum)+": wrong histogram header "+str(row[:8]))
        if len(row) != nExpected :
            raise ValueError("csvHistoReader.py: file "+self.fileName+" line "+str(lineNum)+" histogram '"+row[1]+"': it has "+str(len(row))+" fields, while it should have "+str(nExpected)+" (GAMOS .csv histograms are always written with errors)")
        # all fields after the type and the name are numbers: nan or inf are an error
        for ii in range(2,len(row)) :
            try :
                val = float(row[ii])
            except ValueError :
                raise ValueError("csvHistoReader.py: file "+self.fileName+" line "+str(lineNum)+" histogram '"+row[1]+"': field "+str(ii+1)+" is not a number: '"+row[ii]+"'")
            if not math.isfinite(val) :
                raise ValueError("csvHistoReader.py: file "+self.fileName+" line "+str(lineNum)+" histogram '"+row[1]+"': field "+str(ii+1)+" is "+row[ii]+" (nan and inf are not valid histogram values)")

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetHisto1(self, hisName, bExists = True ) :
        for his in self.fHistos1D :
            if his.name == hisName :
                return his
        if bExists :
            print("csvHistoReader.py !!! FATAL_ERROR at GetHisto1 HISTO DOES NOT EXIST:",hisName)
        return None

    #....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.
    def GetHisto2(self, hisName ) :
        for his in self.fHistos2D :
            if his.name == hisName :
                return his
        return None
