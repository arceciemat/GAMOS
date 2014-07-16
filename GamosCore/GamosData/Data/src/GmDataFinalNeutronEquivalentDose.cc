#include "GmDataFinalNeutronEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalNeutronEquivalentDose::GmDataFinalNeutronEquivalentDose()
{
  bInitial = false;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theFlux2Dose = ReadEnergyBinsForNeutrons();

  SetEquivDoseType("GmDataFinalNeutronEquivalentDose");
}

//----------------------------------------------------------------
GmDataFinalNeutronEquivalentDose::~GmDataFinalNeutronEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataFinalNeutronEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return DoseFromEnergy( aStep->GetPostStepPoint()->GetKineticEnergy(), aStep );
  
}


