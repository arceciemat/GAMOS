#ifndef GmRecHitsHistosUA_HH
#define GmRecHitsHistosUA_HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "G4ThreeVector.hh"
#include <set>
#include "globals.hh"
class GmAnalysisMgr;

class GmRecHit;

class GmRecHitsHistosUA: public GmUserRunAction, public GmUserEventAction
{
public:
  GmRecHitsHistosUA();
  ~GmRecHitsHistosUA();

  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );

  void BookHistos();
  void SumHistos();

private:
  G4int GetHitTypeInt(const GmRecHit* hit );
  G4ThreeVector GetEDepoWidth( GmRecHit* hit );

private:
  std::set<G4String> theSDTypes;
  GmAnalysisMgr* theAnaMgr;
};

#endif
