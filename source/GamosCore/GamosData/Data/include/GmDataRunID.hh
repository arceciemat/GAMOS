#ifndef GmDataRunID_hh
#define GmDataRunID_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"

class GmDataRunID : public GmVDataInt
{
public:
  GmDataRunID();
  ~GmDataRunID();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int index = 0 );
  virtual G4double GetValueFromEvent(const G4Event*, G4int );
  virtual G4double GetValueFromRun(const G4Run*, G4int );

};
#endif
