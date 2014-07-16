#ifndef GmDataAccumulatedEnergyLost_hh
#define GmDataAccumulatedEnergyLost_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedEnergyLost : public GmVDataAccumulated
{
public:
  GmDataAccumulatedEnergyLost();
  ~GmDataAccumulatedEnergyLost();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
