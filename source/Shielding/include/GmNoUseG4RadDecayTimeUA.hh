//
#ifndef GmNoUseG4RadDecayTimeUA_hh
#define GmNoUseG4RadDecayTimeUA_hh
#include <map>
class G4ParticleDefinition;

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <set>

class SHRadDecayChainSet;

class GmNoUseG4RadDecayTimeUA : public GmUserTrackingAction, 
				public GmUserSteppingAction,
				public GmUserEventAction,
				public GmUserRunAction,
				public GmUserStackingAction
{ 
public:
  GmNoUseG4RadDecayTimeUA();
  ~GmNoUseG4RadDecayTimeUA(){};
  virtual void BeginOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event*);
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );

  G4double GetLifeTime( G4ParticleDefinition* part );

private:
  void SetTime( const G4Step* aStep );
  G4double GetLambdaTerm( std::vector<G4double>& lambdas, G4int iExclude = -1);
  G4double GetExpLambdaTerm( G4double trkTime, std::vector<G4double>& lambdas, G4int iInclude );

private:
  std::map<G4ParticleDefinition*,G4double> theLifeTimes;

  SHRadDecayChainSet* theDecayChainSet;

  std::set<G4int> thePrimaries;

  G4double theMinimumActivity;
  G4double theMaxPrimaryTime; // time of latest primary track
  G4double theMaxTracksTime; // max difference allowed between a track time and theMaxPrimaryTime
};

#endif
