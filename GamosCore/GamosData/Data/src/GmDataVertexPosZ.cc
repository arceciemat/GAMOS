#include "GmDataVertexPosZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexPosZ::GmDataVertexPosZ()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexPosZ::~GmDataVertexPosZ()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPosZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexPosition().z();
}

//----------------------------------------------------------------
G4double GmDataVertexPosZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().z();
}

