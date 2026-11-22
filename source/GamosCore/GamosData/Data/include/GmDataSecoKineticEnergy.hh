#ifndef GmDataSecoKineticEnergy_hh
#define GmDataSecoKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataSecoKineticEnergy : public GmVData
{
public:
  GmDataSecoKineticEnergy();
  ~GmDataSecoKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
