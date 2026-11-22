#ifndef GmDataAccumulatedLength_hh
#define GmDataAccumulatedLength_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedLength : public GmVDataAccumulated
{
public:
  GmDataAccumulatedLength();
  ~GmDataAccumulatedLength();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
