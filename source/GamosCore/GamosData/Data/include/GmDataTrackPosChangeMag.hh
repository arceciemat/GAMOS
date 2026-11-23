#ifndef GmDataTrackPosChangeMag_hh
#define GmDataTrackPosChangeMag_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataTrackPosChangeMag : public GmVData
{
public:
  GmDataTrackPosChangeMag();
  ~GmDataTrackPosChangeMag();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
