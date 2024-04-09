import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import sys
import numpy as np
from MyHistos import Histo1D, Histo2D
from csvHistoReader import csvHistoReader

###### -------------- MAIN ----------- ######
verbose = 1

if len(sys.argv) < 3 :
    print("!!! ERROR: at least two arguments are needed",
          "IN_FILE OUT_FILE fileName point (histoName)") 
    sys.exit()
    
fileName = sys.argv[1]
histoFile = csvHistoReader(fileName)

if verbose >= 3 : print("fInName=",fInName)
pointX = float(sys.argv[2])
histoName = ""
if len(sys.argv) == 4 :
    histoName = sys.argv[3]
    
for his1 in histoFile.fHistos1D:
    if histoName == "" or his1.name.find(histoName) != -1 :
        #print("#### getValueAtPoint",pointX,his1.xmin,his1.xmax)
        data,dataErr = his1.GetLinearInterpolatedValue(pointX)
        print(his1.name," VALUE_AT",pointX,"=",data,"+-",dataErr)
