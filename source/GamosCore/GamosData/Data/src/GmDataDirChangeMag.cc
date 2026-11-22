#include "GmDataDirChangeMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangeMag::GmDataDirChangeMag()
{
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangeMag::~GmDataDirChangeMag()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangeMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentumDirection()-aStep->GetPreStepPoint()->GetMomentumDirection()).mag();
}

//----------------------------------------------------------------
G4double GmDataDirChangeMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentumDirection()-aTrack->GetVertexMomentumDirection()).mag();
}
