#ifndef GmDataVertexDirY_hh
#define GmDataVertexDirY_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataVertexDirY : public GmVData
{
public:
  GmDataVertexDirY();
  ~GmDataVertexDirY();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
