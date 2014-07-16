#ifndef GmDataFinalMomTheta_hh
#define GmDataFinalMomTheta_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalMomTheta : public GmVData
{
public:
  GmDataFinalMomTheta();
  ~GmDataFinalMomTheta();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
