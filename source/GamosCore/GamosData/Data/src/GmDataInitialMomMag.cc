#include "GmDataInitialMomMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialMomMag::GmDataInitialMomMag()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMomMag::~GmDataInitialMomMag()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMomMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentum().mag();
}

//----------------------------------------------------------------
G4double GmDataInitialMomMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() );
}

//----------------------------------------------------------------
G4double GmDataInitialMomMag::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentum().mag();
}

//----------------------------------------------------------------
G4double GmDataInitialMomMag::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().mag();
}

//----------------------------------------------------------------
G4double GmDataInitialMomMag::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
 
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetKineticEnergy() )
	   * aTrack->GetMomentumDirection() ).mag();
}

