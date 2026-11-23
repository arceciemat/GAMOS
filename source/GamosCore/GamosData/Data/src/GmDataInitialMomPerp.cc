#include "GmDataInitialMomPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialMomPerp::GmDataInitialMomPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMomPerp::~GmDataInitialMomPerp()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMomPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentum().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialMomPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).perp();
}

//----------------------------------------------------------------
G4double GmDataInitialMomPerp::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentum().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialMomPerp::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().perp();
}


//----------------------------------------------------------------
G4double GmDataInitialMomPerp::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
 
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetKineticEnergy() )
	   * aTrack->GetMomentumDirection() ).perp();
}

