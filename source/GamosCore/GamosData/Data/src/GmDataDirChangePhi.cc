#include "GmDataDirChangePhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangePhi::GmDataDirChangePhi()
{
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangePhi::~GmDataDirChangePhi()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangePhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentumDirection()-aStep->GetPreStepPoint()->GetMomentumDirection()).phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataDirChangePhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentumDirection()-aTrack->GetVertexMomentumDirection()).phi()/CLHEP::deg;
}
