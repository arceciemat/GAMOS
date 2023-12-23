#pragma once
#include "GamosCore/GamosGeometry/include/GmGeometryFromText.hh"
#include "GmDNAFabricParser.hh"
#include <memory>

class G4MoleculeGun;
class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class GmDNAGeometry 
    : public GmGeometryFromText
{
public:
    GmDNAGeometry();
    ~GmDNAGeometry() override;
    G4VPhysicalVolume* Construct() override;
    G4MoleculeGun* GetGun() const
    {
        return fpGun.get();
    }
    
    const GmDNAFabricParser::GeoData& GetGeoDataMap() const
    {
        return fGeometryMap;
    }
private:
    std::unique_ptr<G4MoleculeGun> fpGun;
    std::unique_ptr<GmDNAFabricParser> fpGmDNAFabricParser;
    GmDNAFabricParser::GeoData fGeometryMap;
};
