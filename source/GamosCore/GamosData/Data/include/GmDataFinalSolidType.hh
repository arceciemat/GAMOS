#ifndef GmDataFinalSolidType_hh
#define GmDataFinalSolidType_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalSolidType : public GmVDataString
{
public:
  GmDataFinalSolidType();
  ~GmDataFinalSolidType();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

};
#endif
