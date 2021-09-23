#ifndef GmDataFinalCSDARange_hh
#define GmDataFinalCSDARange_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class G4EmCalculator;

class GmDataFinalCSDARange : public GmVData
{
public:
  GmDataFinalCSDARange();
  ~GmDataFinalCSDARange();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

private:
  G4EmCalculator* theG4EmCalculator;
};
#endif
