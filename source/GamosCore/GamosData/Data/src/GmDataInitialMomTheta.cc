#include "GmDataInitialMomTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialMomTheta::GmDataInitialMomTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMomTheta::~GmDataInitialMomTheta()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMomTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentum().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialMomTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialMomTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentum().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialMomTheta::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().theta();
}

//----------------------------------------------------------------
G4double GmDataInitialMomTheta::GetValueFromStackedTrack( const G4Track* aTrack, G4int)
{
 
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetKineticEnergy() )
	   * aTrack->GetMomentumDirection() ).theta();
}

