#include "GmDataDNAInitialVolumeType.hh"
#include "GmDNAGeometry.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"

//----------------------------------------------------------------
GmDataDNAInitialVolumeType::GmDataDNAInitialVolumeType()
{
  bInitial = false;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTTrack);
  theDetector = 0;}

//----------------------------------------------------------------
GmDataDNAInitialVolumeType::~GmDataDNAInitialVolumeType()
{
}

//----------------------------------------------------------------
G4double GmDataDNAInitialVolumeType::GetValueFromStep( const G4Step* aStep, G4int )
{ 
  return SetupVolumeType( aStep->GetPreStepPoint()->GetPhysicalVolume() );
}

//----------------------------------------------------------------
GmDNAFabricVolumeType GmDataDNAInitialVolumeType::SetupVolumeType(const G4VPhysicalVolume* 
                                                    pPhyVolume)
{   
    if( theDetector == 0 ) {
      const G4VUserDetectorConstruction* det = G4RunManager::GetRunManager()->GetUserDetectorConstruction();
      theDetector = dynamic_cast<GmDNAGeometry*>(const_cast<G4VUserDetectorConstruction*>(det));
    }
    auto it = (theDetector->GetGeoDataMap()).find(pPhyVolume); 
    if(it  == (theDetector->GetGeoDataMap()).end())
    {
        G4ExceptionDescription exceptionDescription;
        exceptionDescription <<pPhyVolume->GetName()
                             <<" is wrong physical volume";
        G4Exception("SteppingAction::SetupVolumeFlag()",
                    "SteppingAction01", FatalException,
                    exceptionDescription);
    }
    
    return it->second;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
