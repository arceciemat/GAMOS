#ifndef GmTrackDataTextFileUA_hh
#define GmTrackDataTextFileUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmVDataUserTextFile.hh"

class GmTrackDataTextFileUA : public GmVDataUserTextFile,
			      public GmUserRunAction,
			      public GmUserTrackingAction,
			      public GmUserSteppingAction
{
public:
  GmTrackDataTextFileUA();
  ~GmTrackDataTextFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PostUserTrackingAction(const G4Track* );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
