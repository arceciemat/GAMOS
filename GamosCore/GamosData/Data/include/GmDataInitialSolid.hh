#ifndef GmDataInitialSolid_hh
#define GmDataInitialSolid_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataInitialSolid : public GmVDataString
{
public:
  GmDataInitialSolid();
  ~GmDataInitialSolid();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );
  virtual G4String GetStringValueFromEvent( const G4Event* aEvent );

};
#endif
