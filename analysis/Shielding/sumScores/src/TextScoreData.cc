#include "TextScoreData.hh"
#include "ScorerData.hh"
#include "TextScorerData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
TextScoreData::TextScoreData(std::vector<G4String> wl) 
  : ScoreData()
{
  theName = wl[0];
  theScore = GmGenUtils::GetValue(wl[1]);
  if( wl.size() == 3 ) {
    theScoreError = atof(wl[2])*theScore;
  }
  //  if( TextScoreData::verbosity >= 2 )
  //   G4cout << " TextScoreData " << this << " " << theName << G4endl;
 
  // get dispersion of flux values in different files
  theScoreSq = theScore*theScore;
  //  G4cout << theName << " score " << theScore << " sq " << theScoreSq  << " NFILES " << theNumberOfFiles<< G4endl;

  if( theScoreError != 0 ) {  
    theScoreDivideError = theScore/sqr(theScoreError);
    theInvErrorExp2 = 1./sqr(theScoreError);
  } else {
    theScoreDivideError = 0;
    theInvErrorExp2 = 0;
  }

  //  if( theName.find( "neutron_7" )!= std::string::npos)
  if( TextScoreData::verbosity >= 2 )
    G4cout << " TextScoreData " << theName << " : " << theScore << " +- " << theScoreError << " DivideError " << theScoreDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  theNumberOfFiles = 1;

} 

//----------------------------------------------------------------------
void TextScoreData::Print( std::ofstream& fout )
{
  TextScorerData* theScorerDataG = static_cast<TextScorerData*>(theScorerData);

  long double scoreAver = 0.;
  if( theInvErrorExp2 != 0 ) scoreAver = theScoreDivideError/theInvErrorExp2;
  /*  long double scoreError;
      if( scoreAver != 0. ) {
    scoreError = 1./sqrt(theInvErrorExp2);
  } else {
    scoreError = 0.;
    } */
  //  G4int nFiles =theNumberOfFiles; 
  G4int nFiles = theScorerDataG->GetNumberOfFiles();
  long double scoreDisp = 0.;
  if( nFiles == 1 || theScore == 0. || scoreDisp/theScore < -0. ) { // precision problems
    scoreDisp = 0.;
  } else {
    scoreDisp = theScoreSq*nFiles - sqr(theScore); // theScore and theScoreSq are already divided by nFiles
    scoreDisp = sqrt( (scoreDisp) / (nFiles-1));
  } 
  //   G4cout << theName << " scoreDisp0 " << scoreDisp << " score " << theScore << " sq " << theScoreSq << " NFILES " << nFiles << G4endl;

  //  G4cout << " scoreDisp " << theScoreSq/nFiles << " " << sqr(theScore/nFiles) << " " << nFiles << G4endl;
  if( TextScoreData::verbosity >= 2 ) 
    G4cout << "   TextScoreData::Print : " << theName << " scoreDisp " << scoreDisp << " N " << nFiles << " score " << theScore << " scoreSq " << theScoreSq << " theScoreSq/nFiles - sqr(theScore/nFiles) " << theScoreSq/nFiles - sqr(theScore/nFiles) << G4endl;

  if( !bTotal ) {
    fout << " index: ";
  } else {
    fout << theScorerDataG->GetScorerName() << " ";
  }
  //  double averScorePerEvent = theScoreTot/ScorerData::GetNEventsSum(theScorerDataG->GetName()); 
  double averScorePerEvent = theScore; 
  double errorRel2 = 0;
  if( averScorePerEvent != 0. ) errorRel2 = scoreDisp/averScorePerEvent;
  double errorRel1 = 0.;
  if( scoreAver != 0 ) errorRel1 = theScoreError/scoreAver;
  fout << theName << "  = " 
       << averScorePerEvent
    //       << " " << theScoreTot
    //  << " / " << ScorerData::GetNEventsSum(theScorerData->GetScorerName()) 
       << " +-(REL) "
       << errorRel1
       << " +-(REL_N) " 
       << errorRel2
       << " weighted_average " 
       << scoreAver;
  //  fout << " SCORE_dispersion(REL) " << scoreDisp/scoreAver;
  fout << G4endl;
    //       << "  " << scoreDisp << " N " << nFiles << " score " << theScore << " scoreSq " << theScoreSq << "  theScore*theScore/nFiles " << theScore*theScore/nFiles ;
 
  //  fout << " non_weighted_average " << theScore/ScorerData::GetNEventsSum(theScorerData->GetScorerName())  << " score " << theScore << " NEV " << ScorerData::GetNEventsSum(theScorerData->GetScorerName()) 

}
