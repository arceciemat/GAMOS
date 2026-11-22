#ifndef GmDataFinalGammaEnergyToEquivalentDose_hh
#define GmDataFinalGammaEnergyToEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataFinalGammaEnergyToEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataFinalGammaEnergyToEquivalentDose();
  ~GmDataFinalGammaEnergyToEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
