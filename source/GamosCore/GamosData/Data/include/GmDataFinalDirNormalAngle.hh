#ifndef GmDataFinalDirNormalAngle_hh
#define GmDataFinalDirNormalAngle_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalDirNormalAngle : public GmVData
{
public:
  GmDataFinalDirNormalAngle();
  ~GmDataFinalDirNormalAngle();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
