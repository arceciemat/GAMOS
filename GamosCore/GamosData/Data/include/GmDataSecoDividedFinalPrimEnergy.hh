#ifndef GmDataSecoDividedFinalPrimEnergy_hh
#define GmDataSecoDividedFinalPrimEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataSecoDividedFinalPrimEnergy : public GmVData
{
public:
  GmDataSecoDividedFinalPrimEnergy();
  ~GmDataSecoDividedFinalPrimEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
