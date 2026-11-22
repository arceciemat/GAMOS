#ifndef GmDataVertexMomX_hh
#define GmDataVertexMomX_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataVertexMomX : public GmVData
{
public:
  GmDataVertexMomX();
  ~GmDataVertexMomX();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
};
#endif
