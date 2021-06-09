#include "GmDataFinalPhysicalVolume.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"

//----------------------------------------------------------------
GmDataFinalPhysicalVolume::GmDataFinalPhysicalVolume()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPhysicalVolume::~GmDataFinalPhysicalVolume()
{
}

//----------------------------------------------------------------
G4String GmDataFinalPhysicalVolume::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( PV ) { 
    return PV->GetName()+":"+GmGenUtils::ftoa(PV->GetCopyNo());
  } else {
    return G4String("OutOfWorld");
  }
}

//----------------------------------------------------------------
G4String GmDataFinalPhysicalVolume::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  G4VPhysicalVolume* PV = aTrack->GetNextVolume();
  if( PV ) { 
    return PV->GetName()+":"+GmGenUtils::ftoa(PV->GetCopyNo());
  } else {
    return G4String("OutOfWorld");
  }
}
