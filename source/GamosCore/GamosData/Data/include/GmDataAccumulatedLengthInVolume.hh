#ifndef GmDataAccumulatedLengthInVolume_hh
#define GmDataAccumulatedLengthInVolume_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedLengthInVolume : public GmVDataAccumulated
{
public:
  GmDataAccumulatedLengthInVolume();
  ~GmDataAccumulatedLengthInVolume();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

private:
  std::vector<G4String> theVolumeNames;
};
#endif
