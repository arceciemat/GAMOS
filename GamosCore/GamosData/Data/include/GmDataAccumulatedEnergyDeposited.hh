#ifndef GmDataAccumulatedEnergyDeposited_hh
#define GmDataAccumulatedEnergyDeposited_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedEnergyDeposited : public GmVDataAccumulated
{
public:
  GmDataAccumulatedEnergyDeposited();
  ~GmDataAccumulatedEnergyDeposited();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
