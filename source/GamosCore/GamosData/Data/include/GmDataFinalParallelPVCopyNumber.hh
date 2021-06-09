#ifndef GmDataFinalParallelPVCopyNumber_hh
#define GmDataFinalParallelPVCopyNumber_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"
#define private public
#include "G4ParallelWorldScoringProcess.hh"

class GmDataFinalParallelPVCopyNumber : public GmVDataInt
{
public:
  GmDataFinalParallelPVCopyNumber();
  ~GmDataFinalParallelPVCopyNumber();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

private:
  G4ParallelWorldScoringProcess* theParallelWorldProcess;

};
#endif
