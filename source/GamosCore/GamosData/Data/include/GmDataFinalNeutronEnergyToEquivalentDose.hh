#ifndef GmDataFinalNeutronEnergyToEquivalentDose_hh
#define GmDataFinalNeutronEnergyToEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataFinalNeutronEnergyToEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataFinalNeutronEnergyToEquivalentDose();
  ~GmDataFinalNeutronEnergyToEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
