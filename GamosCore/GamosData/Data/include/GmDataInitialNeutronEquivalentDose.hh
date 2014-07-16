#ifndef GmDataInitialNeutronEquivalentDose_hh
#define GmDataInitialNeutronEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataInitialNeutronEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataInitialNeutronEquivalentDose();
  ~GmDataInitialNeutronEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
