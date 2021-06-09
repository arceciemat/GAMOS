#ifndef GmTrackDataCoutUA_hh
#define GmTrackDataCoutUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmVDataUser.hh"

class GmTrackDataCoutUA : public GmVDataUser,
			      public GmUserRunAction,
			      public GmUserTrackingAction,
			      public GmUserSteppingAction
{
public:
  GmTrackDataCoutUA();
  ~GmTrackDataCoutUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PostUserTrackingAction(const G4Track* );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
