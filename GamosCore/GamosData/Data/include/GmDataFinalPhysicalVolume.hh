#ifndef GmDataFinalPhysicalVolume_hh
#define GmDataFinalPhysicalVolume_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalPhysicalVolume : public GmVDataString
{
public:
  GmDataFinalPhysicalVolume();
  ~GmDataFinalPhysicalVolume();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
