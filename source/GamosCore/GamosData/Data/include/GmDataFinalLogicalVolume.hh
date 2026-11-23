#ifndef GmDataFinalLogicalVolume_hh
#define GmDataFinalLogicalVolume_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalLogicalVolume : public GmVDataString
{
public:
  GmDataFinalLogicalVolume();
  ~GmDataFinalLogicalVolume();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
