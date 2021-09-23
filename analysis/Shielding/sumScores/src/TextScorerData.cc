#include "TextScorerData.hh"
#include "ScoreData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//----------------------------------------------------------------------
TextScorerData::TextScorerData() : ScorerData()
{
  theScoreType = ST_TEXT;

}

//----------------------------------------------------------------------
void TextScorerData::Print( std::ofstream& fout )
{
  fout << " Scorer: " << theScorerName << G4endl;
  fout << " Number of entries= " << theNScores << G4endl;
  
  if( ScoreData::verbosity >= 2 ) G4cout << "  TextScorerData::Print: " << theScorerName << G4endl; 
 
  ScoreData* totalScoreData = 0;
  for( unsigned int ii = 0; ii < theScoreData.size(); ii++ ) {
    ScoreData* scoreData =  theScoreData[ii];
    if( !scoreData->IsTotal() ) { 
      scoreData->Print(fout);
    } else {
      totalScoreData = scoreData;
    }
  }

  totalScoreData->Print(fout);

}


//----------------------------------------------------------------------
G4String TextScorerData::GetIndexVal( G4String scoreName, G4bool bLogX ) 
{
  G4String indexVal = "0.";
  //--- Find separating '-' 
  // if found get value between the two values, if not take single value
  size_t igf = std::string::npos;
  for( size_t ii = 1; ii < scoreName.length(); ii++ ) {
    if( scoreName[ii] == '-' && scoreName[ii-1] != 'E' && scoreName[ii-1] != 'e' ){
      igf = ii;
      //      G4cout << scoreName <<  " FOUN " << igf << " " << scoreName [igf-1] << G4endl;//GDEB
    }
  }
  if( igf == std::string::npos ) { // NOT FOUND
    return GmGenUtils::GetValue(scoreName);
  }

  //  size_t igInterval = igf; // position of '-' defining an interva
  //  G4cout << " IGF " << igf << " " << scoreName.substr(0,igf) << " " << scoreName.substr(igf+1,scoreName.length()) << G4endl;//GDEB
  G4double valLower = GmGenUtils::GetValue(scoreName.substr(0,igf));
  G4double valUpper = GmGenUtils::GetValue(scoreName.substr(igf+1,scoreName.length()));
  G4double indexVald = GmGenUtils::GetValue(indexVal);
  if( ScoreData::theIndexValType == SIVT_MIDDLE ) {
    if( !bLogX ) {
      indexVald = (valLower+valUpper)/2.;
    } else {
      G4double valSq = valLower*valUpper;
      if( valSq >= 0 ) {
	indexVald = std::sqrt(valSq) ;
	if( valLower < 0. || valUpper < 0. ) indexVald *= -1;
      } else {
	indexVald = 0.;
      }
    }
  } else if( ScoreData::theIndexValType == SIVT_LOWER ) {
    indexVald = valLower;
  } else if( ScoreData::theIndexValType == SIVT_UPPER ) {
    indexVald = valUpper;
  }
  indexVal = GmGenUtils::ftoa(indexVald);
  if( ScoreData::verbosity >= 2 ) G4cout << " GetIndexVal " << scoreName << " -> " << indexVal << " Lower " << valLower << " Upper " << valUpper << " theIndexValType " << ScoreData::theIndexValType << G4endl;

  if( ScoreData::verbosity >= 2 ) G4cout << " GetIndexVal " << scoreName << " -> " << indexVal  << G4endl;

  return indexVal;
}
