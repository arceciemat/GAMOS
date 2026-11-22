#include "GmDataMomChangeTheta.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataMomChangeTheta::GmDataMomChangeTheta()
{
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangeTheta::~GmDataMomChangeTheta()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangeTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentum()-aStep->GetPreStepPoint()->GetMomentum()).theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataMomChangeTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentum() - GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection()).theta()/CLHEP::deg;
}
