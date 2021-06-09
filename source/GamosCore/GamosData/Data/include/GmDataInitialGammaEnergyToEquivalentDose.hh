#ifndef GmDataInitialGammaEnergyToEquivalentDose_hh
#define GmDataInitialGammaEnergyToEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataInitialGammaEnergyToEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataInitialGammaEnergyToEquivalentDose();
  ~GmDataInitialGammaEnergyToEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
