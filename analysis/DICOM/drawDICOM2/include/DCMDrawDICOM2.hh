#ifndef GETORGANINFO_hh
#define GETORGANINFO_hh

class DicomMgr;
class DicomReaderMgr;
class DicomParameterMgr;
class DicomVDrawer;
class DicomVImage;
class DicomVLineSet;
class DicomVReaderImage;

#include "DICOM/DICOMBase/include/DicomJobUtils.hh"
#include <vector>
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DCMDrawDICOM2 : public DicomJobUtils
{
public:
  DCMDrawDICOM2();
  ~DCMDrawDICOM2(){};
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();
  
  void DrawImages();

  void PrintHelp();

  void DrawImage(DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName="");

  void SetPalette();
  DicomVImage* BuildCombinedImage( DicomVImage* image1, DicomVImage* image2 );
  DicomVImage* GetImage( DicomVReaderImage* imageReader );

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
  std::vector<G4double> theIsodPerCents;
  std::vector<G4double> theIsodPerCentsAbs;

  std::vector<G4String> theFileNames;
};

#endif
  
