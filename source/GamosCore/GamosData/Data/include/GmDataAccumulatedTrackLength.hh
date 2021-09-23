#ifndef GmDataAccumulatedTrackLength_hh
#define GmDataAccumulatedTrackLength_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"
class G4EventManager;

class GmDataAccumulatedTrackLength : public GmVDataAccumulated
{
public:
  GmDataAccumulatedTrackLength();
  ~GmDataAccumulatedTrackLength();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );

private:
  G4int theLastEventID;
  G4int theLastTrackID;
  G4EventManager* theEventManager;
};
#endif
