#ifndef GmDataDNAInitialVolumeType_hh
#define GmDataDNAInitialVolumeType_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"
class GmDNAGeometry;
#include "GmDNAFabricParser.hh"

class GmDataDNAInitialVolumeType : public GmVData
{
public:
  GmDataDNAInitialVolumeType();
  ~GmDataDNAInitialVolumeType();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

private:
  GmDNAFabricVolumeType SetupVolumeType(const G4VPhysicalVolume* );
  GmDNAGeometry* theDetector;
};
#endif
