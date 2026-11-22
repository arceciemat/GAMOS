#ifndef __RTPhaseSpace_extUA__
#define __RTPhaseSpace_extUA__

#include "RTPhaseSpaceUA.hh"
using namespace CLHEP;

class RTPhaseSpace_extUA : public RTPhaseSpaceUA
{
public:
  RTPhaseSpace_extUA();
  ~RTPhaseSpace_extUA();
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void BeginOfEventAction(const G4Event* anEvent);
  virtual void EndOfEventAction(const G4Event* anEvent);
  std::vector<G4double> GetZStops() const {
    return theZStops; }

private:
  G4bool CreateIAEAheaders();
  G4bool CreateIAEAheader(G4double zstop);
  int CloseIAEAFiles();
  int CloseIAEAHeaderFiles(G4int nevents);
  int CloseIAEARecordFiles();
  void StoreTrackRecord( const G4Step* aStep, G4double zstop);
  
private:

};

#endif
