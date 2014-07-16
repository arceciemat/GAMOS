#include "GmDataAccumulatedEnergyLost.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"
#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedEnergyLost::GmDataAccumulatedEnergyLost()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataAccumulatedEnergyLost::~GmDataAccumulatedEnergyLost()
{
}
 
//----------------------------------------------------------------
void GmDataAccumulatedEnergyLost::Accumulate( const G4Step* aStep, G4int index )
{
  theDataAccumulated[index] += aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) {
    G4cout << index << " GmDataAccumulatedEnergyLost " << aStep->GetPreStepPoint()->GetKineticEnergy() 
	   << " - " << aStep->GetPostStepPoint()->GetKineticEnergy() 
	   << " ACCU " <<  theDataAccumulated[index] << G4endl;
  }
#endif
}


//----------------------------------------------------------------
G4double GmDataAccumulatedEnergyLost::GetValueFromStep( const G4Step* aStep, G4int )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) {
    G4cout << " GmDataAccumulatedEnergyLost::GetValueFromStep " << aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy() 
	   << " POST " <<aStep->GetPreStepPoint()->GetKineticEnergy() 
	   << " PRE " << aStep->GetPostStepPoint()->GetKineticEnergy() << G4endl;
  }
#endif

  return aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();
}
 
