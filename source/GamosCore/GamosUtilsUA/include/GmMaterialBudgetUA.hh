#ifndef GmMaterialBudgetUA_hh
#define GmMaterialBudgetUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <set>
#include <map>

class GmMaterialBudgetUA : public GmUserRunAction, 
		       public GmUserSteppingAction, 
		       public GmUserTrackingAction, 
		       public GmVHistoBuilder
{
public:
  GmMaterialBudgetUA();
  ~GmMaterialBudgetUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PostUserTrackingAction(const G4Track* aTrack );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:
  void BookHistos(G4int index);

  std::set<G4int> theHistos;

  std::map<G4int,G4double> theMatBud;
};

#endif
