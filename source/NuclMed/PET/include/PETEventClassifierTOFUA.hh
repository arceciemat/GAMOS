#ifndef PETEventClassifierTOFUA_H
#define PETEventClassifierTOFUA_H

#include "NuclMed/Base/include/DetRecHitSet.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUtils/include/GmCone.hh"

class GmAnalysisMgr;
class G4Event;
class GmHitsEventMgr;
class GmCone;
class DetCountScatteringUA;
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <map>
class DetV1stHitAlgorithm;
class DetComptonStudyHistosUA;
typedef std::map<G4double,DetRecHitSet> mddrh;

class PETEventClassifierTOFUA : public GmUserEventAction,
			     public GmUserRunAction
{

public:
  PETEventClassifierTOFUA();
  virtual ~PETEventClassifierTOFUA();
  void BeginOfRunAction(const G4Run* aRun );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction(const G4Run*);
 
  int64_t Classify( gamosRecHitList& rhitList, const G4Event* evt  );
  int64_t ClassifyPET( gamosRecHitList& rhitList, const G4Event* evt );

  void Get511ComptonRecHits( const gamosRecHitList& rhitList );
  G4double GetPETLineDistanceToVtx( const GmLine& line, const G4ThreeVector& eventVtx );
  G4ThreeVector GetEventVertex( const G4Event* evt );

  G4bool CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGamma );
  G4bool CheckOriginalGammaScattered( DetRecHitSetList& rhitsets );
  G4bool CheckRecHitOriginalGammaScattered( GmRecHit* rhit );

private:
  void PrintPETClassificationTable( int64_t nevt );

  void BookHistos();

  void FillDCAHistos( G4double linedist );

  void CheckTimeCoincidence();
  G4bool CheckTimeCoincidence( mddrh::iterator itedss1, mddrh::iterator itedss2 );

  private:
  int64_t theEventClass;

  DetRecHitSetList the511RecHits;
  G4double the511EPrecMin;
  G4double the511EPrecMax;
  G4double thePETLineDistToVtx;
  G4double theCoincidenceTime;

  GmAnalysisMgr* theAnaMgr;

  GmHitsEventMgr* theHitsEventMgr;

  DetCountScatteringUA* theCountScattering;

  GmLine thePetLine;

  G4bool bDumpLM;
  G4bool bDumpPD;

  G4double theComptonRecHitsDist;

  G4int isOriginalGamma;

  DetV1stHitAlgorithm* the1stHitAlgo1;
  DetV1stHitAlgorithm* the1stHitAlgo2;

  G4bool bComptonStudyHistos;
  DetComptonStudyHistosUA* theComptonStudyHistos;

};

#endif
