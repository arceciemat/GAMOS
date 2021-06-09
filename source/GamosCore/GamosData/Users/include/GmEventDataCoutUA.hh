#ifndef GmEventDataCoutUA_hh
#define GmEventDataCoutUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmVDataUser.hh"

class GmEventDataCoutUA : public GmVDataUser,
			  public GmUserRunAction,
			  public GmUserEventAction,
			  public GmUserSteppingAction
{
public:
  GmEventDataCoutUA();
  ~GmEventDataCoutUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction(const G4Event* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
