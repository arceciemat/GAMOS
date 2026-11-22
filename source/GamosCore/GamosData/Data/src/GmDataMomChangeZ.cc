#include "GmDataMomChangeZ.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataMomChangeZ::GmDataMomChangeZ()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangeZ::~GmDataMomChangeZ()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangeZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().z()-aStep->GetPreStepPoint()->GetMomentum().z();
}

//----------------------------------------------------------------
G4double GmDataMomChangeZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().z()-GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection().z();
}
