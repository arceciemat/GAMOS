// katya
#include "GmDataInitialVelocity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmDataInitialVelocity::GmDataInitialVelocity()
{
  bInitial = true;
  theHMax = 1.;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);

}

//----------------------------------------------------------------
GmDataInitialVelocity::~GmDataInitialVelocity()
{
}


//----------------------------------------------------------------
G4double GmDataInitialVelocity::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataInitialVelocity::GetValueFromStep " << aStep->GetPreStepPoint()->GetVelocity() << G4endl;
  return aStep->GetPreStepPoint()->GetVelocity();
  // c_light = 299.792 mm/ns
}

//----------------------------------------------------------------
G4double GmDataInitialVelocity::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetVelocity();
}


