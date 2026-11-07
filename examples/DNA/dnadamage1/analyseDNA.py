import sys
import csv
from DNAmolecule import Molecule, readMolecule
from DNAVolumeType import *
from MyHistos import Histo1D, Histo2D
import numpy as np
import collections
import random

verbose = 2

bCheckTwoMolecules1Reaction = False

######### main #############
if len(sys.argv) < 3 :
    print("!!! FATAL ERROR: MUST HAVE TWO ARGUMENTS <PhysicalInteractions_File> <DamagingChemicalReactions_File> <fab2g4dna_File>")
    sys.exit()

theMinEnergyInteraction = 17.5
theProbDamage = 42 # % probability chemical reaction couses damage
physFileName = sys.argv[1]
chemFileName = sys.argv[2]
fab2g4dnaFileName = "VoxelStraight.fab2g4dna"
if len(sys.argv) >= 4 :
    fab2g4dnaFileName = sys.argv[3]
thePhysFileData = []
theChemFileData = []
eventList = []

physTable = collections.defaultdict(dict)  # map of EventIDp strand copyN: sum of EnergyDeposit's
resultPhysTable = collections.defaultdict(dict) # map of EventIDp,strand: list of copyN interactions (if E > theMinEnergyInteraction in same EventIDp,strand,copyN])
bAllowCopyNRepet = True # if False CopyN cannot be repeated in whole file, if True it can be in file, but it cannot be repeated in same event
CopyNumTable = []  # if not bAllowCopyNRepet: list of [voxelCopyN,strand] with interaction (do not look at EventIDp) (if E > theMinEnergyInteraction in same EventIDp,strand,copyN])
resultChemTable = collections.defaultdict(dict) # mapst of EventIDc,strand : list of molecule ID's 

#### Physics analysis: read data file and produce resultPhysTable
if verbose >= 1 : print("#@#@ START Reading phys file:",physFileName)
with open(physFileName, 'r') as csvFile:
    thePhysFileData = csv.reader(csvFile, delimiter=',')
#thePhysFileData = pandas.read_csv(physFileName)
    if verbose >= 3 : print("N thePhysFileData",(thePhysFileData))

    theMolecules = readMolecule(fab2g4dnaFileName) # read all the molecules of the geometry
    if verbose >= 1 : print("N MOLECULES",len(theMolecules)," in ",fab2g4dnaFileName)
## Check if bases are always in the same strand
    bCheckBaseStrand = True
    theBaseStrands = {}
    for mole in theMolecules :
        if not isBaseByName(mole.name) :
            continue
        if not mole.name in theBaseStrands :
            theBaseStrands[mole.name] = mole.strand
            theBaseStrands[mole.name+"_water"] = mole.strand
        if theBaseStrands[mole.name] != mole.strand :
            if verbose >= 2 : print("!! WARNING: base is in two strands ",mole.name)
            bCheckBaseStrand = False
            break        
    
    #  InitialPosX(nm) InitialPosY(nm) InitialPosZ(nm) AccumulatedEnergyDeposited KineticEnergyChange DNAInitialVolumeType InitialPVCopyNumber EventID TrackID ParentTrackID StepNumber Particle FinalProcess  
    addCopyN = True
    il = 1
    ### Process thePhysFileData and produce resultPhysTable (and CopyNumTable)
    for rowp in thePhysFileData:
        if verbose >= 4 : print(len(rowp),"READ row")
        if len(rowp) != 13:
            if verbose >= 2 : print("!!! WARNING Wrong PhysicalInteraction line No:",il,"\n",rowp)
            continue
        il += 1
        EnergyDeposit = float(rowp[3])
        volumeType = int(rowp[5])
        copyN = int(rowp[6])
        EventIDp = int(rowp[7])
        if not EventIDp in eventList :
            eventList.append(EventIDp)
        
        if verbose >= 3 : print("row READ",EnergyDeposit,printDNAVolumeType(volumeType),copyN,EventIDp)
        ### Avoid histone
        if volumeType == DNAVolumeType.histone :
            if verbose >= 2 : print("!! WARNING: continue volumeType histone",printDNAVolumeType(volumeType),"line No:",il,"\n",rowp)
            continue
        ### Determine the strand
        strand = -1
        isLS = isLeftStrand(volumeType)
        isRS = isRightStrand(volumeType)
        
        if verbose >= 3 : print("isLeft/RightStrand",isLS,isRS)
#        print(list(DNAVolumeType))
        if( isLS ) :
            strand = 0
        elif( isRS ) :
            strand = 1
        else :
            bMoleNoStrand = True
            if bCheckBaseStrand :
                if isBase(volumeType) :
                    strand = theBaseStrands[DNAVolumeType(volumeType).name]-1
                    bMoleNoStrand = False
            if bMoleNoStrand :
                #in case molecules are not assigned "strand"
                if verbose >= 3 : print("!! WARNING: molecule with no assigned strand",printDNAVolumeType(volumeType),"line No:",il,"\n",rowp)
                continue

        ### Determine the name
        isDeox = isDeoxyribose(volumeType)
        isPhos = isPhosphate(volumeType)
        isB = isBase(volumeType)
        if verbose >= 3 : print("isDeoxyribose/isPhosphate/isBase",isDeox,isPhos,isB)
        
        if(isDeox or isPhos or isB) :
            if verbose >= 4 : print("physTable",EventIDp,strand,copyN,"+=",EnergyDeposit)
            if not (EventIDp,strand,copyN) in physTable.keys() :
                physTable[EventIDp,strand,copyN] = 0.
                if verbose >= 3 : print("NEW physTable",EventIDp,":",strand,":",copyN,"=",physTable[EventIDp,strand,copyN])
            physTable[EventIDp,strand,copyN] = physTable[EventIDp,strand,copyN] + EnergyDeposit
            if verbose >= 3 : print(EnergyDeposit,"ADD physTable",EventIDp,":",strand,":",copyN,"=",physTable[EventIDp,strand,copyN])
        else :
            if verbose >= 2 : print("WARNING: NOT isDeoxyribose or isPhosphate or isBase:", printDNAVolumeType(volumeType),"line No:",il,"\n",rowp)
            
        if verbose >= 5 : print("physTable",physTable) 
        if physTable[EventIDp,strand,copyN] >= theMinEnergyInteraction : 
            addCopyN = True
            if not bAllowCopyNRepet : 
                if (copyN,strand) in CopyNumTable :
                    addCopyN = False
                    if verbose >= 3 : print(il,"phys reaction makes damage REPEATED copyNo & strand",copyN,strand)
            else :
                if (EventIDp,strand) in resultPhysTable.keys() :
                    copyList = resultPhysTable[EventIDp,strand]
                    # print(EventIDp,strand,"CHECK ",copyN," in copyList ",copyList) #GDEB
                    if copyN in copyList :
                        addCopyN = False
                        if verbose >= 2 : print(il,"phys reaction makes damage REPEATED copyNo & EventID & strand",copyN,EventIDp,strand) 

            if addCopyN :
                if not bAllowCopyNRepet :
                    CopyNumTable.append((copyN,strand))
                    if verbose >= 2 : print(il,"ADD CopyNumTable",copyN,":",strand)
                    
                if not (EventIDp,strand) in resultPhysTable.keys() :
                    rpt = []
                    resultPhysTable[EventIDp,strand] = rpt 
                resultPhysTable[EventIDp,strand].append(copyN)
                if verbose >= 2 : print(il,"ADD copyN resultPhysTable",copyN,EventIDp,":",strand,"=", resultPhysTable[EventIDp,strand])
        else :
            if verbose >= 3 : print("physTable TOO LOW ENERGY still",physTable[EventIDp,strand,copyN],"<= ",theMinEnergyInteraction," eV") 

if verbose >= 1 : print("SUMMARY physTable",len(physTable)," resultPhysTable",len(resultPhysTable)," CopyNumTable",len(CopyNumTable),"bAllowCopyNRepet",bAllowCopyNRepet)


#### Chemistry analysis: read data file and produce resultPhysTable
if verbose >= 1 : print("#@#@ START Reading chem file:",chemFileName)
with open(chemFileName, 'r') as csvTimeFile:
    theChemFileData = csv.reader(csvTimeFile)
    if verbose >= 9 : print("N theChemFileData",(theChemFileData))

    il = 1
    for rowc in theChemFileData:
        if len(rowc) != 5 and len(rowc) != 6  :
            if verbose >= 2 : print("!!! WARNING Wrong DamagingChemicalReaction line No:",il,"\n",rowc)
            continue
        il += 1
        if verbose >= 4 : print("CHEM row",rowc)
        elemPos = np.array([float(rowc[0]),float(rowc[1]),float(rowc[2])])
        EventIDc = int(rowc[4])
        if not EventIDc in eventList :
            eventList.append(EventIDc)
        #print(EventIDc,"CHEM pos",elemPos) #GDEB

### Look is there is any Molecule close to chemical reaction
        bMoleFound = False
        for mole in theMolecules :
            MIN_DIST= 1e-5
            # print("LOOK FOR MOLECULE close to CHEM",np.linalg.norm(mole.pos - elemPos )/np.linalg.norm(mole.pos+elemPos),(mole.pos - elemPos ),"(",mole.pos," - ",elemPos) #GDEB
            if np.linalg.norm(mole.pos-elemPos)/np.linalg.norm(mole.pos+elemPos) <= MIN_DIST :
                #t  if(mole.pos == elemPos ) :
                if verbose >= 2 : print(il,"FOUND MOLECULE pos",np.linalg.norm(mole.pos-elemPos)/np.linalg.norm(mole.pos+elemPos),mole.pos,elemPos,mole.name)
                if( bMoleFound ) :
                    if verbose >= 1 : print(il,"TWO MOLECULES FOUND for 1 DamagingChemicalReaction. pos=",mole.pos)
                bMoleFound = True
                strand = mole.strand-1
                if strand == -1 :
                    if verbose >= 2 : print("WARNING: Unknown DNA component",mole.name," line No:",il,"\n",rowc)
                    continue
                #    sys.exit()
                
                rdn = random.randrange(0,100)
                # print("rdn",rdn) #GDEB
                if rdn > theProbDamage : # //42% of reaction makes damage
                    continue
                if verbose >= 3 : print("reaction makes damage. copyNo",mole.copyNo)
                addCopyN = True
                if not bAllowCopyNRepet : 
                    if (mole.copyNo,strand) in CopyNumTable :
                        addCopyN = False
                        if verbose >= 2 : print("chem reaction makes damage REPEATED copyNo and strand",mole.copyNo,strand )
                else :
                    if (EventIDc,strand) in resultChemTable.keys() :
                        copyList = resultChemTable[EventIDc,strand]
                        # print(EventIDc,strand,"CHECK ",mole.copyNo," in copyList ",copyList) #GDEB
                        if mole.copyNo in copyList :
                            addCopyN = False
                            if verbose >= 2 : print("chem reaction makes damage REPEATED copyNo & EventID & strand",mole.copyNo,EventIDc,strand)
                
                if addCopyN :
                    if not bAllowCopyNRepet :
                        CopyNumTable.append((copyN,strand))
                        if verbose >= 2 : print("ADD CopyNumTable",copyN,":",strand)

                    if not (EventIDc,strand) in resultChemTable.keys() :
                        rpt = []
                        resultChemTable[EventIDc,strand] = rpt
                    resultChemTable[(EventIDc,strand)].append(mole.copyNo)
                    if verbose >= 2 : print(il,"ADD copyNo resultChemTable",mole.copyNo,EventIDc,":",strand,"=",resultChemTable[(EventIDc,strand)])
                if not bCheckTwoMolecules1Reaction :
                    break

if verbose >= 1 : print("SUMMARY resultChemTable",len(resultChemTable)," CopyNumTable",len(CopyNumTable),"bAllowCopyNRepet",bAllowCopyNRepet)

### Write file of strand breaks
ofile = open("SDDFormat.txt",'w');
#if(ofile.is_open())
ofile.write('\n')
ofile.write("SDD version, SDDv1.0;\n")
ofile.write("Software, chromatin fiber Model;\n")
ofile.write("Author contact, Carmen Villagrasa,carmen.villagrasa@irsn.fr, 02/04/2018, Melyn et al.,Sc. Rep. 7 (2017)11923;\n")
ofile.write("Simulation Details, DNA damages from direct and indirect effects;\n")
ofile.write("Source, Monoenergetic cylinder-parallel proton beam uniformly in 5 nm radius from left side of the cube")
ofile.write(" exposing chromatin fiber. Energy: 5 keV;\n")
ofile.write("Source type, 1;\n")
ofile.write("Incident particles, "+str(len(eventList))+";\n")
ofile.write("Mean particle energy, 00 keV;\n")
ofile.write("Energy distribution, M, 0;\n")
ofile.write("Particle fraction, 1.0;\n")
ofile.write("Dose or fluence, 0, 0;\n")
ofile.write("Dose rate, 0.0;\n")
ofile.write("Irradiation target, Simple spherical chromatin fiber model in a voxel 40 nm;\n")
ofile.write("Volumes, 0,20,20,20,-20,-20,-20,2,15,15,20,-15,-15,20;\n")
ofile.write("Chromosome sizes, ;\n")
ofile.write("DNA Density, Cell Cycle Phase, G0/G1;\n")
ofile.write("DNA Structure, 4, 1;\n")
ofile.write("In vitro / in vivo, 0;\n")
ofile.write("Proliferation status, 1;\n")
ofile.write("Microenvironment, 27, 0.0;\n")
ofile.write("Damage definition, 1, 1, 10, -1, 17.5;\n")
ofile.write("Time, 2.5ns;\n")
ofile.write("Damage and primary count, 638, 200;\n")
ofile.write("Data entries, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0;\n")
ofile.write("Data was generated in minimal output format and used as an example. Please modify the primary particle number for this file;\n")
ofile.write("\n")
ofile.write("***EndOfHeader***;\n")
ofile.write("\n")
#For detail, please refer to Schuemann, J., et al. (2019). A New Standard DNA 
#Damage (SDD) Data Format. Radiation Research, 191(1), 76. 

bPrimaryflag = True;
positionAndCopyN = "; 0, 0, 0; 3, 0, 0, "
SSBType = " 0, 1, 0"
for key,vals in resultPhysTable.items() :
    print("PHYS KEY",key,len(vals))
    for resp in vals :
        if(bPrimaryflag) :
            lineo="2"
            bPrimaryflag = False
        else :
            lineo="0"
        lineo +=", "+str(key[0])+"a,"+str(key[1])+"b"+str(positionAndCopyN)+str(resp)+"c; 0;"+str(SSBType)+"\n"
        ofile.write(lineo)

bPrimaryflag = True;
ofile.write("\n")
for key,vals in resultChemTable.items() :
    print("CHEM KEY",key,len(vals))
    for resc in vals :
        if(bPrimaryflag) :
            lineo="2"
            bPrimaryflag = False
        else :
            lineo="0"
        lineo +=", "+str(key[0])+","+str(key[1])+str(positionAndCopyN)+str(resc)+"; 0;"+str(SSBType)+"\n"
        ofile.write(lineo)
            
ofile.close();

