#ifndef GmDataParticleWidth_hh
#define GmDataParticleWidth_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataParticleWidth : public GmVData
{
public:
  GmDataParticleWidth();
  ~GmDataParticleWidth();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int index = 0 );

};
#endif
