import sys
#https://www.tutorialspoint.com/python/python_gui_programming.htm
import numpy as np
from scipy.stats import norm
from checkPythonVersion import *

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()
verbose = 0

### Manage arguments 
fileName=""
icolumn = 0
fileName = sys.argv[1]
    
### Read file
values = np.loadtxt(fileName, usecols=icolumn, comments=':DATA')
nVals = values.size

mean = values.mean()
std = values.std(ddof=1) if nVals > 1 else 0.   # sample standard deviation
sem = std / np.sqrt(nVals)                 # standard error of the mean
rms = np.sqrt(np.mean(values**2))           # root mean square about zero

print("MEAN=","{:.3f}".format(mean), "STD=", "{:.3f}".format(std), "RMS=", "{:.3f}".format(rms), "SEM=", "{:.3f}".format(sem), "N=", nVals)

