#include "GmDataVertexPosTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexPosTheta::GmDataVertexPosTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexPosTheta::~GmDataVertexPosTheta()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPosTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexPosition().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataVertexPosTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().theta()/CLHEP::deg;
}
