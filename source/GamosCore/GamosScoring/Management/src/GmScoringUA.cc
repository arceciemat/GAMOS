#include "GmScoringUA.hh"
#include "GmScoringRun.hh"
#include "GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4Run.hh"

//-----------------------------------------------------------------
GmScoringUA::GmScoringUA()
{
}

//-----------------------------------------------------------------
GmScoringUA::~GmScoringUA() 
{
}

//-----------------------------------------------------------------
G4Run* GmScoringUA::GenerateRun()
{
  //  G4cout << " generatin GmScoringRun " << G4endl;
  return new GmScoringRun();

}

//-----------------------------------------------------------------
void GmScoringUA::EndOfRunAction(const G4Run* aRun )
{
  //-  GmScoringMgr::GetInstance()->CountSumsForLastEventInAllScorers();

  //- GmScoringRun object.
  GmScoringRun* scrun = (GmScoringRun*)aRun;
  //--- Dump all scored quantities involved in GmScoringRun.
  scrun->DumpAllScorers();
  //---
  //  GmScoringMgr::GetInstance()->PrintAllScorers();

}


#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
void GmScoringUA::BeginOfEventAction( const G4Event* )
{ 
  int nEvents = GmParameterMgr::GetInstance()->GetNumericValue("SqdoseEachNEvents",0);
  if( nEvents != 0 ) {
    //    if ( int(GmNumberOfEvent::GetNumberOfEvent()) < 5899000 ) return;
    //    if ( int(GmNumberOfEvent::GetNumberOfEvent()) < 1181000 ) return;
    if( int(GmNumberOfEvent::GetNumberOfEvent()) % nEvents == 0 ) {
      const G4Run* aRun = G4RunManager::GetRunManager()->GetCurrentRun();
      std::cout << " DUMP SCORES " << GmNumberOfEvent::GetNumberOfEvent() << std::endl;//GDEB
      GmScoringRun* scrun = (GmScoringRun*)aRun;
      //--- Dump all scored quantities involved in GmScoringRun.
      scrun->DumpAllScorers();
    }
  }

}

//----------------------------------------------------------------------
void GmScoringUA::EndOfEventAction( const G4Event* )
{ 
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  std::map<G4String,GmVPrimitiveScorer*> scorers = GmScoringMgr::GetInstance()->GetScorers();

  for( ite = scorers.begin(); ite != scorers.end(); ite++ ){
    (*ite).second->SumEndOfEvent();
  }
}
