#ifndef GmDataInitialRegion_hh
#define GmDataInitialRegion_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataInitialRegion : public GmVDataString
{
public:
  GmDataInitialRegion();
  ~GmDataInitialRegion();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );
  virtual G4String GetStringValueFromEvent( const G4Event* aEvent );

};
#endif
