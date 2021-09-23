#include "GmDataFinalTouchable.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VTouchable.hh"

//----------------------------------------------------------------
GmDataFinalTouchable::GmDataFinalTouchable()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalTouchable::~GmDataFinalTouchable()
{
}

//----------------------------------------------------------------
G4String GmDataFinalTouchable::GetStringValueFromStep( const G4Step* aStep )
{ 
  return GetTouchableLongName( aStep->GetPostStepPoint()->GetTouchable() );
}

//----------------------------------------------------------------
G4String GmDataFinalTouchable::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return GetTouchableLongName( aTrack->GetNextTouchable() );
}


//----------------------------------------------------------------
G4String GmDataFinalTouchable::GetTouchableLongName( const G4VTouchable* touch )
{
  if( touch && touch->GetVolume(0) ) { 
   G4int nAnces = touch->GetHistoryDepth();
   G4String name;
   for( G4int ii = nAnces; ii >= 0; ii-- ) {
     name += "/" + touch->GetVolume(ii)->GetName();
     name += ":" + GmGenUtils::itoa( touch->GetCopyNumber(ii) );
   }
   return name;
  } else {
    return G4String("OutOfWorld");
  }

}
