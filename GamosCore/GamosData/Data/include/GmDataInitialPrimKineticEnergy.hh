#ifndef GmDataInitialPrimKineticEnergy_hh
#define GmDataInitialPrimKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialPrimKineticEnergy : public GmVData
{
public:
  GmDataInitialPrimKineticEnergy();
  ~GmDataInitialPrimKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
