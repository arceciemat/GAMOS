#ifndef GmDataInitialPVCopyNumber_hh
#define GmDataInitialPVCopyNumber_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"

class GmDataInitialPVCopyNumber : public GmVDataInt
{
public:
  GmDataInitialPVCopyNumber();
  ~GmDataInitialPVCopyNumber();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* aEvent, G4int index = 0);
  virtual G4double GetValueFromStackedTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
