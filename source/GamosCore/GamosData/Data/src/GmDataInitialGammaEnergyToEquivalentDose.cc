#include "GmDataInitialGammaEnergyToEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialGammaEnergyToEquivalentDose::GmDataInitialGammaEnergyToEquivalentDose()
{
  bInitial = false;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theFlux2Dose = ReadEnergyBinsForGammas();

  SetEquivDoseType("GmDataInitialGammaEnergyToEquivalentDose");

}

//----------------------------------------------------------------
GmDataInitialGammaEnergyToEquivalentDose::~GmDataInitialGammaEnergyToEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataInitialGammaEnergyToEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return EnergyToDoseFactor( aStep->GetPreStepPoint()->GetKineticEnergy());
  
}


