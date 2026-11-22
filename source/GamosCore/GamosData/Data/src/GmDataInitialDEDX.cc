#include "GmDataInitialDEDX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4EmCalculator.hh"
#include "G4PVParameterised.hh"

//----------------------------------------------------------------
GmDataInitialDEDX::GmDataInitialDEDX()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theEmCalculator = new G4EmCalculator;
  theExcludedTypes.insert( DTEvent );
}

//----------------------------------------------------------------
GmDataInitialDEDX::~GmDataInitialDEDX()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDEDX::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4StepPoint* stepPoint = aStep->GetPreStepPoint();
  return theEmCalculator->GetDEDX( stepPoint->GetKineticEnergy(), 
				   aStep->GetTrack()->GetDefinition(), 
				   stepPoint->GetMaterial(), 
				   stepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion() );
}

//----------------------------------------------------------------
G4double GmDataInitialDEDX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return theEmCalculator->GetDEDX( aTrack->GetVertexKineticEnergy(), 
				   aTrack->GetDefinition(), 
				   mate, 
				   aTrack->GetLogicalVolumeAtVertex()->GetRegion() );
}

//----------------------------------------------------------------
G4double GmDataInitialDEDX::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return theEmCalculator->GetDEDX( aTrack2->GetKineticEnergy(), 
				   aTrack2->GetDefinition(), 
				   mate, 
				   aTrack1->GetNextVolume()->GetLogicalVolume()->GetRegion() );
}

//----------------------------------------------------------------
G4double GmDataInitialDEDX::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return theEmCalculator->GetDEDX( aTrack->GetKineticEnergy(), 
				   aTrack->GetDefinition(), 
				   mate,
				   aTrack->GetLogicalVolumeAtVertex()->GetRegion() );
}


