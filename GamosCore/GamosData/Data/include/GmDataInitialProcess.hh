#ifndef GmDataInitialProcess_hh
#define GmDataInitialProcess_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmDataInitialProcess : public GmVDataString
{
public:
  GmDataInitialProcess();
  ~GmDataInitialProcess();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );

};
#endif
