#ifndef GmDataFinalMaterial_hh
#define GmDataFinalMaterial_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalMaterial : public GmVDataString
{
public:
  GmDataFinalMaterial();
  ~GmDataFinalMaterial();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
