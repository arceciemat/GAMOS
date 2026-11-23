#include "GmDataInitialPrimSecoAngle.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataInitialPrimSecoAngle::GmDataInitialPrimSecoAngle()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTStep);
  //  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialPrimSecoAngle::~GmDataInitialPrimSecoAngle()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPrimSecoAngle::GetValueFromTrack( const G4Track*, G4int )
{
  return TRUE;
}

//----------------------------------------------------------------
G4double GmDataInitialPrimSecoAngle::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4TrackVector trkv = GmG4Utils::GetSecondariesOfCurrentStep();
  if( trkv.size() != 1 ) {
    /*    G4Exception("GmDataInitialPrimSecoAngle::GetValueFromStep",
		"",
		JustWarning,
		//		FatalException,
		("Can only be invoked if number of secondaries is 1, while it is "+GmGenUtils::itoa(trkv.size())).c_str());*/
    return DBL_MAX;
  }

  G4Track* aTrack1 = aStep->GetTrack();
  G4Track* aTrack2 = trkv[0];
  //  if( DataVerb(debugVerb) ) G4cout << "GmDataInitialPrimSecoAngle::GetValueFromStep return " << acos(aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection() * aTrack2->GetMomentumDirection() )/CLHEP::deg << G4endl;
  
  return acos(aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection() * aTrack2->GetMomentumDirection() )/CLHEP::deg;
}


//----------------------------------------------------------------
G4double GmDataInitialPrimSecoAngle::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  return acos(aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection() * aTrack2->GetMomentumDirection() )/CLHEP::deg;
}
