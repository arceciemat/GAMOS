#ifndef GmDataInitialWeight_hh
#define GmDataInitialWeight_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialWeight : public GmVData
{
public:
  GmDataInitialWeight();
  ~GmDataInitialWeight();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int );
  virtual G4double GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int );


};
#endif
