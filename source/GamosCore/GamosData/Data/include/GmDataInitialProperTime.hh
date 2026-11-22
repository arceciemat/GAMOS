#ifndef GmDataInitialProperTime_hh
#define GmDataInitialProperTime_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialProperTime : public GmVData
{
public:
  GmDataInitialProperTime();
  ~GmDataInitialProperTime();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track*, G4int );

};
#endif
