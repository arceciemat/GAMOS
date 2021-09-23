#include "G4Run.hh"
#include "G4Event.hh"

#include "Exer4UserActionA.hh"

#include "G4RunManager.hh"

//-----------------------------------------------------------------------
Exer4UserActionA::Exer4UserActionA() :  GmUserSteppingAction()
{
}


//-----------------------------------------------------------------------
void Exer4UserActionA::UserSteppingAction(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetParentID() == 0 ){

  if( aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Crystal" &&
      aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() != "Crystal" ) {
    G4cout << " Exer4UserActionA::UserSteppingAction  Aborting event " << G4endl;
    G4RunManager::GetRunManager()->AbortEvent();
  }
  }
}


