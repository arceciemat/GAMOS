#ifndef GmDataStepNumber_hh
#define GmDataStepNumber_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"

class GmDataStepNumber : public GmVDataInt
{
public:
  GmDataStepNumber();
  ~GmDataStepNumber();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
