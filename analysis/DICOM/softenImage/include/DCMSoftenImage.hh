#ifndef DCMSoftenImage_hh
#define DCMSoftenImage_hh

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "G4ExceptionSeverity.hh"
#include "G4ThreeVector.hh"
#include <map>
#include <set>
class DicomParameterMgr;
class DicomReaderMgr;
class DicomMgr;
class DicomReaderG4dcmCT;

struct StructData {
  G4String stName;
  G4String mateName;
  G4double density;
  G4bool bOnly;
  size_t stID;
  G4int mateID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DCMSoftenImage
{
public:
  DCMSoftenImage();
  ~DCMSoftenImage(){};

  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();

  void PrintHelp();

private:
  G4String theExeName;

  G4String theOutFN;
  G4double theFactor;
  G4int theNAround;
  G4double theMaxValue;
  G4double theMinSpareValue;
  G4double theMaxSpareValue;

  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomMgr* theDicomMgr;
};

#endif
  
