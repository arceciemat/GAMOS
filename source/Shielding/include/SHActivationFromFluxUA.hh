#ifndef SHActivationFromFluxUA_h
#define SHActivationFromFluxUA_h 1
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "SHActivationUA.hh"
class SHRadDecayScorerFromFlux;

class SHActivationFromFluxUA : public SHActivationUA,
			       public GmUserSteppingAction
{
  
public:
  SHActivationFromFluxUA();
  ~SHActivationFromFluxUA();
  
public:
  virtual void BeginOfRunAction( const G4Run* );
  virtual void EndOfEventAction( const G4Event* );
  virtual void EndOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* );
  
private:
  SHRadDecayScorerFromFlux* theFluxScorer;

};

#endif
