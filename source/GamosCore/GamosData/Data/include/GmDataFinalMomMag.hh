#ifndef GmDataFinalMomMag_hh
#define GmDataFinalMomMag_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalMomMag : public GmVData
{
public:
  GmDataFinalMomMag();
  ~GmDataFinalMomMag();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
