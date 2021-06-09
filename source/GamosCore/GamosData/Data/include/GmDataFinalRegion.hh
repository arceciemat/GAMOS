#ifndef GmDataFinalRegion_hh
#define GmDataFinalRegion_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalRegion : public GmVDataString
{
public:
  GmDataFinalRegion();
  ~GmDataFinalRegion();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
