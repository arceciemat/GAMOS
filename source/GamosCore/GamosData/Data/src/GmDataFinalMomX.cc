#include "GmDataFinalMomX.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomX::GmDataFinalMomX()
{
  bInitial = false;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomX::~GmDataFinalMomX()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().x();
}

//----------------------------------------------------------------
G4double GmDataFinalMomX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().x();
}
