#ifndef GmDataTimeChange_hh
#define GmDataTimeChange_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataTimeChange : public GmVData
{
public:
  GmDataTimeChange();
  ~GmDataTimeChange();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
