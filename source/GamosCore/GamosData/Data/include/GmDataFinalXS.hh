#ifndef GmDataFinalXS_hh
#define GmDataFinalXS_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalXS : public GmVDataString
{
public:
  GmDataFinalXS();
  ~GmDataFinalXS();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
