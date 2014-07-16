#ifndef GmDataInitialMomPerp_hh
#define GmDataInitialMomPerp_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialMomPerp : public GmVData
{
public:
  GmDataInitialMomPerp();
  ~GmDataInitialMomPerp();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* aEvent, G4int index = 0);

};
#endif
