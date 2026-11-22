#include "GmMinRangeLimitsStudyCreateInfoUA.hh"
#include "GmMinRangeLimitsStudyUA.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

//----------------------------------------------------------------
GmMinRangeLimitsStudyCreateInfoUA::GmMinRangeLimitsStudyCreateInfoUA(GmMinRangeLimitsStudyUA* mr) : theMinRangeUA( mr )
{
  theName = "GmMinRangeLimitsStudyCreateInfoUA";
  bInfoCreated = false;
  
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyCreateInfoUA::UserSteppingAction(const G4Step* aStep )
{
  //check at least one seco is created
  //--- create for all, because when an step is accepted, it has to accept all previous steps  if( fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt() != 0 ){ 
    //    if( !GmCutsStudyMgr::GetInstance()->GetTrackStepInfo( aStep->GetTrack()->GetParentID(), aStep->GetTrack()->GetPosition() )) { 
    if( !bInfoCreated ) {
      //      G4cout << " GmMinRangeLimitsStudyCreateInfoUA " << G4endl;
      theMinRangeUA->CreateTrackStepInfo( aStep->GetTrack() );
    }
    //  }
  bInfoCreated = false;

}
