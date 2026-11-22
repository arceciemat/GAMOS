#ifndef GmDataAccumulatedNofTracks_hh
#define GmDataAccumulatedNofTracks_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"

class GmDataAccumulatedNofTracks : public GmVDataAccumulated
{
public:
  GmDataAccumulatedNofTracks();
  ~GmDataAccumulatedNofTracks();
  
  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromTrack( const G4Track*, G4int index );
  virtual G4double GetValueFromEvent( const G4Event*, G4int index );
  virtual void Initialise();

private:
  std::map<G4int,std::set<G4int> > theTracks;
  
};
#endif
