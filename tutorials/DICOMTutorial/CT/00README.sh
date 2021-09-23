DICOM2G4 Data.dat
mv testCT.g4dcm testCT.c2.g4dcm 
drawG4dcm -fG4DCM testCT.c2.g4dcm -fSt Patient_structures.dcm -fStLines structures.lis -bDrawXZ 0 -bDrawYZ 0 

cd readCT/
gamos readCT.in

analyseSqdose sqdose.out.readCT
root dose_analyseSqdose.root
