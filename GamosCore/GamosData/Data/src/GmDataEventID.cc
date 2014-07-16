#include "GmDataEventID.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataEventID::GmDataEventID()
{
}

//----------------------------------------------------------------
GmDataEventID::~GmDataEventID()
{
}


//----------------------------------------------------------------
G4double GmDataEventID::GetValueFromStep( const G4Step*, G4int )
{

  return G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

}

//----------------------------------------------------------------
G4double GmDataEventID::GetValueFromTrack( const G4Track*, G4int )
{

  return G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
}


//----------------------------------------------------------------
G4double GmDataEventID::GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int )
{

  return G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
}

//----------------------------------------------------------------
G4double GmDataEventID::GetValueFromEvent( const G4Event*, G4int )
{
  return G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
}
