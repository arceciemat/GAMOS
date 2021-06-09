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
  theExcludedTypes.insert(DTSeco);
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
