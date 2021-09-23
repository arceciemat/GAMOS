#ifndef __GmRadDecayGammaAngleCorrelationUA__
#define __GmRadDecayGammaAngleCorrelationUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include <map>
#include "G4ThreeVector.hh"
class SHRadDecayChainSet;

class GmRadDecayGammaAngleCorrelationUA :  public GmUserStackingAction, 
					   public GmUserSteppingAction, 
					   public GmUserTrackingAction, 
					   public GmUserEventAction
{
public:
  GmRadDecayGammaAngleCorrelationUA();
  ~GmRadDecayGammaAngleCorrelationUA(){};
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void BeginOfEventAction( const G4Event* );

private:
  SHRadDecayChainSet* theDecayChainSet;
  std::map<G4String,G4ThreeVector> theGammas;
};

#endif
