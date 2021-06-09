#ifndef GmTrackDataBinFileUA_hh
#define GmTrackDataBinFileUA_hh

#include "GmVDataUserBinFile.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmTrackDataBinFileUA : public GmVDataUserBinFile,
			     public GmUserRunAction,
			     public GmUserTrackingAction,
			     public GmUserSteppingAction
{
public:
  GmTrackDataBinFileUA();
  ~GmTrackDataBinFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PostUserTrackingAction(const G4Track* );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
