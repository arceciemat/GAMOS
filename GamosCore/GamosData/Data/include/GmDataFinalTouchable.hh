#ifndef GmDataFinalTouchable_hh
#define GmDataFinalTouchable_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"
class G4VTouchable;

class GmDataFinalTouchable : public GmVDataString
{
public:
  GmDataFinalTouchable();
  ~GmDataFinalTouchable();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

private:
  G4String GetTouchableLongName( const G4VTouchable* touch );

};
#endif
