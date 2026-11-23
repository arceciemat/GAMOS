#include "GmDataVertexPosY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexPosY::GmDataVertexPosY()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexPosY::~GmDataVertexPosY()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPosY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexPosition().y();
}

//----------------------------------------------------------------
G4double GmDataVertexPosY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().y();
}

