#include "GAMOSScoreData.hh"
#include "ScorerData.hh"
#include "GAMOSScorerData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
GAMOSScoreData::GAMOSScoreData(std::vector<G4String> wl, G4bool bTot) 
  : ScoreData()
{
  bTotal = bTot;
  if( !bTotal ) {
    theName = wl[1];
    theScore = atof(wl[3]);
    theScoreError = atof(wl[5])*theScore;
  }else {
    //    theName = "SUM ALL:";
    theName = "SUM_ALL:";
    unsigned int kk = 0;
    for(; kk < wl.size()-1; kk++ ) {
      //      if (wl[kk]=="SUM" && wl[kk+1] == "ALL:") break;
      if (wl[kk]=="SUM_ALL:") break;
    }
    
    theScore = atof(wl[kk+1]);
    theScoreError = atof(wl[kk+3])*theScore;
    //NEW TGHCuF: ALL: neutron: PD1: ALL: SUM ALL:  = 9.87432e-13 +-(REL) 1 cm-2   N 2
    //theScore = atof(wl[kk+3]);
    //theScoreError = atof(wl[kk+5])*theScore;
    //OLD TGHCuF: ALL: neutron: PD1: ALL: SUM ALL:  9.87432e-13 +-(REL) 1 cm-2   N 2
    //    theScore = atof(wl[kk+2]);
    //   theScoreError = atof(wl[kk+4])*theScore;

  }
  //  if( GAMOSScoreData::verbosity >= 2 )
  //   G4cout << " GAMOSScoreData " << this << " " << theName << G4endl;
 
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
  if( GAMOSScoreData::verbosity >= 2 )
    G4cout << " GAMOSScoreData " << theName << " : " << theScore << " +- " << theScoreError << " DivideError " << theScoreDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  theNumberOfFiles = 1;

  //GDEB
  // G4cerr << this <<  " " << wl.size();
  //  for( size_t ii = 0; ii < wl.size(); ii++ ) {
  //   G4cerr << " WL " << wl[ii];
  // } 
  // G4cerr << G4endl;
} 

//----------------------------------------------------------------------
void GAMOSScoreData::Print( std::ofstream& fout )
{
  GAMOSScorerData* theScorerDataG = static_cast<GAMOSScorerData*>(theScorerData);

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
    //    scoreDisp = theScoreSq/nFiles - sqr(theScore);
    scoreDisp = theScoreSq/nFiles - sqr(theScore/nFiles);
    scoreDisp = sqrt( (scoreDisp) / (nFiles-1));
  } 
  //    G4cout << theName << " scoreDisp0 " << scoreDisp << " score " << theScore << " sq " << theScoreSq << " NFILES " << nFiles << G4endl;

  //  G4cout << " scoreDisp " << theScoreSq/nFiles << " " << sqr(theScore/nFiles) << " " << nFiles << G4endl;
  if( GAMOSScoreData::verbosity >= 2 ) 
    G4cout << "   GAMOSScoreData::Print : " << theName << " scoreDisp " << scoreDisp << " N " << nFiles << " score " << theScore << " scoreSq " << theScoreSq << " theScoreSq/nFiles - sqr(theScore/nFiles) " << theScoreSq/nFiles - sqr(theScore/nFiles) << G4endl;

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
  fout << theName << " ";
  if( !bTotal ) fout << "= "; 
  fout << averScorePerEvent
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
