#include "GmDataMomChangePerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataMomChangePerp::GmDataMomChangePerp()
{
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangePerp::~GmDataMomChangePerp()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangePerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentum()-aStep->GetPreStepPoint()->GetMomentum()).perp();
}

//----------------------------------------------------------------
G4double GmDataMomChangePerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentum()-GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection()).perp();
}
