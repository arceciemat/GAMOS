#include "GmDataFinalLocalDirZ.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalDirZ::GmDataFinalLocalDirZ()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalDirZ::~GmDataFinalLocalDirZ()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalDirZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPostStepPoint()->GetMomentumDirection(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localDir.z();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalDirZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack->GetMomentumDirection(), aTrack->GetNextTouchable()->GetHistory());

  return localDir.z();
}


