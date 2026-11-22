#ifndef GETORGANINFO_hh
#define GETORGANINFO_hh

class DicomMgr;
class DicomReaderMgr;
class DicomParameterMgr;
class DicomVDrawer;
class DicomVImage;
class DicomVLineSet;

#include "DICOM/DICOMBase/include/DicomJobUtils.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DCMDrawDICOM : public DicomJobUtils
{
public:
  DCMDrawDICOM();
  ~DCMDrawDICOM(){};
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();
  
  void DrawImages();

  void PrintHelp();

  void DrawImage(DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName="");

  // CT image to be drawn in grey scale below the dose and nuclear medicine images (0 if none)
  DicomVImage* GetOverlayCTImage();

private:
    
  DicomVDrawer* theDrawer;
  //  DicomMgr* theDicomMgr;
  DicomReaderMgr* theReaderMgr;
  
  G4bool bDrawXY;
  G4bool bDrawXZ;
  G4bool bDrawYZ;
  G4bool bDrawMateID;
  G4bool bDrawStructID;
  G4bool bDrawMateID1Fig;
  G4bool bDrawStructID1Fig;
  G4bool bDrawStruct1Fig;
  G4bool bDrawIsodLines;
  G4bool bOverlayCT;
  std::vector<G4double> theIsodPerCents;
  std::vector<G4double> theIsodPerCentsAbs;
};

#endif
  
