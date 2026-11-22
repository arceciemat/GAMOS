#ifndef GmDataInitialProcess_hh
#define GmDataInitialProcess_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataInitialProcess : public GmVDataString
{
public:
  GmDataInitialProcess();
  ~GmDataInitialProcess();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );

};
#endif
