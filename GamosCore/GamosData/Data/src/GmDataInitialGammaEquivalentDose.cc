#include "GmDataInitialGammaEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialGammaEquivalentDose::GmDataInitialGammaEquivalentDose()
{
  bInitial = true;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theFlux2Dose = ReadEnergyBinsForGammas();

  SetEquivDoseType("GmDataInitialGammaEquivalentDose");
}

//----------------------------------------------------------------
GmDataInitialGammaEquivalentDose::~GmDataInitialGammaEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataInitialGammaEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return DoseFromEnergy( aStep->GetPreStepPoint()->GetKineticEnergy(), aStep );
  
}



