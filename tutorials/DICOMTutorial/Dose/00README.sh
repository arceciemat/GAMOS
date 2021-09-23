### PRODUCTION OF DOSE FILE IN GAMOS FORMAT
DICOM2G4 Data.dat

### VISUALITATION OF DOSE WITH ISODOSES 
drawG4dcm -fSqDose testDose.sqdose -fIsodLines isodoses.lis -palette 55 
mkdir gif.360points
mv *.gif gif.360points

drawG4dcm -fSqDose testDose.sqdose -fIsodLines isodoses.lis -palette 55  -isodNAngles 10 
mkdir gif.10points
mv *.gif gif.10points
