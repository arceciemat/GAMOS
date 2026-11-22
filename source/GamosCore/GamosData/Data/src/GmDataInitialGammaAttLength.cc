#include "GmDataInitialGammaAttLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4EmCalculator.hh"
#include "G4PVParameterised.hh"
#include "G4Gamma.hh"

//----------------------------------------------------------------
GmDataInitialGammaAttLength::GmDataInitialGammaAttLength()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theEmCalculator = new G4EmCalculator;
  theExcludedTypes.insert( DTEvent );
}

//----------------------------------------------------------------
GmDataInitialGammaAttLength::~GmDataInitialGammaAttLength()
{
}


//----------------------------------------------------------------
G4double GmDataInitialGammaAttLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  if( aStep->GetTrack()->GetDefinition() != G4Gamma::Gamma() ) {
    G4Exception("GmDataFinalGammaAttLength::GetValueFromTrack",
		"",
		FatalException,
		"This data can only be used for gammas, please add a GmGammaFilter");
  }
  G4StepPoint* stepPoint = aStep->GetPreStepPoint();
  return theEmCalculator->ComputeGammaAttenuationLength( stepPoint->GetKineticEnergy(), 
							 stepPoint->GetMaterial());
}

//----------------------------------------------------------------
G4double GmDataInitialGammaAttLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  if( aTrack->GetDefinition() != G4Gamma::Gamma() ) {
    G4Exception("GmDataFinalGammaAttLength::GetValueFromTrack",
		"",
		FatalException,
		"This data can only be used for gammas, please add a GmGammaFilter");
  }
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return theEmCalculator->ComputeGammaAttenuationLength( aTrack->GetVertexKineticEnergy(), 
							 mate);
}

//----------------------------------------------------------------
G4double GmDataInitialGammaAttLength::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  if( aTrack2->GetDefinition() != G4Gamma::Gamma() ) {
    G4Exception("GmDataFinalGammaAttLength::GetValueFromTrack",
		"",
		FatalException,
		"This data can only be used for gammas, please add a GmGammaFilter");
  }
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return theEmCalculator->ComputeGammaAttenuationLength( aTrack2->GetKineticEnergy(), 
							    mate);
}

//----------------------------------------------------------------
G4double GmDataInitialGammaAttLength::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  if( aTrack->GetDefinition() != G4Gamma::Gamma() ) {
    G4Exception("GmDataFinalGammaAttLength::GetValueFromTrack",
		"",
		FatalException,
		"This data can only be used for gammas, please add a GmGammaFilter");
  }
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return theEmCalculator->ComputeGammaAttenuationLength( aTrack->GetKineticEnergy(),
							 mate);
}


