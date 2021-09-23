#ifndef SHStudyDecayChannelsUA_h
#define SHStudyDecayChannelsUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <set>
class G4ParticleDefinition;

class SHStudyDecayChannelsUA : public GmUserRunAction, 
			       public GmUserTrackingAction,
			       public GmUserSteppingAction
{
  
public:
  SHStudyDecayChannelsUA();
  ~SHStudyDecayChannelsUA();
  
public:
  virtual void BeginOfRunAction( const G4Run* );
  virtual void EndOfRunAction( const G4Run* );
  virtual void PostUserTrackingAction(const G4Track* );
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  std::set<G4ParticleDefinition*> theParticles;
};

#endif
