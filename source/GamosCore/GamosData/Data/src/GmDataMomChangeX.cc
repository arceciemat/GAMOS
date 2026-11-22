#include "GmDataMomChangeX.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataMomChangeX::GmDataMomChangeX()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangeX::~GmDataMomChangeX()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangeX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().x()-aStep->GetPreStepPoint()->GetMomentum().x();
}

//----------------------------------------------------------------
G4double GmDataMomChangeX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().x()-GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection().x();
}
