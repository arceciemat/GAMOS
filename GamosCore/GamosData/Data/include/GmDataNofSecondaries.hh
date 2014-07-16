#ifndef GmDataNofSecondaries_hh
#define GmDataNofSecondaries_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"

class GmDataNofSecondaries : public GmVDataInt
{
public:
  GmDataNofSecondaries();
  ~GmDataNofSecondaries();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
};
#endif
