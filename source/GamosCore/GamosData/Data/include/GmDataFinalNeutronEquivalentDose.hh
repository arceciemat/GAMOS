#ifndef GmDataFinalNeutronEquivalentDose_hh
#define GmDataFinalNeutronEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataFinalNeutronEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataFinalNeutronEquivalentDose();
  ~GmDataFinalNeutronEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
