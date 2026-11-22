#ifndef GmDataFinalPrimSecoAngle_hh
#define GmDataFinalPrimSecoAngle_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalPrimSecoAngle : public GmVData
{
public:
  GmDataFinalPrimSecoAngle();
  ~GmDataFinalPrimSecoAngle();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
