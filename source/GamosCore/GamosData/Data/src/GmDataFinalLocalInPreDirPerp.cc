#include "GmDataFinalLocalInPreDirPerp.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalInPreDirPerp::GmDataFinalLocalInPreDirPerp()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalInPreDirPerp::~GmDataFinalLocalInPreDirPerp()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalInPreDirPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPostStepPoint()->GetMomentumDirection(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory());

  return localDir.perp();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalInPreDirPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack->GetMomentumDirection(), touch->GetHistory() );

  return localDir.perp();
}


