#ifndef GmDataVertexPrimKineticEnergy_hh
#define GmDataVertexPrimKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataVertexPrimKineticEnergy : public GmVData
{
public:
  GmDataVertexPrimKineticEnergy();
  ~GmDataVertexPrimKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
