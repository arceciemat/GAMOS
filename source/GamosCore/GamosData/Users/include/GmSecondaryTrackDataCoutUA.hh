#ifndef GmSecondaryTrackDataCoutUA_hh
#define GmSecondaryTrackDataCoutUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmVDataUser.hh"

class GmSecondaryTrackDataCoutUA : public GmVDataUser,
				   public GmUserRunAction,
				   public GmUserSteppingAction
{
public:
  GmSecondaryTrackDataCoutUA();
  ~GmSecondaryTrackDataCoutUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
