#ifndef GmDataTrackLength_hh
#define GmDataTrackLength_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataTrackLength : public GmVData
{
public:
  GmDataTrackLength();
  ~GmDataTrackLength();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int );

};
#endif
