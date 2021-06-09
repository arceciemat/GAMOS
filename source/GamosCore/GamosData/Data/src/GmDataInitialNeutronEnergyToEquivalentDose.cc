#include "GmDataInitialNeutronEnergyToEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialNeutronEnergyToEquivalentDose::GmDataInitialNeutronEnergyToEquivalentDose()
{
  bInitial = false;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theFlux2Dose = ReadEnergyBinsForNeutrons();

  SetEquivDoseType("GmDataInitialNeutronEnergyToEquivalentDose");

}

//----------------------------------------------------------------
GmDataInitialNeutronEnergyToEquivalentDose::~GmDataInitialNeutronEnergyToEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataInitialNeutronEnergyToEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return EnergyToDoseFactor( aStep->GetPreStepPoint()->GetKineticEnergy());
  
}


