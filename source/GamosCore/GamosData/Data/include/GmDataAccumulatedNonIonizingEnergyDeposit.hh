#ifndef GmDataAccumulatedNonIonizingEnergyDeposit_hh
#define GmDataAccumulatedNonIonizingEnergyDeposit_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedNonIonizingEnergyDeposit : public GmVDataAccumulated
{
public:
  GmDataAccumulatedNonIonizingEnergyDeposit();
  ~GmDataAccumulatedNonIonizingEnergyDeposit();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
