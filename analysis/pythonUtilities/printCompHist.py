import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import matplotlib, sys
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
from checkPythonVersion import *
from MyHistos_nch import Histo1D
from csvHistoReader_nch import csvHistoReader

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()

listofDict=[]
# histype= sys.argv[1] #'Profile_X_at_Braggpeak'
# path='/home/natalia/gamos/GAMOS.6.3.0/Scripts/RADPROTIM_20ene/'
# files=sys.argv[2:]#[path+'analyseSqdose_xLETDose_80.2_1001_1000000_32.csv',path+'analyseSqdose_xLETDose_90.1_1001_1000000_32.csv']
#añadir un modificador para que lo entienda como tipo de variable
if ('-t' in sys.argv) and ('-f' in sys.argv):
    idxType = sys.argv.index('-t')
    idxFiles = sys.argv.index('-f')
else:
    print('Error: provide the histogram type after -t and the files after -f.')
    exit()
if idxType > idxFiles: print('Error: -f has to be written only after -t.'); exit()
histypes = sys.argv[idxType+1:idxFiles]
files = sys.argv[idxFiles+1:]
# print('histypes: ',histypes,idxType,'files: ',files,idxFiles,'option',option)
#Store info in a list of dictionaries:
xvalues=[]
yvalues=[]
count=0
for file in files:
    oneDict={}
    oneDict.update({'name':file})
    histoFile = csvHistoReader(file)
    #print("NHISTOS 1D=",len(histoFile.fHistos1D))
    for his1 in histoFile.fHistos1D:
        hX = his1.Xbins()
        his1.norm(1)
        hY = his1.data
        xStep = his1.Xstep()
        oneDict.update({his1.name : [hX,hY]})
    listofDict.append(oneDict) 
              
        
    #Check error: ¿puedo hacer el plot de dos gráficas con distinto numero de bines? me interesa? si quisiera comparar un PDD que llega a 12 y otro que llega a 14, xej.
    # if len(listofDict[0]['Profile_X_at_Braggpeak'][0]) ~= len(listofDict[1]['Profile_X_at_Braggpeak'][0])
        #print('Error: curves need to have the same size.')
        #break
    #plot diff energies together
for types in histypes:
    fig =plt.figure()
    for histodict in listofDict:
        if types in histodict:
            print('in ',histodict['name'])
            xvalues.append(histodict[types][0][histodict[types][1].index(np.max(histodict[types][1]))] ) #posición x del máximo en y, en el diccionario.
            yvalues.append(np.max(histodict[types][1]))

            plt.plot(histodict[types][0],histodict[types][1],label=histodict['name']+' max is:'+str(np.max(histodict[types][1])))
            plt.plot(xvalues[count],yvalues[count], marker="o")
            plt.axvline(x=2.15)
            plt.title('Energy comparison for '+types)
            plt.ylabel('nbins')
            plt.legend(loc='upper right')
            count+=1

    plt.grid()
    plt.show()
    plt.savefig("histcomparison_"+types+".jpg")    
    plt.savefig("hist1.jpg")    
    print('xvalues:',xvalues,' yvalues:',yvalues)     
sys.exit()
    

