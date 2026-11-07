import sys
import csv

verbose = 2

######### main #############
if len(sys.argv) != 4 :
    print("!!! FATAL ERROR: MUST HAVE FOUR ARGUMENTS <StepFile_Out> <TimeStepFile_Out> <Step-and-TimeStep_FileList_In>")
    sys.exit()

theFileData = []
theTimeFileData = []
#physFileOutName = sys.argv[1]
#timeFileOutName = sys.argv[2]
#fileListInName = sys.argv[3]

fPhysOut = open(sys.argv[1],"w")
fChemOut = open(sys.argv[2],"w")
flIn = open(sys.argv[3])

linesG = flIn.readlines()
eventAdd = 0
eventAddTime = 0
for line in linesG :
    words = line.split()
### physics analysis
    filePhysName = words[0]
    if verbose >= 1 : print("#@#@ START Reading phys file:",filePhysName,"nEv",eventAdd)
    with open(filePhysName, 'r') as csvFile:
        thePhysFileData = csv.reader(csvFile, delimiter=',')
        #3.81488,-9.02518,-20,10.79,18.7361,19,0,0,1,0,1,"proton","proton_G4DNAIonisation"
        for row in thePhysFileData:
#            print(len(row),"NROWS",row)
            lastEvent = int(row[7])
            row[7] = str(int(row[7])+eventAdd)
            rowout = ""
            for ii in range(11) :
                rowout += str(row[ii])+","
#            rowout += '"'+row[11]+'","'+str(row[12])+'"\n'
            rowout += row[11]+','+str(row[12])+'\n'
            fPhysOut.write(rowout)  #str(row).replace("[","").replace("]","")+"\n")

    fileChemName = words[1]
    if verbose >= 1 : print("#@#@ START Reading chem file:",fileChemName,"nEv",eventAdd)
    with open(fileChemName, 'r') as csvTimeFile:
        theChemFileData = csv.reader(csvTimeFile)

        for rowt in theChemFileData:
            lastEventTime = int(rowt[4])
            # print("EVENT",lastEvent) #GDEB
            rowt[4] = str(int(rowt[4])+eventAdd)
            rowtout = str(rowt[0])+","+str(rowt[1])+","+str(rowt[2])+','+rowt[3]+','+str(rowt[4])+"\n"
            fChemOut.write(rowtout) # str(rowt[0:5]).replace("[","").replace("]","")+"\n")

    eventAdd += max(lastEvent,lastEventTime)
    print("ADDED eventAdd",eventAdd,":",lastEvent,lastEventTime)
        
#### Chemistry analysis

fPhysOut.close();

#2.75216,-11.6227,8.95583,OH^0,0,

fChemOut.close();

