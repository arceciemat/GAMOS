#ifndef GmDataInitialGammaEquivalentDose_hh
#define GmDataInitialGammaEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataInitialGammaEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataInitialGammaEquivalentDose();
  ~GmDataInitialGammaEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
