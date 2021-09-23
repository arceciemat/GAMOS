#ifndef GmDataInitialSolidType_hh
#define GmDataInitialSolidType_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"
#include "GmVDataInitialMaterial.hh"

class GmDataInitialSolidType : public GmVDataString, public GmVDataInitialMaterial
{
public:
  GmDataInitialSolidType();
  ~GmDataInitialSolidType();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );
  virtual G4String GetStringValueFromEvent( const G4Event* aEvent );
  virtual G4String GetStringValueFromStackedTrack( const G4Track* aTrack );
 
};

#endif
