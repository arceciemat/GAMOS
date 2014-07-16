#include "GmDataInitialDirY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirY::GmDataInitialDirY()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirY::~GmDataInitialDirY()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().y();
}

//----------------------------------------------------------------
G4double GmDataInitialDirY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().y();
}

//----------------------------------------------------------------
G4double GmDataInitialDirY::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().y();
}


//----------------------------------------------------------------
G4double GmDataInitialDirY::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.y();
}
