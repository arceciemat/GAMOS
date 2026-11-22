// katya
#ifndef GmDataInitialVelocity_hh
#define GmDataInitialVelocity_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialVelocity : public GmVData
{
public:
  GmDataInitialVelocity();
  ~GmDataInitialVelocity();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int );

};
#endif
