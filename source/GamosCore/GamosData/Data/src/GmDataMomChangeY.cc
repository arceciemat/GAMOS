#include "GmDataMomChangeY.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataMomChangeY::GmDataMomChangeY()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataMomChangeY::~GmDataMomChangeY()
{
}


//----------------------------------------------------------------
G4double GmDataMomChangeY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().y()-aStep->GetPreStepPoint()->GetMomentum().y();
}

//----------------------------------------------------------------
G4double GmDataMomChangeY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().y()-GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() )*aTrack->GetMomentumDirection().y();
}
