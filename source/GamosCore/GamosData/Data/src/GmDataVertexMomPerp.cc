#include "GmDataVertexMomPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexMomPerp::GmDataVertexMomPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexMomPerp::~GmDataVertexMomPerp()
{
}


//----------------------------------------------------------------
G4double GmDataVertexMomPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  const G4Track* aTrack = aStep->GetTrack();
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).perp();
}

//----------------------------------------------------------------
G4double GmDataVertexMomPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).perp();
}

