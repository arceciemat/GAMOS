#include "GmDataVertexMomTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexMomTheta::GmDataVertexMomTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexMomTheta::~GmDataVertexMomTheta()
{
}


//----------------------------------------------------------------
G4double GmDataVertexMomTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  const G4Track* aTrack = aStep->GetTrack();
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataVertexMomTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).theta()/CLHEP::deg;
}

