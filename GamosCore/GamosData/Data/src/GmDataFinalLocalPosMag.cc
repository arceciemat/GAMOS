#include "GmDataFinalLocalPosMag.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalPosMag::GmDataFinalLocalPosMag()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalPosMag::~GmDataFinalLocalPosMag()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalPosMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPostStepPoint()->GetPosition(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localPos.mag();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalPosMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack->GetPosition(), aTrack->GetNextTouchable()->GetHistory());

  return localPos.mag();
}


