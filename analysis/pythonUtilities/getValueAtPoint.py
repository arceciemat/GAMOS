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
bY = False
print("NAR",len(sys.argv))
if len(sys.argv) == 4 :
    pointY = float(sys.argv[3])
    bY = True
if len(sys.argv) >= 5 :
    if sys.argv[3] == '-hn' :
        histoName = sys.argv[4]
if len(sys.argv) >= 6 :
    pointY = float(sys.argv[3])
    bY = True
    if sys.argv[4] == '-hn' :
        histoName = sys.argv[5]
    
for his1 in histoFile.fHistos1D:
    if histoName == "" or his1.name.find(histoName) != -1 :
        #print("#### getValueAtPoint",pointX,his1.xmin,his1.xmax)
        data,dataErr = his1.GetLinearInterpolatedValue(pointX)
        print(his1.name," VALUE_AT",pointX,"=",data,"+-",dataErr)


if bY :
    for his2 in histoFile.fHistos2D:
        if histoName == "" or his2.name.find(histoName) != -1 :
            #print("#### getValueAtPoint",pointX,his1.xmin,his1.xmax)
            data,dataErr = his2.GetLinearInterpolatedValue(pointX,pointY)
            print(his2.name," VALUE_AT",pointX,pointY,"=",data,"+-",dataErr)
