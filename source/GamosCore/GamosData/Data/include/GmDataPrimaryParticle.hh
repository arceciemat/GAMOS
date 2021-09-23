#ifndef GmDataPrimaryParticle_hh
#define GmDataPrimaryParticle_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataPrimaryParticle : public GmVDataString
{
public:
  GmDataPrimaryParticle();
  ~GmDataPrimaryParticle();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
