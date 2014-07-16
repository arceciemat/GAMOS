#include "GmDataFinalMomZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomZ::GmDataFinalMomZ()
{
  bInitial = false;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomZ::~GmDataFinalMomZ()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().z();
}

//----------------------------------------------------------------
G4double GmDataFinalMomZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().z();
}
