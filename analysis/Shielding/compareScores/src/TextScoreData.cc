#include "TextScoreData.hh"
#include "ScorerData.hh"
#include "TextScorerData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
TextScoreData::TextScoreData(std::vector<G4String> wl) 
  : ScoreData()
{
  //  theName = wl[0];
  theName = GmGenUtils::ftoa(GmGenUtils::GetValue(wl[0]));
  //  G4cout <<  " THENAME " << theName << " " << GmGenUtils::GetValue(wl[0]) << " " << wl[0] << G4endl; //GDEB
  theDose = GmGenUtils::GetValue(wl[1]);
  if( wl.size() == 3 ) {
    theDoseError = atof(wl[2]);
  } else{ 
    theDoseError = 0.;
  }
  //  if( TextScoreData::verbosity >= 2 )
  //   G4cout << " TextScoreData " << this << " " << theName << G4endl;
 
  // get dispersion of flux values in different files
  theDoseSq = theDose*theDose;
  //  G4cout << theName << " dose " << theDose << " sq " << theDoseSq  << " NFILES " << theNumberOfFiles<< G4endl;

  if( theDoseError != 0 ) {  
    theDoseDivideError = theDose/sqr(theDoseError);
    theInvErrorExp2 = 1./sqr(theDoseError);
  } else {
    theDoseDivideError = 0;
    theInvErrorExp2 = 0;
  }

  //  if( theName.find( "neutron_7" )!= std::string::npos)
  if( TextScoreData::verbosity >= 2 )
    G4cout << " TextScoreData " << theName << " : " << theDose << " +- " << theDoseError << " DivideError " << theDoseDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  theNumberOfFiles = 1;

} 

//----------------------------------------------------------------------
void TextScoreData::Print( std::ofstream& fout )
{
  TextScorerData* theScorerDataG = static_cast<TextScorerData*>(theScorerData);

  long double doseAver = 0.;
  if( theInvErrorExp2 != 0 ) doseAver = theDoseDivideError/theInvErrorExp2;
  /*  long double doseError;
      if( doseAver != 0. ) {
    doseError = 1./sqrt(theInvErrorExp2);
  } else {
    doseError = 0.;
    } */
  //  G4int nFiles =theNumberOfFiles; 
  G4int nFiles = theScorerDataG->GetNumberOfFiles();
  long double doseDisp = 0.;
  if( nFiles == 1 || theDose == 0. || doseDisp/theDose < -0. ) { // precision problems
    doseDisp = 0.;
  } else {
    doseDisp = theDoseSq*nFiles - sqr(theDose); // theDose and theDoseSq are already divided by nFiles
    doseDisp = sqrt( (doseDisp) / (nFiles-1));
  } 
  //   G4cout << theName << " doseDisp0 " << doseDisp << " dose " << theDose << " sq " << theDoseSq << " NFILES " << nFiles << G4endl;

  //  G4cout << " doseDisp " << theDoseSq/nFiles << " " << sqr(theDose/nFiles) << " " << nFiles << G4endl;
  if( TextScoreData::verbosity >= 2 ) 
    G4cout << "   TextScoreData::Print : " << theName << " doseDisp " << doseDisp << " N " << nFiles << " dose " << theDose << " doseSq " << theDoseSq << " theDoseSq/nFiles - sqr(theDose/nFiles) " << theDoseSq/nFiles - sqr(theDose/nFiles) << G4endl;

  if( !bTotal ) {
    fout << " index: ";
  } else {
    fout << theScorerDataG->GetName() << " ";
  }
  //  double averDosePerEvent = theDoseTot/ScorerData::GetNEventsSum(theScorerDataG->GetName()); 
  double averDosePerEvent = theDose; 
  double errorRel2 = 0;
  if( averDosePerEvent != 0. ) errorRel2 = doseDisp/averDosePerEvent;
  double errorRel1 = 0.;
  if( doseAver != 0 ) errorRel1 = theDoseError/doseAver;
  fout << theName << "  = " 
       << averDosePerEvent
    //       << " " << theDoseTot
    //  << " / " << ScorerData::GetNEventsSum(theScorerData->GetName()) 
       << " +-(REL) "
       << errorRel1
       << " +-(REL_N) " 
       << errorRel2
       << " weighted_average " 
       << doseAver;
  //  fout << " DOSE_dispersion(REL) " << doseDisp/doseAver;
  fout << G4endl;
    //       << "  " << doseDisp << " N " << nFiles << " dose " << theDose << " doseSq " << theDoseSq << "  theDose*theDose/nFiles " << theDose*theDose/nFiles ;
 
  //  fout << " non_weighted_average " << theDose/ScorerData::GetNEventsSum(theScorerData->GetName())  << " dose " << theDose << " NEV " << ScorerData::GetNEventsSum(theScorerData->GetName()) 

}
