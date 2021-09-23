#ifndef GmGeneratorScanVisUA_HH
#define GmGeneratorScanVisUA_HH

class G4Event;

#include "G4ThreeVector.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#ifndef GAMOS_NO_ROOT
#include "TCanvas.h"
#include "TH2F.h"
#include "TColor.h"
#endif

enum planeFig {PF_XY, PF_XZ, PF_YZ};
enum CBVIT { CBVIT_LV, CBVIT_PV, CBVIT_T};

typedef struct ColPoint {
  double x, y, z;
  char tag[64];          // border name
} ColPoint;
typedef std::pair<G4ThreeVector,G4ThreeVector> pvv;

class GmGeometryUtils;
class GmGeneratorScanVis;

class GmGeneratorScanVisUA: public GmUserRunAction, 
			  public GmUserEventAction, 
			  public GmUserSteppingAction 
{
public:
  GmGeneratorScanVisUA();
  ~GmGeneratorScanVisUA();
  virtual void UserSteppingAction(const G4Step* aStep);
#ifndef GAMOS_NO_ROOT
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  //  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );
  void ReadLinesFile(G4String linesFN);

  void SetGenerator( GmGeneratorScanVis* gener ) {
    theGenerator = gener;
  }
  
private:
  void SetColors();
  EColor GetColor( int icol );

  G4double theStepFig;
  G4double thePosTransv;
  G4double theScanWorldMinX;
  G4double theScanWorldMaxX;
  G4double theScanWorldMinY;
  G4double theScanWorldMaxY;
  G4double theScanWorldMinZ;
  G4double theScanWorldMaxZ;
  planeFig thePlaneFig;
  G4String thePlaneFigName;
  G4double theMarkerSize;
  G4int theMarkerStyle;
  
  std::map<G4String, std::vector<G4ThreeVector> > thePoints;
  TCanvas* theCanvas;
  TH2F* theHisto;
  G4int theWorldNDigits;
  G4int theNameNDigits;

  FILE * theFileOut;
  std::map<G4String,EColor> theColorMap;
  G4bool bColorByName;
  G4int theCanvasNPixels;
  // vertical and horizontal limits
  G4double theDrawMinH;
  G4double theDrawMaxH;
  G4double theDrawMinV;
  G4double theDrawMaxV;
  
  GmGeometryUtils* theGeomUtils;

  G4bool bDrawBorderNames;

  std::map<G4int,EColor> theGoodColors;
  std::map<G4String,G4int> theColorVolumeIdx;
  G4String theColorByVolumeIndexFN;
  CBVIT theColorByVolumeIndexType;
  G4bool bColorByVolumeIndex;
  G4bool bPrintAllTogether;

  G4double thePrecision;
  G4ThreeVector theLastPos;
  std::vector<pvv> theLinesXY;
  std::vector<pvv> theLinesXZ;
  std::vector<pvv> theLinesYZ;
  GmGeneratorScanVis* theGenerator;
#endif
};
#endif
