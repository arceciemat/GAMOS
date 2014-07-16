#include "GmDataMomChangePhi.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataMomChangePhi::GmDataMomChangePhi()
{
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangePhi::~GmDataMomChangePhi()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangePhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentum()-aStep->GetPreStepPoint()->GetMomentum()).phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataMomChangePhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentum()-GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection()).phi()/CLHEP::deg;
}
