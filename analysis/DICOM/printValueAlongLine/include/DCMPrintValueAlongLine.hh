#ifndef GETORGANINFO_hh
#define GETORGANINFO_hh

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "G4ExceptionSeverity.hh"
class DicomMgr;
class DicomReaderMgr;
class DicomParameterMgr;
class DicomLineSet;
#include "globals.hh"
#include <vector>
#include <map>
#include <set>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DCMPrintValueAlongLine
{
public:
  DCMPrintValueAlongLine();
  ~DCMPrintValueAlongLine(){};

  void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();
  
  void ReadLinesDefinition();

  void GetInfoFromImages();

  void PrintHelp();

private:
  void DicomException(G4String msg, G4ExceptionSeverity sever = FatalException);

  G4String theExeName;

  G4bool bHistos;

  DicomParameterMgr* theParamMgr;
  DicomMgr* theDMgr;
  DicomReaderMgr* theReaderMgr;

  std::vector<DicomVImage*> theImages;

  G4ThreeVector theLinePos;
  G4ThreeVector theLineDir;
  G4ThreeVector theLinePos0;
  DicomLineSet* theLineSet;
  /*  DIModality theAnalyseModality;  // Modality of image to be analysed

  std::map< G4int, std::set<G4double> > theVoxelDosePerSt; // struct_ID set of doses (so they are ordered)
  G4bool bDosimQ;
  size_t theNofParticles;
  G4double theVoxelVolume_anal;
  G4double thePrescribedDose;
  G4String theOutputName;
  G4int thePrintStID;
  G4String thePrintStName;
  */
  
  G4String theLineListFN;
  G4int theAlongLineInput;
  G4double theLineStep;
};

#endif
  
