#ifndef GmDataSecoDividedInitialPrimKineticEnergy_hh
#define GmDataSecoDividedInitialPrimKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataSecoDividedInitialPrimKineticEnergy : public GmVData
{
public:
  GmDataSecoDividedInitialPrimKineticEnergy();
  ~GmDataSecoDividedInitialPrimKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
