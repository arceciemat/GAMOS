#ifndef GmDataPrimParticle_hh
#define GmDataPrimParticle_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataPrimParticle : public GmVDataString
{
public:
  GmDataPrimParticle();
  ~GmDataPrimParticle();

  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
