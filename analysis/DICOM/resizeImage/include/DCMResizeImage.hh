#ifndef DCMResizeImage_hh
#define DCMResizeImage_hh

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
class DCMResizeImage
{
public:
  DCMResizeImage();
  ~DCMResizeImage(){};

  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();

  void PrintHelp();

private:
  G4String theExeName;

  std::vector<StructData> theStructData;
  
  DicomVImage* theAnalyseImage;
  DicomVImage* theMateDensImage;
  DicomVImage* theStructImage;

  G4String theStructDataListFN;   
  G4bool bChangeMaterials;
  G4String theStructureName;
  G4String theMaterialName;
  G4int theMaterialID;
  G4String theOutFN;
  G4double theDensity;
  G4bool bStOnly;
  G4bool bGivenStructData;
  G4bool bImageG4dcmCT;
  G4bool bChangeDensities;
  G4bool bImageLimits;
  G4double theImageMinX;
  G4double theImageMaxX;
  G4double theImageMinY;
  G4double theImageMaxY;
  G4double theImageMinZ;
  G4double theImageMaxZ;

  DicomReaderG4dcmCT* theG4dcmCTReader;
  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomMgr* theDicomMgr;
};

#endif
  
