#ifndef GmDataDNAPFlagProcess_hh
#define GmDataDNAPFlagProcess_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataDNAPFlagProcess : public GmVData
{
public:
  GmDataDNAPFlagProcess();
  ~GmDataDNAPFlagProcess();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
};
#endif
