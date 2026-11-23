#ifndef GmDataFinalPosY_hh
#define GmDataFinalPosY_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalPosY : public GmVData
{
public:
  GmDataFinalPosY();
  ~GmDataFinalPosY();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
