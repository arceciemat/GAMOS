#ifndef GmFutureFilterUA__HH
#define GmFutureFilterUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

class GmStepMgr;

class GmFutureFilterUA : public GmUserSteppingAction, public GmUserEventAction
{
public:
  GmFutureFilterUA();
  ~GmFutureFilterUA(){};

  virtual void UserSteppingAction( const G4Step* );
  virtual void EndOfEventAction( const G4Event* );

  void SetStepMgr( GmStepMgr* sm ) {
    theStepMgr = sm;
  }

private:
  GmStepMgr* theStepMgr;
};

#endif
