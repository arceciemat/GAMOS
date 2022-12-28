import numpy as np
from MyHistos import Histo1D, Histo2D

npmat = np.ones([4,5])
npmat[1,3] = 2.
nperr = np.ones([4,5])
args = []
his2 = Histo2D(args)
print("npmat",npmat.shape)
his2.FillFromNPMats("testnph",0.,100.,-20,20.,npmat,nperr)

fOut = open("testnph.csv",'w')
his2.Write(fOut)

