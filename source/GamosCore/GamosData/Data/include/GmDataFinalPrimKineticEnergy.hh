#ifndef GmDataFinalPrimKineticEnergy_hh
#define GmDataFinalPrimKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalPrimKineticEnergy : public GmVData
{
public:
  GmDataFinalPrimKineticEnergy();
  ~GmDataFinalPrimKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
