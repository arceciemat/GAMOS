#ifndef GmDataAccumulatedKerma_hh
#define GmDataAccumulatedKerma_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"
class GmGeometryUtils;

class GmDataAccumulatedKerma : public GmVDataAccumulated
{
public:
  GmDataAccumulatedKerma();
  ~GmDataAccumulatedKerma();
  
  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

private:
  GmGeometryUtils* theGeomUtils;
};
#endif
