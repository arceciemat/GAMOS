#include "GmDataFinalGammaEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalGammaEquivalentDose::GmDataFinalGammaEquivalentDose()
{
  bInitial = false;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);

  theFlux2Dose = ReadEnergyBinsForGammas();

  SetEquivDoseType("GmDataFinalGammaEquivalentDose");
}

//----------------------------------------------------------------
GmDataFinalGammaEquivalentDose::~GmDataFinalGammaEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataFinalGammaEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return DoseFromEnergy( aStep->GetPostStepPoint()->GetKineticEnergy(), aStep );
  
}


