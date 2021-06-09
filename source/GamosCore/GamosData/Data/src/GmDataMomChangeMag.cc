#include "GmDataMomChangeMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataMomChangeMag::GmDataMomChangeMag()
{
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangeMag::~GmDataMomChangeMag()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangeMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentum()-aStep->GetPreStepPoint()->GetMomentum()).mag();
}

//----------------------------------------------------------------
G4double GmDataMomChangeMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentum()-GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection()).mag();
}
