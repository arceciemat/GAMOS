#include "ScorerData.hh"
#include "ScoreData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//-----------------------------------------------------------------
ScorerData::ScorerData()
{
  theNumberOfFiles = 1;
  theNScores = 0;
}

//-----------------------------------------------------------------
void ScorerData::AddScoreData(ScoreData* ed )
{
  theScoreData.push_back(ed);
  ed->SetScorerData( this );
  //  if( bSetScorer) ed->MultiplyByNEvents();
  if( ScoreData::verbosity >= 2 ) G4cout << " ScorerData::AddScoreData " << theName << " SIZE= " << theScoreData.size() << " : " << ed->GetName() << G4endl;

}

//-----------------------------------------------------------------
void ScorerData::SetNEvents(G4double evts) 
{
  //  G4cout << " SetNEvents  " << theName << " = " << theNEvents << " nevts " << evts << G4endl;
  theNEvents = evts;
}

//-----------------------------------------------------------------
ScorerData ScorerData::operator+=( const ScorerData& sd2 )
{
  if( ScoreData::verbosity >= 1 ) G4cout << "  ScorerData::operator+=: ADDING SCORER " << sd2.GetName() << "  N= " << sd2.GetNScores() << "  TO  " << theName << "  N= " << theNScores << G4endl;

  G4double NEvents2 = sd2.GetNEvents();
  /*t  for( int ii = 0; ii < GetNScores()+1; ii++ ){
    ScoreData* id = GetScoreData(ii);
    (*id) *= theNEvents;
    } */

  // loop to score data of 2 and check if same score data is found in 1: if yes, add data, if not create new data
  /* if( sd2.GetNScores() == 0 ) {
    G4Exception("ScorerData::operator+=",
		"",
		JustWarning,
		G4String("Adding empty scorer " + sd2.GetName()).c_str());
    return *this;
    }*/

  //  G4cout << sd2.GetName() << " N SCORES " <<  sd2.GetNScores() << G4endl; //GDEB
  if( ScoreData::verbosity >= 2 ) G4cout << "@@@ CHECK IF NEW SCORER ADDS SOME NEW SCORE " << G4endl; 
  for( int ii2 = 0; ii2 < sd2.GetNScores()+1; ii2++ ){
    ScoreData* id2 = sd2.GetScoreData(ii2);
    bool bFound1 = false;
    ScoreData* id1 = 0;
    for( int ii1 = 0; ii1 < theNScores+1; ii1++ ){
      id1 = theScoreData[ii1];
      if( *id1 == *id2 ) {
	bFound1 = true;
	break;
      }
    }

    //-    G4cout << ii2 << " CHECK DATA 2 " << id2->GetName() << " " << id2->GetScore() << " bFound1 " << bFound1 << G4endl; //GDEB

    if( bFound1 ) {
      *id1 += *id2;
      if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: " << theName << " ADDED DATA " << id2->GetName() << " to " << id1->GetName() << G4endl; 
    } else {
      AddScoreData( id2 );
      //-      (*id2) *= NEvents2;
      theNScores += 1;
      if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: " << theName << " CREATED NEW DATA " << id2->GetName() << G4endl; 
    }

  }
      
  theNumberOfFiles++;

  /*t  for( int ii = 0; ii < GetNScores()+1; ii++ ){
    ScoreData* id = GetScoreData(ii);
    (*id) *= 1./(theNEvents+NEvents2);
    if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: " << theName << " SCALE TO PER EVENT " << id->GetName() << " = " << id->GetScore() << G4endl; 
    } */

  theNEvents += NEvents2;

  return *this;
  
}

//-----------------------------------------------------------------
bool ScorerData::operator!=( const ScorerData& sd2 )
{

  if (GetName() != sd2.GetName() ) {
    return true;
  }

  return false;

}

//-----------------------------------------------------------------
ScoreData* ScorerData::GetScoreData(unsigned int ii) const
{
  if( ii >= theScoreData.size() ) {
    G4cerr << theName << " Asking for score " << ii << " >= N scores = " << theScoreData.size() << G4endl;
    G4Exception("ScorerData::GetScoreData",
		"Asking for an score that does not exists",
		FatalException,
		"You are trying to add an scorer with ' Number of entries= ' wrong");
  }
  return theScoreData[ii];
}

//-----------------------------------------------------------------
mspdd* ScorerData::GetPlotData( G4bool bLogX )
{
  mspdd* plotData = new mspdd;
  //-  G4cout << "  GETPLOTDATA " << theScoreData.size() << G4endl;
  for( unsigned int ii = 0; ii < theScoreData.size(); ii++ ) {
    ScoreData* scoreData = theScoreData[ii];
    //    G4cerr << " GETPLOTDATA " << scoreData<< G4endl;    //GDEB

    // Index must represent a numeric value
    G4String scoreName = scoreData->GetName();
    if( !GmGenUtils::IsNumber(scoreName) ) {
      if( scoreName == "SUM_ALL:" ) continue;
      /*      G4Exception("GAMOSScorerData::GetPlotData",
		  "Non numeric score index",
		  FatalException,
		  (G4String("Score index is not numeric = ")+scoreName).c_str()); */
    } 
    //    G4cout << "  GETPLOTDATA scoreName " << scoreName << " bLogX " << bLogX << G4endl; //GDEB
    G4String indexVal = GetIndexVal( scoreName, bLogX );
    pdd pd(scoreData->GetScore(),scoreData->GetScoreError());
    //    G4cout << " GETPLOTDATA " << indexVal << " " << scoreData->GetScore() << " +- " << scoreData->GetScoreError() << G4endl;//GDEB
    (*plotData)[indexVal] = pd;
  }

  return plotData;

}