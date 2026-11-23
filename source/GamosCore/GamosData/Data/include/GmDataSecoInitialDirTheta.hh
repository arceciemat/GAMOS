#ifndef GmDataSecoInitialDirTheta_hh
#define GmDataSecoInitialDirTheta_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataSecoInitialDirTheta : public GmVData
{
public:
  GmDataSecoInitialDirTheta();
  ~GmDataSecoInitialDirTheta();

  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

};
#endif
