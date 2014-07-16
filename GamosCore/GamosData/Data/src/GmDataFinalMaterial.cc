#include "GmDataFinalMaterial.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

//----------------------------------------------------------------
GmDataFinalMaterial::GmDataFinalMaterial()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMaterial::~GmDataFinalMaterial()
{
}

//----------------------------------------------------------------
G4String GmDataFinalMaterial::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetPostStepPoint()->GetMaterial()->GetName();
}

//----------------------------------------------------------------
G4String GmDataFinalMaterial::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetNextMaterial()->GetName();
}
