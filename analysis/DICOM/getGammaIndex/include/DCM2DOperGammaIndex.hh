#ifndef DCM2DOperGammaIndex__HH
#define DCM2DOperGammaIndex__HH

#include "DICOM/DICOMBase/include/DicomVOperator.hh"
#include "G4ThreeVector.hh"
#include <map>
#include <vector>
class TH2F;
class DicomVImage;
class DicomParameterMgr;
class DicomVDrawer;
class DicomDrawerROOT;
class DicomPolygonSet;

enum GIPCT_Values { GIPCT_None, GIPCT_Max2Images, GIPCT_ImageMax, GIPCT_Voxel, GIPCT_AtPoint };

class DCM2DOperGammaIndex 
{
public:
  DCM2DOperGammaIndex(G4int iOrder = INT_MAX, G4String name = "DCM2DOperGammaIndex");
  ~DCM2DOperGammaIndex(){};

public:
  //  void OperateXYEqual( DicomVImage* image1, DicomVImage* image2, double perCentLim, double minDistLim, double pval = 1., DicomVImage* imageOut = 0 );
  void OperateXY( DicomVImage* image1, DicomVImage* image2, DicomVImage* imageErr1, DicomVImage* imageErr2, double perCentLim, double minDistLim, double pval = 1., DicomVImage* imageOut = 0 );
  void OperateXZ( DicomVImage* image1, DicomVImage* image2, double perCentLim, double minDistLim, double pval = 1., DicomVImage* imageOut = 0 );
  void OperateYZ( DicomVImage* image1, DicomVImage* image2, double perCentLim, double minDistLim, double pval = 1., DicomVImage* imageOut = 0 );

  void IsMaxAtPoint( G4bool bm ) {
    bMaxAtPoint = bm;
  }
  void SetMaxAtPoint( G4ThreeVector pt ) {
    theMaxAtPoint = pt;
  }
  void SetMaxAtPointFactor( G4double mptf ) {
    theMaxAtPointFactor = mptf;
  }
  void SetPerCentType( GIPCT_Values rv ) {
    thePerCentType = rv;
  }
  void SetPointPerCent( G4ThreeVector pt ) {
    thePointPerCent = pt;
  }
  void SetOnlyDiff( G4bool od ) {
    bOnlyDiff = od;
  }
  void SetNErrorSigmas( G4double nsig ) {
    theNErrorSigmas = nsig;
    if( nsig != 0. ) {
      bErrors = true;
    } else {
      bErrors = false;
    }
  }
  
protected:
  //  G4bool bImageEqual;

private:
  G4double GetMinAtVoxel( G4double imageMin, G4double phantomMin, G4double voxelWidth );
  G4double GetMaxAtVoxel( G4double imageMax, G4double phantomMax, G4int nVox, G4double voxelWidth );

  G4bool bMaxGammaValue;
  G4double theMaxGammaValue;
  G4double MAXDISTVOXEL;
  G4bool bMaxAtPoint;
  G4double theMaxAtPointFactor;
  G4ThreeVector theMaxAtPoint;
  DicomParameterMgr* theParamMgr;
  DicomDrawerROOT* theDrawer;
  GIPCT_Values thePerCentType; 
  G4ThreeVector thePointPerCent;
  G4bool bOnlyDiff;
  G4bool bErrors;// (INCLUDE ERROR IMAGE)
  G4double theNErrorSigmas;
  G4bool bHisto1D;
};

#endif
  
  
