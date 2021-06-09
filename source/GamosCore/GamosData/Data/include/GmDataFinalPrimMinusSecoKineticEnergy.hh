#ifndef GmDataFinalPrimMinusSecoKineticEnergy_hh
#define GmDataFinalPrimMinusSecoKineticEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalPrimMinusSecoKineticEnergy : public GmVData
{
public:
  GmDataFinalPrimMinusSecoKineticEnergy();
  ~GmDataFinalPrimMinusSecoKineticEnergy();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
