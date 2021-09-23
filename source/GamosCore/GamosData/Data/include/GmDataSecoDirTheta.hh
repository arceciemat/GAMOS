#ifndef GmDataSecoDirTheta_hh
#define GmDataSecoDirTheta_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataSecoDirTheta : public GmVData
{
public:
  GmDataSecoDirTheta();
  ~GmDataSecoDirTheta();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
