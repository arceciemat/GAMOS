#ifndef DCMPrintValueAtPoint_hh
#define DCMPrintValueAtPoint_hh

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "G4ExceptionSeverity.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <map>
#include <set>
class DicomParameterMgr;
class DicomReaderMgr;
class DicomMgr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DCMPrintValueAtPoint
{
public:
  DCMPrintValueAtPoint();
  ~DCMPrintValueAtPoint(){};

  void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();
  
  void PrintInfoFromImage();

  void PrintHelp();

  
private:
  void DicomException(G4String msg, G4ExceptionSeverity sever = FatalException);

  G4String theExeName;

  G4ThreeVector thePoint;
  G4ThreeVector theExtension;
  G4ThreeVector theUsedExtensionMin;
  G4ThreeVector theUsedExtensionMax;
  G4int theExtensionN;
  DicomVImage* theImage;
  G4bool bPointGiven;
  DicomParameterMgr* theParamMgr;
  DicomReaderMgr* theReaderMgr;
  DicomMgr* theDicomMgr;
};

#endif
  
