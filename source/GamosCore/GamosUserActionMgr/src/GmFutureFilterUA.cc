#include "G4Run.hh"
#include "G4Event.hh"

#include "GmFutureFilterUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "GmStepMgr.hh"

//-----------------------------------------------------------------------
void GmFutureFilterUA::UserSteppingAction( const G4Step* aStep)
{
  //  if( futureFilter->AcceptPastStep(aStep) || futureFilter->IsFilterToChildren() ) stepMgr->SaveStep( aStep );
  theStepMgr->SaveStep( aStep );
}

//-----------------------------------------------------------------------
void GmFutureFilterUA::EndOfEventAction( const G4Event* )
{
  theStepMgr->ClearObjects();

}
//-----------------------------------------------------------------------
GmFutureFilterUA::GmFutureFilterUA() 
{
}
