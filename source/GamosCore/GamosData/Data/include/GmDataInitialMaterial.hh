#ifndef GmDataInitialMaterial_hh
#define GmDataInitialMaterial_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"
#include "GmVDataInitialMaterial.hh"

class GmDataInitialMaterial : public GmVDataString, public GmVDataInitialMaterial
{
public:
  GmDataInitialMaterial();
  ~GmDataInitialMaterial();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );
  virtual G4String GetStringValueFromEvent( const G4Event* aEvent );
  virtual G4String GetStringValueFromStackedTrack( const G4Track* aTrack );

private:
  G4String GetNameFromPV( G4VPhysicalVolume* pv );
  
};
#endif
