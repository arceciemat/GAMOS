#ifndef GmStepRandomWeightUA_hh
#define GmStepRandomWeightUA_hh 1

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmStepRandomWeightUA : public GmUserSteppingAction, 
		      public GmUserRunAction
{
public:   

  GmStepRandomWeightUA();
  virtual ~GmStepRandomWeightUA();
  
  virtual void BeginOfRunAction( const G4Run* aRun );
  virtual void UserSteppingAction( const G4Step* aStep );

private:
  G4double theMinWeight;
  G4double theMaxWeight;
  G4double theWeightSpan;


};


#endif
