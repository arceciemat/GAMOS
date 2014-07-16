#include "GmDataRunID.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

//----------------------------------------------------------------
GmDataRunID::GmDataRunID()
{
  theExcludedTypes.clear(); // delete DTRun
}

//----------------------------------------------------------------
GmDataRunID::~GmDataRunID()
{
}


//----------------------------------------------------------------
G4double GmDataRunID::GetValueFromStep( const G4Step*, G4int )
{

  return G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
}

//----------------------------------------------------------------
G4double GmDataRunID::GetValueFromTrack( const G4Track*, G4int )
{

  return G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
}

//----------------------------------------------------------------
G4double GmDataRunID::GetValueFromSecoTrack(const G4Track* , const G4Track*, G4int )
{

  return G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
}

//----------------------------------------------------------------
G4double GmDataRunID::GetValueFromEvent(const G4Event*, G4int )
{

  return G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
}

//----------------------------------------------------------------
G4double GmDataRunID::GetValueFromRun(const G4Run*, G4int )
{

  return G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
}
