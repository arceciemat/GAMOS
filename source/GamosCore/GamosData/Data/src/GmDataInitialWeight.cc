#include "GmDataInitialWeight.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataInitialWeight::GmDataInitialWeight()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialWeight::~GmDataInitialWeight()
{
}


//----------------------------------------------------------------
G4double GmDataInitialWeight::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetWeight();
}


//----------------------------------------------------------------
G4double GmDataInitialWeight::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetWeight();
}

//----------------------------------------------------------------
G4double GmDataInitialWeight::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetWeight();
}
