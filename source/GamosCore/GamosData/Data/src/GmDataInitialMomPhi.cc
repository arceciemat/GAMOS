#include "GmDataInitialMomPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialMomPhi::GmDataInitialMomPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMomPhi::~GmDataInitialMomPhi()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMomPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentum().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialMomPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialMomPhi::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentum().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialMomPhi::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().phi();
}

//----------------------------------------------------------------
G4double GmDataInitialMomPhi::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
 
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetKineticEnergy() )
	   * aTrack->GetMomentumDirection() ).phi();
}

