#ifndef GmDataFinalPosPerp_hh
#define GmDataFinalPosPerp_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalPosPerp : public GmVData
{
public:
  GmDataFinalPosPerp();
  ~GmDataFinalPosPerp();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
