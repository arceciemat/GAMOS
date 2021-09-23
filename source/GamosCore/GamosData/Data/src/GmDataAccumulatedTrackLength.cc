#include "GmDataAccumulatedTrackLength.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataAccumulatedTrackLength::GmDataAccumulatedTrackLength()
{
  //  G4cout << "DATA GmDataAccumulatedTrackLength::GmDataAccumulatedTrackLength " << this << G4endl; //GDEB
  bInitial = false;
  theHMax = 10.;

  theExcludedTypes.clear(); // delete DTRun
  theEventManager = G4EventManager::GetEventManager();
}

//----------------------------------------------------------------
GmDataAccumulatedTrackLength::~GmDataAccumulatedTrackLength()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedTrackLength::Accumulate( const G4Step* aStep, G4int index )
{
  G4int eventID = theEventManager->GetConstCurrentEvent()->GetEventID();
  G4int trackID = aStep->GetTrack()->GetTrackID();
  if( eventID != theLastEventID || trackID != theLastTrackID ) Initialise(); // initialise theDataAccumulated

  theDataAccumulated[index] += aStep->GetStepLength();

  theLastEventID = eventID;
  theLastTrackID = trackID;
  //  G4cout << this << " GmDataAccumulatedTrackLength::Accumulate " << theDataAccumulated[index] << G4endl; //GDEB
}

//----------------------------------------------------------------
G4double GmDataAccumulatedTrackLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << this << " GmDataAccumulatedTrackLength::GetValueFromStep "  << G4endl; //GDEB
  Accumulate( aStep );
  return GetValueFromTrack( aStep->GetTrack() );
}

//----------------------------------------------------------------
G4double GmDataAccumulatedTrackLength::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* , G4int )
{ 
  return GetValueFromTrack( aTrack1 );
}
