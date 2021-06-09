#ifndef GmSDOpticalPhoton_HH
#define GmSDOpticalPhoton_HH

#include "GamosCore/GamosSD/include/GmSDSimple.hh"

class GmSDOpticalPhoton: public GmSDSimple
{

public:
  GmSDOpticalPhoton(G4String type );
  ~GmSDOpticalPhoton();

  G4bool IsInterestingStep( G4Step* aStep );
  virtual G4double GetEnergyInSD( G4Step* aStep );

};

#endif
