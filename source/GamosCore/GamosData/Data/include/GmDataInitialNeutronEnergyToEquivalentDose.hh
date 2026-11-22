#ifndef GmDataInitialNeutronEnergyToEquivalentDose_hh
#define GmDataInitialNeutronEnergyToEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataInitialNeutronEnergyToEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataInitialNeutronEnergyToEquivalentDose();
  ~GmDataInitialNeutronEnergyToEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
