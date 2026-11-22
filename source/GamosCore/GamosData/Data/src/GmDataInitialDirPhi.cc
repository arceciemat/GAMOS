#include "GmDataInitialDirPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirPhi::GmDataInitialDirPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirPhi::~GmDataInitialDirPhi()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirPhi::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirPhi::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.phi();
}

//----------------------------------------------------------------
G4double GmDataInitialDirPhi::GetValueFromStackedTrack( const G4Track* aTrack, G4int  )
{
  return aTrack->GetMomentumDirection().phi();
}

