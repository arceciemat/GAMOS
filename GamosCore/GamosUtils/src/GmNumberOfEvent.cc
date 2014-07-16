#include "GmNumberOfEvent.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

G4double GmNumberOfEvent::theNEvent = -1;
G4double GmNumberOfEvent::theNEventFactor = 1.;

//----------------------------------------------------------------------
G4double GmNumberOfEvent::GetNumberOfEvent()
{
  if( theNEvent == -1 ) {
    //    G4cout << " GmNumberOfEvent::GetNumberOfEvent() from run " <<  G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent() << G4endl;
    return G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
  } else {
    //     G4cout << " GmNumberOfEvent::GetNumberOfEvent() " << theNEvent * theNEventFactor << theNEvent << " * " << theNEventFactor << G4endl;
    return theNEvent * theNEventFactor;
  }

}


//----------------------------------------------------------------------
void GmNumberOfEvent::SetNumberOfEvent( G4double nev )
{
  theNEvent = nev;
}


//----------------------------------------------------------------------
void GmNumberOfEvent::SetNumberOfEventFactor(G4double nevfac)
{
  theNEventFactor = nevfac;
}

//----------------------------------------------------------------------
G4double GmNumberOfEvent::GetNumberOfEventFactor()
{
  return theNEventFactor; 
}

//----------------------------------------------------------------------
void GmNumberOfEvent::AddEvent()
{
  theNEvent++;
}
