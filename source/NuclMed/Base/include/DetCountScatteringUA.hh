#ifndef __DetCountScatteringUA__
#define __DetCountScatteringUA__

#include "GamosCore/GamosSD/include/GmSDMessenger.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <map>
#include <set>
class G4Run;
class G4Event;

class DetCountScatteringUA : public GmUserRunAction, public GmUserEventAction, public GmUserSteppingAction, public GmVHistoBuilder
{
public:
  DetCountScatteringUA();
  ~DetCountScatteringUA(){};
  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void UserSteppingAction(const G4Step* aStep);
  G4int GetNScatterOriginalTrack( G4int id );

private:
  std::map<G4int,G4int> theOriginalTracks;
  std::set<G4String> theProcNames;
  std::vector<G4String> theVolumes;
  G4double theEnergyMin;

};

#endif
