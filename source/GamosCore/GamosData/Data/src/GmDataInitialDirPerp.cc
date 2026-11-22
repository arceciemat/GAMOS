#include "GmDataInitialDirPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirPerp::GmDataInitialDirPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirPerp::~GmDataInitialDirPerp()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialDirPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialDirPerp::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialDirPerp::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.perp();
}


//----------------------------------------------------------------
G4double GmDataInitialDirPerp::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().perp();
}

