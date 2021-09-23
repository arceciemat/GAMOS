#ifndef GmDataTrackPointer_hh
#define GmDataTrackPointer_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataTrackPointer : public GmVDataString
{
public:
  GmDataTrackPointer();
  ~GmDataTrackPointer();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
