#include "GmDataVertexPosMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexPosMag::GmDataVertexPosMag()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexPosMag::~GmDataVertexPosMag()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPosMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexPosition().mag();
}

//----------------------------------------------------------------
G4double GmDataVertexPosMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().mag();
}
