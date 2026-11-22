#ifndef SPECTEventClassifierUA_H
#define SPECTEventClassifierUA_H

enum SPAlgo {SPByEnergy, SPByXYRadius, SPByDistanceTo1st, SPByComptonCone, SPBy2ComptonCone3XYRadius, SPByDistanceToOther};

#include "NuclMed/Base/include/DetRecHitSet.hh"

#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "G4ThreeVector.hh"

#include <map>
#include <set>

class GmAnalysisMgr;
class GmHitsEventMgr;
class DetCountScatteringUA;
class GmMovementMgr;
class G4LogicalVolume;
class DetV1stHitAlgorithm;
class DetComptonStudyHistosUA;

typedef std::map<G4String,G4ThreeVector> mapSTV;


class SPECTEventClassifierUA : public GmUserRunAction, 
			       public GmUserEventAction, 
			       public GmUserSteppingAction 
{

public:
  SPECTEventClassifierUA();
  virtual ~SPECTEventClassifierUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* );

  int64_t Classify( gamosRecHitList& rhitList, const G4Event* evt  );
  int64_t ClassifySPECT( gamosRecHitList& rhitList, const G4Event* evt );

  void GetGoodComptonRecHits( const gamosRecHitList& rhitList );
  G4double GetSPECTLineDistanceToVtx( const GmLine& line, const G4ThreeVector& eventVtx );
  G4ThreeVector GetEventVertex( const G4Event* evt );

  G4bool CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGamma );
  G4bool CheckOriginalGammaScattered( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitOriginalGammaScattered( GmRecHit* rhit );

private:
  void PrintSPECTClassificationTable( int64_t nevt );

  void BookHistos();

  void FillDCAHistos( G4double linedist );

  void GetCollimatorCentres();

  SPAlgo SelectPosAlgorithm(const G4String& SelectPosStr);

  mapSTV::const_iterator GetClosestCollimator( G4ThreeVector pos );

  void CleanCollimatorsTraversed();

  G4int CheckTraverseCollimator(DetRecHitSetList& rhitsets, mapSTV::const_iterator colliIteClosest, G4bool diffOrigGamma );

private:
  int64_t theEventClass;

  DetRecHitSetList theGoodRecHits;
  G4double theGammaEnergy;
  G4double theEPrecMin;
  G4double theEPrecMax;
  G4double theSPECTLineDistToVtx;

  GmAnalysisMgr* theAnaMgr;

  GmHitsEventMgr* theHitsEventMgr;

  DetCountScatteringUA* theCountScattering;

  G4bool bDumpLM;
  G4bool bDumpPD;

  G4double theComptonRecHitsDist;

  G4int isOriginalGamma;

  DetV1stHitAlgorithm* the1stHitAlgo;
 
  G4int theNHits2Reject;

  mapSTV theCollimatorLongNamesAndCentres;
  G4LogicalVolume* theCollimatorLogVol;
  std::map<G4int, mapSTV::const_iterator > theCollimatorTraversed;

  GmMovementMgr* theMovementMgr;

  std::map<unsigned int, unsigned int> theEventClassifierMap;

  G4bool bComptonStudyHistos;
  DetComptonStudyHistosUA* theComptonStudyHistos;

};

#endif
