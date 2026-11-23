#include "GmDataFinalWeight.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalWeight::GmDataFinalWeight()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataFinalWeight::~GmDataFinalWeight()
{
}


//----------------------------------------------------------------
G4double GmDataFinalWeight::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataFinalWeight::GetValueFromStep " << aStep->GetPostStepPoint()->GetWeight() << " pre " << aStep->GetPreStepPoint()->GetWeight() << G4endl; 
  return aStep->GetPostStepPoint()->GetWeight();
}

//----------------------------------------------------------------
G4double GmDataFinalWeight::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  //  G4cout << " GmDataFinalWeight::GetValueFromTrack " << aTrack->GetWeight() << G4endl; 
  return aTrack->GetWeight();
}
