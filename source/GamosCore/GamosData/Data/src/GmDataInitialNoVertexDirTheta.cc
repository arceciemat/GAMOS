#include "GmDataInitialNoVertexDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialNoVertexDirTheta::GmDataInitialNoVertexDirTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialNoVertexDirTheta::~GmDataInitialNoVertexDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataInitialNoVertexDirTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialNoVertexDirTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialNoVertexDirTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialNoVertexDirTheta::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.theta();
}
