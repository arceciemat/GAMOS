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
  GmScoringMgr::GetInstance()->CountSumsForLastEventInAllScorers();

  //- GmScoringRun object.
  GmScoringRun* scrun = (GmScoringRun*)aRun;
  //--- Dump all socred quantities involved in GmScoringRun.
  scrun->DumpAllScorers();
  //---
  //  GmScoringMgr::GetInstance()->PrintAllScorers();

}


//----------------------------------------------------------------------
void GmScoringUA::BeginOfEventAction( const G4Event* )
{ 
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  std::map<G4String,GmVPrimitiveScorer*> scorers = GmScoringMgr::GetInstance()->GetScorers();

  for( ite = scorers.begin(); ite != scorers.end(); ite++ ){
    (*ite).second->SetNewEvent( TRUE );
  }
}
