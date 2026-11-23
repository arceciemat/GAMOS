#include "GmDataFinalLogicalVolume.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

//----------------------------------------------------------------
GmDataFinalLogicalVolume::GmDataFinalLogicalVolume()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLogicalVolume::~GmDataFinalLogicalVolume()
{
}

//----------------------------------------------------------------
G4String GmDataFinalLogicalVolume::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  
 if( PV ) { 
    return PV->GetLogicalVolume()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}

//----------------------------------------------------------------
G4String GmDataFinalLogicalVolume::GetStringValueFromTrack( const G4Track* aTrack )
{ 
 G4VPhysicalVolume* PV = aTrack->GetNextVolume();
 if( PV ) { 
    return PV->GetLogicalVolume()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}
