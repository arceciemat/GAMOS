#include "GmKillTooManyStepsUA.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"


//-----------------------------------------------------------------
GmKillTooManyStepsUA::GmKillTooManyStepsUA()
{
}


//-----------------------------------------------------------------
GmKillTooManyStepsUA::~GmKillTooManyStepsUA() 
{
}

//-----------------------------------------------------------------
void GmKillTooManyStepsUA::BeginOfRunAction(const G4Run* )
{
  theNStepsMax = GmParameterMgr::GetInstance()->GetNumericValue(theName+":NStepsMax",1E4);
  theNStepsWarning = GmParameterMgr::GetInstance()->GetNumericValue(theName+":NStepsWarning",theNStepsMax-10);
}

//-----------------------------------------------------------------
void GmKillTooManyStepsUA::UserSteppingAction(const G4Step* aStep )
{
  if( aStep->GetTrack()->GetCurrentStepNumber() > theNStepsMax ) {
    G4cerr << "!!! GmKillTooManyStepsUA: Aborting Event " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << " at Track " << aStep->GetTrack()->GetTrackID() << " Step " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
    G4RunManager::GetRunManager()->AbortEvent();
  } 

  if( aStep->GetTrack()->GetCurrentStepNumber() > theNStepsWarning ) {
    G4cerr << "!!! GmKillTooManyStepsUA: Too many Steps at Event " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << " at Track " << aStep->GetTrack()->GetTrackID() << " Step " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
  }

}

