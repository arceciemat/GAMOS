#include "GmDataFinalSolid.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

//----------------------------------------------------------------
GmDataFinalSolid::GmDataFinalSolid()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalSolid::~GmDataFinalSolid()
{
}

//----------------------------------------------------------------
G4String GmDataFinalSolid::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}

//----------------------------------------------------------------
G4String GmDataFinalSolid::GetStringValueFromTrack( const G4Track* aTrack )
{ 
   G4VPhysicalVolume* PV = aTrack->GetNextVolume();return aTrack->GetNextVolume()->GetLogicalVolume()->GetSolid()->GetName();
 if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}
