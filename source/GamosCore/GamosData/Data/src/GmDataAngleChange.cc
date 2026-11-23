#include "GmDataAngleChange.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAngleChange::GmDataAngleChange()
{
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataAngleChange::~GmDataAngleChange()
{
}

//----------------------------------------------------------------
G4double GmDataAngleChange::GetValueFromStep( const G4Step* aStep, G4int )
{

 double cosAng  = aStep->GetPostStepPoint()->GetMomentumDirection()*aStep->GetPreStepPoint()->GetMomentumDirection();
 if( cosAng > 1. ) cosAng = 1.;
 if( cosAng < -1. ) cosAng = -1.;
  return acos(cosAng)/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataAngleChange::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return acos(aTrack->GetMomentumDirection() * aTrack->GetVertexMomentumDirection())/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataAngleChange::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
#ifndef WIN32 //WINDEB
  if( DataVerb(debugVerb) ) G4cout << " GmDataAngleChange::GetValueFromSecoTrack " 
	 << acos(aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection()*aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection())/CLHEP::deg
	 << " anglePost " << aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection()
	 << " anglePre " << aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection() << G4endl;
#endif
  return acos(aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection()*aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection())/CLHEP::deg;
}
