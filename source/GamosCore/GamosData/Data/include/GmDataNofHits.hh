#ifndef GmDataNofHits_hh
#define GmDataNofHits_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataNofHits : public GmVData
{
public:
  GmDataNofHits();
  ~GmDataNofHits();

  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* aEvent, G4int index = 0);

private:
  G4int GetNofHits();
  G4int GetNofHitsThisTrack(G4int theTrackID );

};
#endif
