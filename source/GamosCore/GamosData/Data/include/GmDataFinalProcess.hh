#ifndef GmDataFinalProcess_hh
#define GmDataFinalProcess_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataFinalProcess : public GmVDataString
{
public:
  GmDataFinalProcess();
  ~GmDataFinalProcess();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
