#include "GmDataInitialMomZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialMomZ::GmDataInitialMomZ()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMomZ::~GmDataInitialMomZ()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMomZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentum().z();
}

//----------------------------------------------------------------
G4double GmDataInitialMomZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return ( GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() ) 
	   * aTrack->GetVertexMomentumDirection() ).z();
}

//----------------------------------------------------------------
G4double GmDataInitialMomZ::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentum().z();
}

//----------------------------------------------------------------
G4double GmDataInitialMomZ::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().z();
}
