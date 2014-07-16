#include "GmTimeStudyMgr.hh"
#include "GmTimeStudyUA.hh"
#include "G4SteppingVerbose.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

GmTimeStudyMgr* GmTimeStudyMgr::theInstance = 0;
G4SteppingVerbose* GmTimeStudyMgr::theG4Verbose = 0;
int GmTimeStudyMgr::theG4VerboseLevel = 0;

//----------------------------------------------------------------------
GmTimeStudyMgr::GmTimeStudyMgr()
{
  G4VSteppingVerbose::SetInstance(this);
}

//----------------------------------------------------------------------
GmTimeStudyMgr* GmTimeStudyMgr::GetInstance()
{
  if( !theInstance ) {
    theG4VerboseLevel = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmTimeStudyUA:G4VerboseLevel",0));
    G4VSteppingVerbose::SetInstance( (G4VSteppingVerbose*)0 );
    if( theG4VerboseLevel > 0 ) theG4Verbose =  new G4SteppingVerbose();
    G4VSteppingVerbose::SetInstance( (G4VSteppingVerbose*)0 );
    theInstance = new GmTimeStudyMgr;
  }

  return theInstance;

}

//----------------------------------------------------------------------
void GmTimeStudyMgr::InitManagers()
{
  G4TrackingManager* trackingManager = G4EventManager::GetEventManager()->GetTrackingManager();
  G4SteppingManager* steppingManager = trackingManager->GetSteppingManager();
  steppingManager->SetVerbose( GmTimeStudyMgr::GetInstance() );

  if( theG4VerboseLevel > 0 ) {
    theG4Verbose->SetManager(steppingManager);
    steppingManager->SetVerboseLevel(theG4VerboseLevel);
    trackingManager->SetVerboseLevel(theG4VerboseLevel);
    G4VSteppingVerbose::SetSilent(0);
  } else {
    G4VSteppingVerbose::SetSilent(1);
    steppingManager->SetVerboseLevel(1);
  }
  SetManager(steppingManager);

}

//----------------------------------------------------------------------
void GmTimeStudyMgr::AddTimeStudy( GmTimeStudyUA* ts )
{
  theTimeStudies.push_back( ts );
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::NewStep()
{
  CopyState();
  if( fTrack->GetCurrentStepNumber() == 0 ) return;
  std::vector<GmTimeStudyUA*>::const_iterator ite;
  for( ite = theTimeStudies.begin(); ite != theTimeStudies.end(); ite++ ){
    (*ite)->StartTimer( fTrack->GetStep() );
  }

  if( theG4VerboseLevel > 0 ) theG4Verbose->NewStep();
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::StepInfo()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->StepInfo();
}
 

//----------------------------------------------------------------------
GmTimeStudyMgr::~GmTimeStudyMgr()
{
}


//----------------------------------------------------------------------
void GmTimeStudyMgr::TrackingStarted()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->TrackingStarted();
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::VerboseTrack()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->VerboseTrack();
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::AtRestDoItInvoked() {
  if( theG4VerboseLevel > 0 ) theG4Verbose->AtRestDoItInvoked(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::AlongStepDoItAllDone() 
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->AlongStepDoItAllDone(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::PostStepDoItAllDone() 
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->PostStepDoItAllDone(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::AlongStepDoItOneByOne() 
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->AlongStepDoItOneByOne(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::PostStepDoItOneByOne()
{ 
  if( theG4VerboseLevel > 0 ) theG4Verbose->PostStepDoItOneByOne(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::DPSLStarted()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->DPSLStarted(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::DPSLUserLimit()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->DPSLUserLimit(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::DPSLPostStep()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->DPSLPostStep(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::DPSLAlongStep()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->DPSLAlongStep(); 
}

//----------------------------------------------------------------------
void GmTimeStudyMgr::VerboseParticleChange()
{
  if( theG4VerboseLevel > 0 ) theG4Verbose->VerboseParticleChange(); 
}
