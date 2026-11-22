#include "GmScoringUA.hh"
#include "GmScoringRun.hh"
#include "GmScoringMgr.hh"
#include "GmVPrimitiveScorer.hh"
#include "G4Run.hh"
#include "G4Threading.hh"

//-----------------------------------------------------------------
GmScoringUA::GmScoringUA()
{
  //--- Multithreading: scores of all threads are merged in master run (GmScoringRun::Merge) and printed by the master
  SetRunOnMaster(true);
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
  if( G4Threading::IsMultithreadedApplication() && !G4Threading::IsMasterThread() ) return;
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
  if( nEvents != 0 && G4Threading::IsMultithreadedApplication() ) {
    static G4bool bWarned = false;
    if( !bWarned ) {
      bWarned = true;
      G4Exception("GmScoringUA::BeginOfEventAction","",JustWarning,"SqdoseEachNEvents is not supported in multithreading mode, it will be ignored");
    }
    nEvents = 0;
  }
  if( nEvents != 0 ) {
    //    if ( int(GmNumberOfEvent::GetNumberOfEvent()) < 5899000 ) return;
    //    if ( int(GmNumberOfEvent::GetNumberOfEvent()) < 1181000 ) return;
    if( int(GmNumberOfEvent::GetNumberOfEvent()) % nEvents == 0 && int(GmNumberOfEvent::GetNumberOfEvent()) != 0 ) {
      const G4Run* aRun = G4RunManager::GetRunManager()->GetCurrentRun();
      GmScoringRun* scrun = (GmScoringRun*)aRun;
      //--- Dump all scored quantities involved in GmScoringRun.
      scrun->DumpAllScorers();
    }
  }

}

//----------------------------------------------------------------------
void GmScoringUA::EndOfEventAction( const G4Event* )
{ 
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator iteS;
  std::map<G4String,GmVPrimitiveScorer*> scorers = GmScoringMgr::GetInstance()->GetScorers();
  for( iteS = scorers.begin(); iteS != scorers.end(); iteS++ ){
    (*iteS).second->SumEndOfEvent();
  }

}
