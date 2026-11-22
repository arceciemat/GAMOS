#ifndef GmDataDirChangePhi_hh
#define GmDataDirChangePhi_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataDirChangePhi : public GmVData
{
public:
  GmDataDirChangePhi();
  ~GmDataDirChangePhi();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
