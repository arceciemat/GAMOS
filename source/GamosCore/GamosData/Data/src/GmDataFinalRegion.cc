#include "GmDataFinalRegion.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"

//----------------------------------------------------------------
GmDataFinalRegion::GmDataFinalRegion()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalRegion::~GmDataFinalRegion()
{
}

//----------------------------------------------------------------
G4String GmDataFinalRegion::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( PV ) { 
    return PV->GetLogicalVolume()->GetRegion()->GetName();
  } else {
    return G4String("OutOfWorld");
  }

}

//----------------------------------------------------------------
G4String GmDataFinalRegion::GetStringValueFromTrack( const G4Track* aTrack )
{ 

  G4VPhysicalVolume* PV = aTrack->GetNextVolume();
  if( PV ) { 
    return PV->GetLogicalVolume()->GetRegion()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}
