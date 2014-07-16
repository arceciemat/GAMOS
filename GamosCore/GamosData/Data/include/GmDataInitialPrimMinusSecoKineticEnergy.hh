#ifndef GmDataInitialPrimMinusSecoKineticEnergy_hh
#define GmDataInitialPrimMinusSecoKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialPrimMinusSecoKineticEnergy : public GmVData
{
public:
  GmDataInitialPrimMinusSecoKineticEnergy();
  ~GmDataInitialPrimMinusSecoKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
