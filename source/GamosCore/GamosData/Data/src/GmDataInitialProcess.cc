#include "GmDataInitialProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//----------------------------------------------------------------
GmDataInitialProcess::GmDataInitialProcess()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialProcess::~GmDataInitialProcess()
{
}

 //----------------------------------------------------------------
G4String GmDataInitialProcess::GetStringValueFromStep( const G4Step* aStep )
{ 
  const G4VProcess* proc = aStep->GetPreStepPoint()->GetProcessDefinedStep();
  if( proc ) {
      return proc->GetProcessName();
  } else {
    return "UNDEFINED";
  }

}
//----------------------------------------------------------------
G4String GmDataInitialProcess::GetStringValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* )
{
  return GetStringValueFromStep( aTrack1->GetStep() );
}

