#ifndef GmDataParticleSubType_hh
#define GmDataParticleSubType_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataParticleSubType : public GmVDataString
{
public:
  GmDataParticleSubType();
  ~GmDataParticleSubType();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
