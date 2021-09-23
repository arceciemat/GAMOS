#include "GmDataInitialMomX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialMomX::GmDataInitialMomX()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMomX::~GmDataInitialMomX()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMomX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentum().x();
}

//----------------------------------------------------------------
G4double GmDataInitialMomX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).x();
}

//----------------------------------------------------------------
G4double GmDataInitialMomX::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentum().x();
}

//----------------------------------------------------------------
G4double GmDataInitialMomX::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().x();
}

//----------------------------------------------------------------
G4double GmDataInitialMomX::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
 
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetKineticEnergy() )
	   * aTrack->GetMomentumDirection() ).x();
}

