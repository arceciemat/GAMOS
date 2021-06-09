#ifndef GmDataInitialPrimSecoAngle_hh
#define GmDataInitialPrimSecoAngle_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialPrimSecoAngle : public GmVData
{
public:
  GmDataInitialPrimSecoAngle();
  ~GmDataInitialPrimSecoAngle();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
