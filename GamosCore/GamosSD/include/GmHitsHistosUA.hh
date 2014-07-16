#ifndef GmHitsHistosUA_HH
#define GmHitsHistosUA_HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "G4ThreeVector.hh"
#include <set>
#include "globals.hh"
class GmAnalysisMgr;

class GmHit;

class GmHitsHistosUA: public GmUserRunAction, public GmUserEventAction
{
public:
  GmHitsHistosUA();
  ~GmHitsHistosUA();

  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );

  void BookHistos();
  void SumHistos();

private:
  G4int GetHitTypeInt(const GmHit* hit );
  G4ThreeVector GetEDepoWidth( GmHit* hit );

private:
  std::set<G4String> theSDTypes;
  GmAnalysisMgr* theAnaMgr;
};

#endif
