#ifndef GmDataInitialGammaAttLength_hh
#define GmDataInitialGammaAttLength_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GmVDataInitialMaterial.hh"
class G4EmCalculator;
class G4Material;
class G4VPhysicalVolume;

class GmDataInitialGammaAttLength : public GmVData, public GmVDataInitialMaterial
{
public:
  GmDataInitialGammaAttLength();
  ~GmDataInitialGammaAttLength();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );
  virtual G4double GetValueFromStackedTrack( const G4Track* aTrack, G4int index = 0 );

  G4EmCalculator* theEmCalculator;

};
#endif