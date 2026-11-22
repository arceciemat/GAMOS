#include "GmDataFinalPhysicalVolume.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"

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
  return  GmG4Utils::GetNameNumberFromTouchable(aStep->GetPostStepPoint()->GetTouchable());

}

//----------------------------------------------------------------
G4String GmDataFinalPhysicalVolume::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return  GmG4Utils::GetNameNumberFromTouchable(aTrack->GetNextTouchable());
}
