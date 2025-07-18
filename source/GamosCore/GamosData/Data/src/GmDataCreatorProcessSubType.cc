#include "GmDataCreatorProcessSubType.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//----------------------------------------------------------------
GmDataCreatorProcessSubType::GmDataCreatorProcessSubType()
{
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataCreatorProcessSubType::~GmDataCreatorProcessSubType()
{
}

 //----------------------------------------------------------------
G4String GmDataCreatorProcessSubType::GetStringValueFromStep( const G4Step* aStep )
{ 
  const G4VProcess* proc = aStep->GetTrack()->GetCreatorProcess();
  if( proc ) {
    return proc->GetProcessName();
  } else { 
    return "UNDEFINED";
  }

}

//----------------------------------------------------------------
G4String GmDataCreatorProcessSubType::GetStringValueFromTrack( const G4Track* aTrack )
{
  const G4VProcess* proc = aTrack->GetCreatorProcess();
  if( proc ) {
    return proc->GetProcessName();
  } else { 
    return "UNDEFINED";
  }
}

//----------------------------------------------------------------
G4String GmDataCreatorProcessSubType::GetStringValueFromSecoTrack( const G4Track* , const G4Track* aTrack2 )
{
  return aTrack2->GetCreatorProcess()->GetProcessName();
}
