#include "GmMovementEventAction.hh"
#include "GmVMovement.hh"
#include "GmMovementMgr.hh"
#include "GmMovementMessenger.hh"
#include "GmMovementVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmMovementUtils.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//---------------------------------------------------------------------
GmMovementEventAction::GmMovementEventAction()
{
  new GmMovementMessenger();
}

//---------------------------------------------------------------------
void GmMovementEventAction::BeginOfEventAction(const G4Event* anEvent)
{
  //--- take as event time that of the first primary vertex
  std::vector<GmVMovement*> movements = GmMovementMgr::GetInstance()->GetNewMovements(anEvent->GetEventID(),anEvent->GetPrimaryVertex(0)->GetT0() );

  std::vector<GmVMovement*>::const_iterator ite;
#ifndef GAMOS_NO_VERBOSE
  if( MoveVerb(debugVerb) ) G4cout << " GmMovementEventAction::BeginOfEventAction MOVE " << G4endl;
#endif
  for( ite = movements.begin(); ite != movements.end(); ite++) {
    (*ite)->Move();
  }

  //  G4cout << " GmMovementEventAction::BeginOfEventAction() nmovements " << movements.size() << G4endl; //GDEB
  

  // Recalculate generator distribution (in case a GmGenerator GenerDist use volume position)
  if( GmMovementUtils::bUsingGmGenerator && movements.size() != 0 ) {
    theGmGenerator->ReInitialiseDistributions();
  }


}

//---------------------------------------------------------------------
void GmMovementEventAction::BeginOfRunAction(const G4Run* )
{
  GmMovementMgr::GetInstance()->InitialiseLastValue();

  if( GmMovementUtils::bUsingGmGenerator ) {
    theGmGenerator = (GmGenerator*)(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  }
}
