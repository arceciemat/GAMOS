#ifndef DCMDmToDw_hh
#define DCMDmToDw_hh

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
class DCMDmToDw
{
public:
  DCMDmToDw();
  ~DCMDmToDw(){};

  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();

  void PrintHelp();

private:
  size_t GetMaterialIndex( G4double Hval);
    
  G4String theExeName;

  DicomReaderG4dcmCT* theG4dcmCTReader;
  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomMgr* theDicomMgr;

  G4String theG4dcmCTFN;
  G4String theSqdoseFN;
  G4String theSPRFN;
  G4String theOutFN;
};

#endif
  
