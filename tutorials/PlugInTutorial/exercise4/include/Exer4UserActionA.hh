#ifndef Exer4UserActionA__HH
#define Exer4UserActionA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class Exer4UserActionA : public GmUserSteppingAction
{
public:
  Exer4UserActionA();
  ~Exer4UserActionA(){};

   virtual void UserSteppingAction(const G4Step* aStep);

private:
  
};

#endif
