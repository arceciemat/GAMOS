#ifndef DCMGetGammaIndex__HH
#define DCMGetGammaIndex__HH
#include "DCM2DOperGammaIndex.hh"
class TH1F;
class GmSqdose;
class DicomReaderCT;
#include "G4ThreeVector.hh"
#include <vector>
class DicomMgr;
class DicomReaderMgr;
class DicomParameterMgr;
class DicomVImage;
class DicomVLineSet;
class DCM2DOperGammaIndex;

enum GIDNormType {GIDN_None, GIDN_atMaxima, GIDN_atMaximum1, GIDN_atPoint };

class DCMGetGammaIndex
{
public:
  DCMGetGammaIndex();
  ~DCMGetGammaIndex(){};

  void Initialise();
  
  void ProcessArguments(int argc,char** argv);
  
  void CheckArguments();

  void ReadFilesAndGetImages();
  
  void PrintHelp();

private:
  void DCMGetGammaIndexXY( GmSqdose* sqdose1, GmSqdose* sqdose2, double perCentLim, double minDistLim, double pval = 1. );
  void DCMGetGammaIndexXZ( GmSqdose* sqdose1, GmSqdose* sqdose2, double perCentLim, double minDistLim, double pval = 1. );
  void DCMGetGammaIndexYZ( GmSqdose* sqdose1, GmSqdose* sqdose2, double perCentLim, double minDistLim, double pval = 1. );

  void SetImageMaxValue(DicomVImage* image, G4double maxValue);

  void CalculateDumpPerSt();
  
protected:

  G4bool bDistanceByVoxel;

  G4bool bMaxAtPoint;
  G4double theMaxAtPointFactor;
  G4ThreeVector theMaxAtPoint;
  DicomReaderCT* theDicomReaderCT;
  std::vector<G4String> theStListPerSt; // histos per Structure
  G4bool bDumpPerSt;
  G4bool bOnly1St;
  G4String theOnly1StName;
  G4bool bDrawPerSt;
  G4bool bGetGammaIndex;
  G4bool bAdjustVoxels;

  G4ThreeVector thePointNorm;

  DicomVImage* theStructImage;
  
  DicomParameterMgr* theParamMgr;
  DicomMgr* theDicomMgr;
  DicomReaderMgr* theReaderMgr;

  G4String theExeName;
  GIDNormType theNormType;

  G4double theDefaultPercent;
  G4double theDefaultDistance;
  std::vector< std::pair<G4double,G4double> > thePercentDistances;
  G4bool bDrawXY;
  G4bool bDrawXZ;
  G4bool bDrawYZ;
  std::vector<DicomVLineSet*> theLineSets;
  GIPCT_Values thePerCentType;
  G4ThreeVector thePointPerCent;
  G4bool bUseMateID;
  G4bool bUseStructID;
  G4bool bErrors;
  G4bool bOutputFile;
  G4double theGammaOverCut;

  DCM2DOperGammaIndex* theOperGammaIndex;
};

#endif
  
  
