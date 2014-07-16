#include "GmDataVertexPosPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexPosPhi::GmDataVertexPosPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexPosPhi::~GmDataVertexPosPhi()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPosPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataInitalPosPhi::GetValueFromStep " << aStep->GetPreStepPoint()->GetPosition().phi()/CLHEP::deg << " " << aStep->GetPreStepPoint()->GetPosition() << G4endl;
  return  aStep->GetTrack()->GetVertexPosition().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataVertexPosPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().phi()/CLHEP::deg;
}
