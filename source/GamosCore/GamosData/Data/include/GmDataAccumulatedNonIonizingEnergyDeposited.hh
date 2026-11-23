#ifndef GmDataAccumulatedNonIonizingEnergyDeposited_hh
#define GmDataAccumulatedNonIonizingEnergyDeposited_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedNonIonizingEnergyDeposited : public GmVDataAccumulated
{
public:
  GmDataAccumulatedNonIonizingEnergyDeposited();
  ~GmDataAccumulatedNonIonizingEnergyDeposited();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
