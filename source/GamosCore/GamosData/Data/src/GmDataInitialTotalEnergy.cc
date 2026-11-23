#include "GmDataInitialTotalEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialTotalEnergy::GmDataInitialTotalEnergy()
{
  bInitial = true;
  theHMax = 1.;
}

//----------------------------------------------------------------
GmDataInitialTotalEnergy::~GmDataInitialTotalEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataInitialTotalEnergy::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetKineticEnergy()+aStep->GetTrack()->GetDefinition()->GetPDGMass();
}

//----------------------------------------------------------------
G4double GmDataInitialTotalEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexKineticEnergy()+aTrack->GetDefinition()->GetPDGMass();
}

//----------------------------------------------------------------
G4double GmDataInitialTotalEnergy::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetKineticEnergy()+aTrack2->GetDefinition()->GetPDGMass();
}

//----------------------------------------------------------------
G4double GmDataInitialTotalEnergy::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4double mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().mag();
  G4double mass = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMass();

  return sqrt( sqr(mom) + sqr(mass) );
}

//----------------------------------------------------------------
G4double GmDataInitialTotalEnergy::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetKineticEnergy()+aTrack->GetDefinition()->GetPDGMass();
}
