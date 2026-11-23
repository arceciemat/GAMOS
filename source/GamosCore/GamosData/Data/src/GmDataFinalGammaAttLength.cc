#include "GmDataFinalGammaAttLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4EmCalculator.hh"
#include "G4Gamma.hh"

//----------------------------------------------------------------
GmDataFinalGammaAttLength::GmDataFinalGammaAttLength()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theEmCalculator = new G4EmCalculator;
  theExcludedTypes.insert( DTEvent );
  theExcludedTypes.insert( DTSeco );
}

//----------------------------------------------------------------
GmDataFinalGammaAttLength::~GmDataFinalGammaAttLength()
{
}


//----------------------------------------------------------------
G4double GmDataFinalGammaAttLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  if( aStep->GetTrack()->GetDefinition() != G4Gamma::Gamma() ) {
    G4Exception("GmDataFinalGammaAttLength::GetValueFromTrack",
		"",
		FatalException,
		"This data can only be used for gammas, please add a GmGammaFilter");
  }
  G4StepPoint* stepPoint = aStep->GetPostStepPoint();
  return theEmCalculator->ComputeGammaAttenuationLength( stepPoint->GetKineticEnergy(), 
							 stepPoint->GetMaterial());
}

//----------------------------------------------------------------
G4double GmDataFinalGammaAttLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  if( aTrack->GetDefinition() != G4Gamma::Gamma() ) {
    G4Exception("GmDataFinalGammaAttLength::GetValueFromTrack",
		"",
		FatalException,
		"This data can only be used for gammas, please add a GmGammaFilter");
  }
  return theEmCalculator->ComputeGammaAttenuationLength( aTrack->GetKineticEnergy(), 
							 aTrack->GetVolume()->GetLogicalVolume()->GetMaterial());
}

