#ifndef Exer4UserActionB__HH
#define Exer4UserActionB__HH

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class Exer4UserActionB : public GmUserRunAction, public GmUserEventAction, public GmUserSteppingAction
{
public:
  Exer4UserActionB();
  ~Exer4UserActionB(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  GmAnalysisMgr* theAnaMgr;
  G4double theEDepo;
};

#endif
