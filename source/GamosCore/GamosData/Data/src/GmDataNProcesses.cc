#include "GmDataNProcesses.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

//----------------------------------------------------------------
GmDataNProcesses::GmDataNProcesses()
{
  bInitial = false;
  theHMax = 10.;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);

  theProcessesNames = GmParameterMgr::GetInstance()->GetVStringValue("GmDataNProcesses:Processes",theProcessesNames);
  if( theProcessesNames.size() == 0 ) {
    G4Exception("GmDataNProcesses::GmDataNProcesses",
		"",
		FatalException,
		"No process name given, please use '/gamos/setParam GmDataNProcesses:Processes PROC_NAME_1 PROC_NAME_2 ...'");
  }

  theEventManager = G4EventManager::GetEventManager();
}

//----------------------------------------------------------------
GmDataNProcesses::~GmDataNProcesses()
{
}

//----------------------------------------------------------------
void GmDataNProcesses::Accumulate( const G4Step* aStep, G4int index )
{
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  for( size_t ii = 0; ii < theProcessesNames.size(); ii++ ) {
    if( GmGenUtils::AreWordsEquivalent(theProcessesNames[ii],procName) ) {
      theDataAccumulated[index] += 1;
    }
#ifndef WIN32
	if( DataVerb(debugVerb) ) G4cout << " GmDataNProcesses::Accumulate " << index << " : " <<  theDataAccumulated[index] << G4endl; 
#endif

  }
}

//----------------------------------------------------------------
G4double GmDataNProcesses::GetValueFromStep( const G4Step* aStep, G4int index )
{
  G4int evtID = theEventManager->GetConstCurrentEvent()->GetEventID();
  // Check track to start new counter
  G4int trkID = aStep->GetTrack()->GetTrackID();
  if( trkID != thePrevTrackID || evtID != thePrevEventID ) theNSteps.clear();
  thePrevTrackID = trkID;
  thePrevEventID = evtID;
  
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  for( size_t ii = 0; ii < theProcessesNames.size(); ii++ ) {
    if( GmGenUtils::AreWordsEquivalent(procName,theProcessesNames[ii]) ) {
      theNSteps[index] += 1;
      break;
    }
  }

#ifndef WIN32
  if( DataVerb(debugVerb) ) G4cout << " GmDataNProcesses::GetValueFromStep " << theNSteps[index] << " " << index << " : " <<  theDataAccumulated[index] << G4endl; 
#endif

  return theNSteps[index];
}

