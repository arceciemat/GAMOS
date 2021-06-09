#include "GmDataInitialNeutronEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialNeutronEquivalentDose::GmDataInitialNeutronEquivalentDose()
{ 
  bInitial = true;
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theFlux2Dose = ReadEnergyBinsForNeutrons();

  SetEquivDoseType("GmDataInitialNeutronEquivalentDose");

}

//----------------------------------------------------------------
GmDataInitialNeutronEquivalentDose::~GmDataInitialNeutronEquivalentDose()
{
}


//----------------------------------------------------------------
G4double GmDataInitialNeutronEquivalentDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  return DoseFromEnergy( aStep->GetPreStepPoint()->GetKineticEnergy(), aStep );
  
}

