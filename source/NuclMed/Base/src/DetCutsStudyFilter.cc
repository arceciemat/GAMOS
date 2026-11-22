#include "DetCutsStudyFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"

DetCutsStudyFilter::DetCutsStudyFilter(const G4String& name) : GmVFilter(name)
{ 
}

G4bool DetCutsStudyFilter::AcceptStep(const G4Step* aStep) 
{
  if( aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSensitiveDetector() != 0 
      && aStep->GetTotalEnergyDeposit() != 0 ) {
    return true; 
  } else {
    return false;
  }
}
