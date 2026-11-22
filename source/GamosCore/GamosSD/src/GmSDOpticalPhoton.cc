#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"

#include "GmSDOpticalPhoton.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmSDOpticalPhoton::GmSDOpticalPhoton(G4String sdtype): GmSDSimple( sdtype )
{
}

//--------------------------------------------------------------------
GmSDOpticalPhoton::~GmSDOpticalPhoton(){}

//--------------------------------------------------------------------
G4bool GmSDOpticalPhoton::IsInterestingStep( G4Step* aStep )
{
  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption" ) {
    return TRUE;
  } else { 
    return FALSE;
  }
}


//----------------------------------------------------------------------
G4double GmSDOpticalPhoton::GetEnergyInSD( G4Step* aStep )
{
  return aStep->GetPostStepPoint()->GetKineticEnergy();
  
}
