#include "GmDataInitialDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirTheta::GmDataInitialDirTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirTheta::~GmDataInitialDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.theta();
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().theta();
}

