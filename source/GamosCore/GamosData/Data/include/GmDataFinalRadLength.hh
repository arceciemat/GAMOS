#ifndef GmDataFinalRadLength_hh
#define GmDataFinalRadLength_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataFinalRadLength : public GmVData
{
public:
  GmDataFinalRadLength();
  ~GmDataFinalRadLength();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
