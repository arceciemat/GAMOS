#include "GmDataVertexPosPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexPosPerp::GmDataVertexPosPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexPosPerp::~GmDataVertexPosPerp()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPosPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexPosition().perp();
}

//----------------------------------------------------------------
G4double GmDataVertexPosPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().perp();
}

