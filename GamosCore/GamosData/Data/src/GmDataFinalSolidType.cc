#include "GmDataFinalSolidType.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

//----------------------------------------------------------------
GmDataFinalSolidType::GmDataFinalSolidType()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalSolidType::~GmDataFinalSolidType()
{
}

//----------------------------------------------------------------
G4String GmDataFinalSolidType::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetEntityType();
  } else {
    return G4String("OutOfWorld");
  }
}

//----------------------------------------------------------------
G4String GmDataFinalSolidType::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  G4VPhysicalVolume* PV = aTrack->GetNextVolume();
  if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetEntityType();
  } else {
    return G4String("OutOfWorld");
  }
}
