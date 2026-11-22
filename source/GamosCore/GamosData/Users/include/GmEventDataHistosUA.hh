#ifndef GmEventDataHistosUA_hh
#define GmEventDataHistosUA_hh

#include "GmVDataUserHistos.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmEventDataHistosUA : public GmVDataUserHistos,
			    public GmUserRunAction,
			    public GmUserEventAction,
			    public GmUserSteppingAction

{
public:
  GmEventDataHistosUA();
  ~GmEventDataHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void BeginOfEventAction(const G4Event* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:

};

#endif
