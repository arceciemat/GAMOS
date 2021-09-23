#ifndef SHActivationUA_h
#define SHActivationUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"

#include <map>
class G4ParticleDefinition;
#include "SHMetastableIsotMgr.hh"
class SHRadDecayChainSet;

class SHActivationUA : public GmUserRunAction, 
		       public GmUserEventAction,
		       public GmUserTrackingAction
{
  
public:
  SHActivationUA();
  ~SHActivationUA();

public:
  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfRunAction( const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* );

protected:
  void InitialiseRun();  
  void SetPeriods( const std::vector<G4String>& periods );
  void ReadPeriods(const G4String& fileName);
  void PrintActivities();

protected:
  G4double theMaximumTime;
  G4double theMinimumTime;
  std::map<G4String,G4double> theParticles;
  std::vector<G4double> theTimes;
  //  std::vector<ppdd> theParticlesInEvent;
  //  std::map<G4ParticleDefinition*,G4double> theParticlesInEvent;
  SHMetastableIsotMgr* theMetastableIsotMgr;
  G4double theTotalActivity;
  std::map<G4double,G4bool> thePeriods;

  SHRadDecayChainSet* theRadDecayChainSet;
};

#endif
