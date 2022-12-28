#ifndef GmDataVertexLocalPos1X_hh
#define GmDataVertexLocalPos1X_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataVertexLocalPos1X : public GmVData
{
public:
  GmDataVertexLocalPos1X();
  ~GmDataVertexLocalPos1X();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromStackedTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
