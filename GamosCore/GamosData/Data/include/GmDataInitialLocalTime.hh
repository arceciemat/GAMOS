#ifndef GmDataInitialLocalTime_hh
#define GmDataInitialLocalTime_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialLocalTime : public GmVData
{
public:
  GmDataInitialLocalTime();
  ~GmDataInitialLocalTime();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track*, G4int );

};
#endif
