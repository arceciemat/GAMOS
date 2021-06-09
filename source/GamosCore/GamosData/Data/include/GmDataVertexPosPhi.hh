#ifndef GmDataVertexPosPhi_hh
#define GmDataVertexPosPhi_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataVertexPosPhi : public GmVData
{
public:
  GmDataVertexPosPhi();
  ~GmDataVertexPosPhi();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
