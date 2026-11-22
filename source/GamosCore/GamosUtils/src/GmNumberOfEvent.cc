#include "GmNumberOfEvent.hh"
#include "GmMTMergeMgr.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

G4ThreadLocal G4double GmNumberOfEvent::theNEvent = -1;
G4ThreadLocal G4double GmNumberOfEvent::theNEventFactor = 1.;
G4ThreadLocal G4double GmNumberOfEvent::theNEventMerged = 0.;
G4double GmNumberOfEvent::theMasterNEvent = -1;

G4double GmNumberOfEvent::GetNumberOfEvent()
{
  //--- Multithreading master: events counted by the worker generators (if they count them)
  if( GmMTMergeMgr::IsMTMaster() && theMasterNEvent != -1 ) {
    return theMasterNEvent;
  }
  if( theNEvent == -1 ) {
    return G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
  } else {
    return theNEvent * theNEventFactor;
  }

}


void GmNumberOfEvent::SetNumberOfEvent( G4double nev )
{
  theNEvent = nev;
  if( GmMTMergeMgr::IsWorkerThread() ) {
    GmMTMergeMgr::RegisterMerger("GmNumberOfEvent", [](){ GmNumberOfEvent::MergeToMaster(); } );
  }
}


void GmNumberOfEvent::SetNumberOfEventFactor(G4double nevfac)
{
  theNEventFactor = nevfac;
}

G4double GmNumberOfEvent::GetNumberOfEventFactor()
{
  return theNEventFactor; 
}

void GmNumberOfEvent::AddEvent()
{
  theNEvent++;
}

void GmNumberOfEvent::MergeToMaster()
{
  if( theNEvent == -1 ) return; // this worker does not count events
  G4double nevW = theNEvent * theNEventFactor;
  if( theMasterNEvent == -1 ) theMasterNEvent = 0.;
  theMasterNEvent += nevW - theNEventMerged;
  theNEventMerged = nevW;
}
