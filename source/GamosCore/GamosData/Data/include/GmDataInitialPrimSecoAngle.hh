#ifndef GmDataInitialPrimSecoAngle_hh
#define GmDataInitialPrimSecoAngle_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialPrimSecoAngle : public GmVData
{
public:
  GmDataInitialPrimSecoAngle();
  ~GmDataInitialPrimSecoAngle();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int );
  G4double GetValueFromTrack( const G4Track*, G4int );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
