#include "GmDataFinalDirNormalAngle.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirNormalAngle::GmDataFinalDirNormalAngle()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirNormalAngle::~GmDataFinalDirNormalAngle()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirNormalAngle::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double cosangle = aStep->GetPostStepPoint()->GetMomentumDirection()*aStep->GetPostStepPoint()->GetPosition().unit();
  /*  if( cosangle > 1. ) {
      G4cerr << " GmDataFinalDirNormalAngle::GetValueFromStep TOO BIG COS ANGLE " << cosangle << " " << cosangle-1. << " DIR " << aStep->GetPostStepPoint()->GetMomentumDirection() << */
  return acos(cosangle)/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalDirNormalAngle::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return acos(aTrack->GetMomentumDirection()*aTrack->GetPosition().unit())/CLHEP::deg;
}
