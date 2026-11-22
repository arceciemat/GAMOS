#include "GmDataInitialDirZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirZ::GmDataInitialDirZ()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirZ::~GmDataInitialDirZ()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().z();
}

//----------------------------------------------------------------
G4double GmDataInitialDirZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().z();
}

//----------------------------------------------------------------
G4double GmDataInitialDirZ::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().z();
}


//----------------------------------------------------------------
G4double GmDataInitialDirZ::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();

  return mom.z();
}

//----------------------------------------------------------------
G4double GmDataInitialDirZ::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().z();
}

