#ifndef GmDataInitialParallelPVCopyNumber_hh
#define GmDataInitialParallelPVCopyNumber_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"
#define private public
#include "G4ParallelWorldScoringProcess.hh"

class GmDataInitialParallelPVCopyNumber : public GmVDataInt
{
public:
  GmDataInitialParallelPVCopyNumber();
  ~GmDataInitialParallelPVCopyNumber();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* aEvent, G4int index = 0);

private:
  G4ParallelWorldScoringProcess* theParallelWorldProcess;

};
#endif
