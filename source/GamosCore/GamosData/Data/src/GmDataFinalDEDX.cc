#include "GmDataFinalDEDX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4EmCalculator.hh"

//----------------------------------------------------------------
GmDataFinalDEDX::GmDataFinalDEDX()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theEmCalculator = new G4EmCalculator;
  theExcludedTypes.insert( DTEvent );
  theExcludedTypes.insert( DTSeco );
}

//----------------------------------------------------------------
GmDataFinalDEDX::~GmDataFinalDEDX()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDEDX::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4StepPoint* stepPoint = aStep->GetPostStepPoint();
  return theEmCalculator->GetDEDX( stepPoint->GetKineticEnergy(), 
				   aStep->GetTrack()->GetDefinition(), 
				   stepPoint->GetMaterial(), 
				   stepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion() );
}

//----------------------------------------------------------------
G4double GmDataFinalDEDX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return theEmCalculator->GetDEDX( aTrack->GetKineticEnergy(), 
				   aTrack->GetDefinition(), 
				   aTrack->GetVolume()->GetLogicalVolume()->GetMaterial(), 
				   aTrack->GetVolume()->GetLogicalVolume()->GetRegion() );
}

