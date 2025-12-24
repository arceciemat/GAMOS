#ifndef GETORGANINFO_hh
#define GETORGANINFO_hh

#include "DICOM/DICOMBase/include/DicomVImageStr.hh"
#include "G4ExceptionSeverity.hh"
class DicomMgr;
class DicomReaderMgr;
class DicomParameterMgr;
class GmAnalysisMgr;
#include "TH1F.h"
#include "TProfile.h"
#include "TFile.h"
#include "globals.hh"
#include <vector>
#include <map>
#include <set>
typedef std::map<size_t,TH1F*> mh;
typedef std::pair<mh,mh> pmhmh;
typedef std::map<size_t,TProfile*> mhp;
typedef std::pair<mhp,mhp> pmhpmhp;
typedef std::multimap<G4double,G4double> mmdd;
class GmReadPhantomStMgr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DCMGetOrganInfo
{
public:
  DCMGetOrganInfo();
  ~DCMGetOrganInfo();

  void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();
  
  void ReadLinesDefinition();

  void GetInfoFromImage();

  void PrintHelp();

  std::pair<G4double,G4double> GetDosimVolume(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id);
  std::pair<G4double,G4double> GetDosimDPercentage(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id);
  std::pair<G4double,G4double> GetDosimDmincc(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id);
  std::pair<G4double,G4double> GetDosimDmaxcc(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id);
  std::pair<G4double,G4double> GetDosimDmean(std::map<size_t,mmdd> voxelDoseErrors11, size_t id);
  
private:
  pmhmh FillDoseHistos( DicomVImage* analyseImage, DicomVImage* perCentImage );
  void WriteHistosToFile( pmhmh hpair, TFile* histosFile );
  void WriteHistosToFile( pmhpmhp hpair, TFile* histosFile );
  void DicomException(G4String msg, G4ExceptionSeverity sever = FatalException);
  DicomVImage* FillImageDoseInSt( std::map<size_t, long double> theDosesPerSt );
  DicomVImage* BuildAnalyseErrorImage( DicomVImage* image, G4double relErr );
  
  DicomVImage* theAnalyseImage = 0; //FILE THAT CONTAINS THE GEOMETRY: CT/NM/DOSE
  DicomVImage* theAnalyseSqImage = 0; //FILE THAT CONTAINS THE GEOMETRY SQUARED VALUES: CT/NM/DOSE
  DicomVImage* theAnalyseErrorImage = 0; //FILE THAT CONTAINS THE GEOMETRY ERRORS: CT/NM/DOSE
  DicomVImageStr* theStructIDImage = 0;
  DicomVImage* theAnalyseMateDensImage = 0;
  DicomVImage* thePerCentImage;
  DicomVImage* theAnalyseImageRandom;  
  G4bool bDepoEnergy;  // CALCULATE DEPO ENERGY INSTEAD OF DOSE
  G4bool bVolumeMass;    // VOLUME MASS PER STRUCTURE
  G4bool bPerMate;    // DOSE/ACTIVITY/VOLUME PER MATERIAL (INSTED OF PER STRUCTURE)

  G4String theExeName;

  std::map<G4int,G4String> theClassifNames;

  //  G4double theMultFactor;
  G4double theHistoDoseMin;
  G4double theHistoDoseMax;
  G4bool bUserHistoDoseMin;
  G4bool bUserHistoDoseMax;
  
  G4bool bHistos;
  G4bool bLog;

  G4String theStLinesFN;
  std::map<G4String,G4int> theStLines;
  std::map<G4String,G4int> theStColor;
  std::map<G4String,G4int> theStLineStyle;
  std::map<G4String,short> theStLineWidth;
  G4int theNColorsInFile;
 
  DicomParameterMgr* theParamMgr;
  DicomMgr* theDMgr;
  DicomReaderMgr* theReaderMgr;

  DIModality theAnalyseModality;  // Modality of image to be analysed

  //  std::map< G4int, std::set<G4double> > theVoxelDosePerSt; // struct_ID set of doses (so they are ordered)
  G4bool bDosimQ;
  size_t theNofEvents;
  std::vector<G4String> theDosimQuantities;
  G4double theVoxelVolume_anal;
  G4double thePrescribedDose;
  G4String theOutputName;
  G4int thePrintStID;
  G4String thePrintStName;
  GmAnalysisMgr* theAnaMgr;
  G4bool bRandom;
  G4bool bDiffDim;
  G4double theXInit0;
  G4int theCanvasSizeXY;
  GmReadPhantomStMgr* theRPStMgr;

  G4bool bDrawImageDoseInSt;
  G4double theHistosLogN;
  G4bool bHistosLogN;
  std::vector<G4String> theExcludeStNames;
  std::map<size_t, long double> theDosesPerSt;
  std::map<size_t, long double> theDoseErrorsPerSt;
  G4double theErrorImageRelErr;
  G4double theNErrorSigmas;
  
};

#endif
  
