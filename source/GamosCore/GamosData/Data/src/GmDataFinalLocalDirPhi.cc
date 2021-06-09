#include "GmDataFinalLocalDirPhi.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalDirPhi::GmDataFinalLocalDirPhi()
{
  bInitial = false;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalDirPhi::~GmDataFinalLocalDirPhi()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalDirPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPostStepPoint()->GetMomentumDirection(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localDir.phi()/CLHEP::deg;

}

//----------------------------------------------------------------
G4double GmDataFinalLocalDirPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack->GetMomentumDirection(), aTrack->GetNextTouchable()->GetHistory());

  return localDir.phi()/CLHEP::deg;
}


