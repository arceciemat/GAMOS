#ifndef GmDataFinalDirTheta_hh
#define GmDataFinalDirTheta_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalDirTheta : public GmVData
{
public:
  GmDataFinalDirTheta();
  ~GmDataFinalDirTheta();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
