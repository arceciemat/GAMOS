#include "GmDataFinalLocalDirX.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalDirX::GmDataFinalLocalDirX()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalDirX::~GmDataFinalLocalDirX()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalDirX::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPostStepPoint()->GetMomentumDirection(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localDir.x();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalDirX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack->GetMomentumDirection(), aTrack->GetNextTouchable()->GetHistory());

  return localDir.x();
}


