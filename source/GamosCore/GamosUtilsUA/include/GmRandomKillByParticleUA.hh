#ifndef __GmRandomKillByParticleUA__
#define __GmRandomKillByParticleUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <map>

class GmRandomKillByParticleUA :  public GmUserStackingAction, public GmUserRunAction
{
public:
  GmRandomKillByParticleUA();
  ~GmRandomKillByParticleUA(){};
  virtual void BeginOfRunAction( const G4Run* );
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );
  void ReadFile();

  private:
  G4String theFileName;
  G4double theMultFactor;
  std::map<G4String,G4double> theParticleProb;
};

#endif
