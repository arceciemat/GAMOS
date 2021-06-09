#ifndef GmDataFinalGammaEquivalentDose_hh
#define GmDataFinalGammaEquivalentDose_hh

#include "GmVDataEquivalentDose.hh"

class GmDataFinalGammaEquivalentDose : public GmVDataEquivalentDose
{
public:
  GmDataFinalGammaEquivalentDose();
  ~GmDataFinalGammaEquivalentDose();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
 
#endif
