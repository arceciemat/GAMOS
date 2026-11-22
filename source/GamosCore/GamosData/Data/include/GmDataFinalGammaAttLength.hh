#ifndef GmDataFinalGammaAttLength_hh
#define GmDataFinalGammaAttLength_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"
class G4EmCalculator;

class GmDataFinalGammaAttLength : public GmVData
{
public:
  GmDataFinalGammaAttLength();
  ~GmDataFinalGammaAttLength();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

  G4EmCalculator* theEmCalculator;
};
#endif
