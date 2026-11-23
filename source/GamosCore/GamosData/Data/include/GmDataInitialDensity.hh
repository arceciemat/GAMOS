#ifndef GmDataInitialDensity_hh
#define GmDataInitialDensity_hh
class G4VPhysicalVolume;

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GmVDataInitialMaterial.hh"

class GmDataInitialDensity : public GmVData, public GmVDataInitialMaterial
{
public:
  GmDataInitialDensity();
  ~GmDataInitialDensity();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* aEvent, G4int index = 0);
  virtual G4double GetValueFromStackedTrack( const G4Track* aTrack, G4int index = 0 );

private:
  G4double GetDensityFromPV( G4VPhysicalVolume* pv );

};
#endif
