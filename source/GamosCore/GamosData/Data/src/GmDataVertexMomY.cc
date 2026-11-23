#include "GmDataVertexMomY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexMomY::GmDataVertexMomY()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexMomY::~GmDataVertexMomY()
{
}


//----------------------------------------------------------------
G4double GmDataVertexMomY::GetValueFromStep( const G4Step* aStep, G4int )
{
  const G4Track* aTrack = aStep->GetTrack();
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).y();
}

//----------------------------------------------------------------
G4double GmDataVertexMomY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).y();
}

