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
ScorerData ScorerData::operator+=( const ScorerData& srd2 )
{
  if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: ADDING SCORER " << srd2.GetName() << "  N= " << srd2.GetNScores() << "  TO  " << theName << "  N= " << theNScores << G4endl;

  G4double NEvents2 = srd2.GetNEvents();
  /*t  for( int ii = 0; ii < GetNScores()+1; ii++ ){
    ScoreData* sd = GetScoreData(ii);
    (*sd) *= theNEvents;
    } */

  // loop to score data of 2 and check if same score data is found in 1: if yes, add data, if not create new data
  /* if( srd2.GetNScores() == 0 ) {
    G4Exception("ScorerData::operator+=",
		"",
		JustWarning,
		G4String("Adding empty scorer " + srd2.GetName()).c_str());
    return *this;
    }*/

  //  G4cout << srd2.GetName() << " N SCORES " <<  srd2.GetNScores() << G4endl; //GDEB
  if( ScoreData::verbosity >= 2 ) G4cout << "@@@ CHECK IF NEW SCORER ADDS SOME NEW SCORE " << G4endl; 
  for( int ii2 = 0; ii2 < srd2.GetNScores()+1; ii2++ ){
    ScoreData* sd2 = srd2.GetScoreData(ii2);
    if( ScoreData::verbosity >= 3 && ii2%10000 == 0 ) G4cout << ii2 << " out of " <<  srd2.GetNScores() << "  ScorerData::operator+=: " << theName << " DATA " << sd2->GetName() << G4endl;
    bool bFound1 = false;
    ScoreData* sd1 = 0;
    for( size_t ii1 = 0; ii1 < theNScores+1; ii1++ ){
      if( ii1 >= theScoreData.size() ) break;
      sd1 = theScoreData[ii1];
      if( *sd1 == *sd2 ) {
	bFound1 = true;
	break;
      }
    }

    //-    G4cout << ii2 << " CHECK DATA 2 " << sd2->GetName() << " " << sd2->GetScore() << " bFound1 " << bFound1 << G4endl; //GDEB

    if( bFound1 ) {
      *sd1 += *sd2;
      if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: " << theName << " ADDED DATA " << sd2->GetName() << " to " << sd1->GetName() << G4endl;
    } else {
      AddScoreData( sd2 );
      //-      (*sd2) *= NEvents2;
      theNScores += 1;
      if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: " << theName << " CREATED NEW DATA " << sd2->GetName() << G4endl; 
    }

  }
      
  theNumberOfFiles++;

  /*t  for( int ii = 0; ii < GetNScores()+1; ii++ ){
    ScoreData* sd = GetScoreData(ii);
    (*sd) *= 1./(theNEvents+NEvents2);
    if( ScoreData::verbosity >= 2 ) G4cout << "  ScorerData::operator+=: " << theName << " SCALE TO PER EVENT " << sd->GetName() << " = " << sd->GetScore() << G4endl; 
    } */

  theNEvents += NEvents2;

  return *this;
  
}

//-----------------------------------------------------------------
bool ScorerData::operator!=( const ScorerData& srd2 )
{

  if (GetName() != srd2.GetName() ) {
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
