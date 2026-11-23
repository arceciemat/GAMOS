#ifndef GmVDataAccumulated_hh
#define GmVDataAccumulated_hh

#include "GmVData.hh"

class GmVDataAccumulated : public GmVData
{
public:
  GmVDataAccumulated();
  ~GmVDataAccumulated();

  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* , const G4Track* , G4int index);
  virtual G4double GetValueFromEvent( const G4Event*, G4int index = 0);
  virtual G4double GetValueFromRun( const G4Run*, G4int index = 0);

  virtual void Initialise();

protected:

};
#endif
