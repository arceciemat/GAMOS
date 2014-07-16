#ifndef GmDataParticleType_hh
#define GmDataParticleType_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataParticleType : public GmVDataString
{
public:
  GmDataParticleType();
  ~GmDataParticleType();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
