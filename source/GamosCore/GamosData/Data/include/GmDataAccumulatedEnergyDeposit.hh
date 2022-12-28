#ifndef GmDataAccumulatedEnergyDeposit_hh
#define GmDataAccumulatedEnergyDeposit_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedEnergyDeposit : public GmVDataAccumulated
{
public:
  GmDataAccumulatedEnergyDeposit();
  ~GmDataAccumulatedEnergyDeposit();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
