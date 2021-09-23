#include "GmDataInitialKineticEnergy.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialKineticEnergy::GmDataInitialKineticEnergy()
{
  bInitial = true;
  theHMax = 1.;
}

//----------------------------------------------------------------
GmDataInitialKineticEnergy::~GmDataInitialKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataInitialKineticEnergy::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataInitialKineticEnergy::GetValueFromStep " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
  return aStep->GetPreStepPoint()->GetKineticEnergy();
}

//----------------------------------------------------------------
G4double GmDataInitialKineticEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  //  G4cout << " GmDataInitialKineticEnergy::GetValueFromTrack " << aTrack->GetVertexKineticEnergy() << G4endl;
  return aTrack->GetVertexKineticEnergy();
}

//----------------------------------------------------------------
G4double GmDataInitialKineticEnergy::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  //  G4cout << " GmDataInitialKineticEnergy::GetValueFromSecoTrack " << aTrack2->GetKineticEnergy() << G4endl;
  return aTrack2->GetKineticEnergy();
}

//----------------------------------------------------------------
G4double GmDataInitialKineticEnergy::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4double mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().mag();
  G4double mass = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMass();

  return GmGenUtils::GetKineticEnergy( mass, mom );
}

 //----------------------------------------------------------------
G4double GmDataInitialKineticEnergy::GetValueFromStackedTrack( const G4Track* aTrack, G4int ) 
{
  return aTrack->GetKineticEnergy();
}
