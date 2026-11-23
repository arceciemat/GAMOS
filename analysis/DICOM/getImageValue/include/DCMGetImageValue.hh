#ifndef DCMGetImageValue_hh
#define DCMGetImageValue_hh

#include "DICOM/DICOMBase/include/DicomVImageStr.hh"
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
class DCMGetImageValue
{
public:
  DCMGetImageValue();
  ~DCMGetImageValue(){};

  void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();

  void GetClosestValue();
  
  void PrintHelp();

private:
  G4String theExeName;

  DicomVImage* theAnalyseImage;
  G4bool bImageLimits;
  G4double theImageMinX;
  G4double theImageMaxX;
  G4double theImageMinY;
  G4double theImageMaxY;
  G4double theImageMinZ;
  G4double theImageMaxZ;

  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomMgr* theDicomMgr;

  G4double theValue, thePrecision;
};

#endif
  
