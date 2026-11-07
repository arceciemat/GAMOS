import numpy as np
import math
import sys
import os

class Molecule :
 
    name = ""
    copyNo = 0
    pos = np.array([0.,0.,0.])
    strand = 0
    radius = 0.
    radiusWater = 0.

    def __init__(self,name, copyNo, posX, posY, posZ, strand) :
        self.name = name
        self.copyNo = copyNo
        self.pos = np.array([posX,posY,posZ])
        self.strand = strand
     
def readMolecule(fab2g4dnaFileName):

    theMolecules = []
    size = 0.;
    name = "";
    if os.path.isfile(fab2g4dnaFileName) :
        fabFile = open(fab2g4dnaFileName);
    else :
        print("!!!! FATAL ERROR FILE DOES NOT EXIST",fab2g4dnaFileName)
        sys.exit()

    copyNoPlus = 0
    nLinkers = 0
    for line in fabFile:
        words = line.rstrip().split()
        if len(words) == 0 :
            continue        
        if words[0] == "_Size" :
            size = int(words[1])
        elif words[0] == "_Number" and words[1] == "voxelLinker" :
            nLinkers = int(words[2])
        elif words[1] == "Start" and  words[2] == "linker" :
            copyNoPlus = 1000
        elif words[1] == "End" and  words[2] == "linker" :
            copyNoPlus = 1000*nLinkers
        elif words[1] == "Linker" :
            copyNoPlus = 1000 + 1000*(int(words[2])+1)
        elif words[1] == "Nucleosome" :
            copyNoPlus = 100000*(int(words[2])+1)
        elif words[0] == "_pl" :
            name = words[1]
            material = words[2]
            strand = int(words[3])
#             copyNo = int(words[4])+copyNoPlus
            copyNo = int(words[4])
            posX = float(words[5]) # in nm
            posY = float(words[6])
            posZ = float(words[7])

            molecule = Molecule(name, copyNo, posX, posY, posZ, strand)
          #  print("MOLECULE FOUND",words)
            theMolecules.append(molecule);

    fabFile.close();
    
    return theMolecules;

