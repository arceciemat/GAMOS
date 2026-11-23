#ifndef GmDataAccumulatedDose_hh
#define GmDataAccumulatedDose_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"
class GmGeometryUtils;

class GmDataAccumulatedDose : public GmVDataAccumulated
{
public:
  GmDataAccumulatedDose();
  ~GmDataAccumulatedDose();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

private:
  GmGeometryUtils* theGeomUtils;

};
#endif
