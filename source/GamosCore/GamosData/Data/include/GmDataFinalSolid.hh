#ifndef GmDataFinalSolid_hh
#define GmDataFinalSolid_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalSolid : public GmVDataString
{
public:
  GmDataFinalSolid();
  ~GmDataFinalSolid();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
