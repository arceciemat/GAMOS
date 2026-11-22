#ifndef GmTrackDataHistosUA_hh
#define GmTrackDataHistosUA_hh

#include "GmVDataUserHistos.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmTrackDataHistosUA : public GmVDataUserHistos,
			    public GmUserRunAction,
			    public GmUserTrackingAction,
			    public GmUserSteppingAction

{
public:
  GmTrackDataHistosUA();
  ~GmTrackDataHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PostUserTrackingAction(const G4Track* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:

};

#endif
