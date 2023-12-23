#include "GAMOSScorerData.hh"
#include "ScoreData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//----------------------------------------------------------------------
GAMOSScorerData::GAMOSScorerData() : ScorerData()
{
  theScoreType = ST_GAMOS;

}

//----------------------------------------------------------------------
void GAMOSScorerData::Print( std::ofstream& fout )
{
  fout << " MultiFunctionalDet: " << theDetectorName << G4endl;
  fout << " PrimitiveScorer: " << theScorerName << G4endl;
  fout << " Number of entries= " << theNScores << G4endl;
  
  if( ScoreData::verbosity >= 2 ) G4cout << "  GAMOSScorerData::Print: " << theScorerName << G4endl; 
 
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
G4String GAMOSScorerData::GetIndexVal( G4String scoreName, G4bool bLogX ) 
{
  G4String indexVal = "";
  //--- Find separating '-'
  size_t igf = scoreName.find("-");
  size_t igInterval = std::string::npos; // position of '-' defining an interval
  //--- Do not count if first digit 
  if( igf == 0 ) igf = scoreName.find("-",igf+1);

  //--- Do not count if 'e-'
  if( igf != std::string::npos ) {
    size_t igfe = scoreName.find("e-");
    if( igfe == igf-1 ) {
      igf = scoreName.find("-",igf+1);
    }
  }

  if( igf != std::string::npos ) {
    igInterval = igf;
  }

  if( igInterval == std::string::npos ) {
    indexVal = scoreName;
  } else {
    G4double indexVald = GmGenUtils::GetValue(indexVal);
    G4double valLower = GmGenUtils::GetValue(scoreName.substr(0,igf));
    G4double valUpper = GmGenUtils::GetValue(scoreName.substr(igf+1,scoreName.length()));
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
    if( ScoreData::verbosity >= 2 ) G4cout << " GetIndexVal " << scoreName << " -> " << indexVald << " Lower " << valLower << " Upper " << valUpper << " theIndexValType " << ScoreData::theIndexValType << " LogX " << bLogX << G4endl;
    indexVal = GmGenUtils::ftoa(indexVald);
  }
    if( ScoreData::verbosity >= 2 ) G4cout << " GetIndexVal " << scoreName << " -> " << indexVal  << G4endl;

  return indexVal;
}
