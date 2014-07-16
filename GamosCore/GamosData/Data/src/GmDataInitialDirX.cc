#include "GmDataInitialDirX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirX::GmDataInitialDirX()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirX::~GmDataInitialDirX()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().x();
}

//----------------------------------------------------------------
G4double GmDataInitialDirX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().x();
}

//----------------------------------------------------------------
G4double GmDataInitialDirX::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().x();
}

//----------------------------------------------------------------
G4double GmDataInitialDirX::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.x();
}
