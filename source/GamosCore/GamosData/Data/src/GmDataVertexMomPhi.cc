#include "GmDataVertexMomPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexMomPhi::GmDataVertexMomPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexMomPhi::~GmDataVertexMomPhi()
{
}


//----------------------------------------------------------------
G4double GmDataVertexMomPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  const G4Track* aTrack = aStep->GetTrack();
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataVertexMomPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).phi()/CLHEP::deg;
}

