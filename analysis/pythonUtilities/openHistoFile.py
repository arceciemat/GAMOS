import sys

#https://www.tutorialspoint.com/python/python_gui_programming.htm
import random
import matplotlib, sys
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import numpy as np
import pandas as pd

from checkPythonVersion import *
from MyHistos import Histo1D
from csvHistoReader import csvHistoReader
from histBrowser import *

###### -------------- main ----------- ######
#histoData = pd.read_csv(sys.argv[1])
checkPythonVersion()

histoFile = csvHistoReader(sys.argv[1])

app = UI()
#app.master.title = "First Tkinter"
app.title = "First Tkinter"
app.mainloop()

print("check ",app.onevar)

sys.exit()
    
