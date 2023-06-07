#include "GmDataFinalMeanIoniPot.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalMeanIoniPot::GmDataFinalMeanIoniPot()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMeanIoniPot::~GmDataFinalMeanIoniPot()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMeanIoniPot::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4Material* mate = aStep->GetPostStepPoint()->GetMaterial();
  if( mate ) {
    return mate->GetIonisation()->GetMeanExcitationEnergy();
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------
G4double GmDataFinalMeanIoniPot::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = aTrack->GetNextMaterial();
  if( mate ) {
    return mate->GetIonisation()->GetMeanExcitationEnergy();
  } else {
    return 0.;
  }
}
