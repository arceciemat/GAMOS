#ifndef GmDataFinalLocalPosPhi_hh
#define GmDataFinalLocalPosPhi_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalLocalPosPhi : public GmVData
{
public:
  GmDataFinalLocalPosPhi();
  ~GmDataFinalLocalPosPhi();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
