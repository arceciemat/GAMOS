#include "GmDataFinalProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
 
//----------------------------------------------------------------
GmDataFinalProcess::GmDataFinalProcess()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalProcess::~GmDataFinalProcess()
{
}

 //----------------------------------------------------------------
G4String GmDataFinalProcess::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

}

//----------------------------------------------------------------
G4String GmDataFinalProcess::GetStringValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* )
{
  return GetStringValueFromStep( aTrack1->GetStep() );
}

