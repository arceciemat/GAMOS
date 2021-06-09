#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"

#include <math.h>

#include "PETPrintPositron.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------
PETPrintPositron::PETPrintPositron()
{
}

//----------------------------------------------------------------
void PETPrintPositron::UserSteppingAction(const G4Step* aStep)
{
  const G4Track* aTrack = aStep->GetTrack();
  ///* for JM
  if( aTrack->GetDefinition()->GetParticleName() == "e+" ) {
    if( aTrack->GetCurrentStepNumber() == 1 ) {
      G4double mass = CLHEP::electron_mass_c2;
      G4double T =  aTrack->GetKineticEnergy();
      G4double velocity = CLHEP::c_light*std::sqrt(T*(T+2.*mass))/(T+mass);
      G4cout << "STEP: 0 " << std::setprecision(6) << 
	aTrack->GetVertexPosition().x() << " " << 
	aTrack->GetVertexPosition().y() << " " << 
	aTrack->GetVertexPosition().z() << " " << 
	aTrack->GetVertexKineticEnergy() << " " << 
	"0" << " " << 
	velocity << " " << G4endl;
    }
    G4cout << "STEP: " << std::setprecision(6) << aTrack->GetCurrentStepNumber() << " " << 
      aTrack->GetPosition().x() << " " << 
      aTrack->GetPosition().y() << " " << 
      aTrack->GetPosition().z() << " " << 
      aTrack->GetKineticEnergy() << " " << 
      aTrack->GetGlobalTime() << " " << 
      aTrack->GetVelocity() << " " << G4endl;
 
  }
 // */


  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "annihil" ) return;

  G4bool bPrintPos = TRUE;
  if( bPrintPos ) G4cout << " e+POS " << aTrack->GetPosition().x() << " " <<aTrack->GetPosition().y() << " " <<aTrack->GetPosition().z() << " VTX " <<
aTrack->GetVertexPosition().x() << " " << aTrack->GetVertexPosition().y() << " " <<aTrack->GetVertexPosition().z() << " " << G4endl;
 
}
