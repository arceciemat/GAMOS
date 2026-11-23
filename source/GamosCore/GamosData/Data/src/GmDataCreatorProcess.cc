#include "GmDataCreatorProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//----------------------------------------------------------------
GmDataCreatorProcess::GmDataCreatorProcess()
{
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataCreatorProcess::~GmDataCreatorProcess()
{
}

 //----------------------------------------------------------------
G4String GmDataCreatorProcess::GetStringValueFromStep( const G4Step* aStep )
{ 
  const G4VProcess* proc = aStep->GetTrack()->GetCreatorProcess();
  if( proc ) {
    return proc->GetProcessName();
  } else { 
    return "UNDEFINED";
  }

}

//----------------------------------------------------------------
G4String GmDataCreatorProcess::GetStringValueFromTrack( const G4Track* aTrack )
{
  const G4VProcess* proc = aTrack->GetCreatorProcess();
  if( proc ) {
    return proc->GetProcessName();
  } else { 
    return "UNDEFINED";
  }
}

//----------------------------------------------------------------
G4String GmDataCreatorProcess::GetStringValueFromSecoTrack( const G4Track* , const G4Track* aTrack2 )
{
  return aTrack2->GetCreatorProcess()->GetProcessName();
}
