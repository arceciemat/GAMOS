#include "GmDataFinalNeutronEnergyToEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalNeutronEnergyToEquivalentDose::GmDataFinalNeutronEnergyToEquivalentDose()
{
  bInitial = false;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);

  SetEquivDoseType("GmDataFinalNeutronEnergyToEquivalentDose");

  theFlux2Dose = ReadEnergyBinsForNeutrons();

}

//----------------------------------------------------------------
GmDataFinalNeutronEnergyToEquivalentDose::~GmDataFinalNeutronEnergyToEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataFinalNeutronEnergyToEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return EnergyToDoseFactor( aStep->GetPostStepPoint()->GetKineticEnergy());
  
}


