#ifndef GmDataEventID_hh
#define GmDataEventID_hh

#include "GamosCore/GamosData/Management/include/GmVDataInt.hh"

class GmDataEventID : public GmVDataInt
{
public:
  GmDataEventID();
  ~GmDataEventID();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event*, G4int );

};
#endif
