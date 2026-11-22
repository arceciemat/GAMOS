#include "GmDataFinalGammaEnergyToEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalGammaEnergyToEquivalentDose::GmDataFinalGammaEnergyToEquivalentDose()
{
  bInitial = false;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  SetEquivDoseType("GmDataFinalGammaEnergyToEquivalentDose");

  theFlux2Dose = ReadEnergyBinsForGammas();

}

//----------------------------------------------------------------
GmDataFinalGammaEnergyToEquivalentDose::~GmDataFinalGammaEnergyToEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataFinalGammaEnergyToEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return EnergyToDoseFactor( aStep->GetPostStepPoint()->GetKineticEnergy());
  
}


