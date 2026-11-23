#ifndef GmStepDataCoutUA_hh
#define GmStepDataCoutUA_hh

#include "GmVDataUser.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmStepDataCoutUA : public GmVDataUser,
			 public GmUserRunAction,
			 public GmUserSteppingAction
{
public:
  GmStepDataCoutUA();
  ~GmStepDataCoutUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
