#include "MCNPVTallierData.hh"
#include "MCNPTallyData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//------------------------------------------------------------------------
MCNPVTallierData::MCNPVTallierData(std::vector<G4String>& wl, G4bool bMod )
{
  bModified = bMod;
  theName = "tally " + wl[1];
  if( wl.size() == 5 ) {
    SetNEvents( GmGenUtils::GetValue(wl[4]) ); // total sum is calculated for each type of scorer name
  } else {
    G4String evtStr = wl[3].substr(1,wl[3].length());
    if( evtStr == "*********" ) {
      SetNEvents( 0 );
    } else {
      SetNEvents( GmGenUtils::GetValue(evtStr) ); 
    }
  } 
}

//------------------------------------------------------------------------
G4String MCNPVTallierData::GetIndexVal( G4String scoreName, G4bool )
{

  return scoreName;

}

//------------------------------------------------------------------------
void MCNPVTallierData::CreateTallys( std::vector<G4String> wl, std::vector<G4String> wl2 )
{
  std::vector<G4String> cosNames;
  std::vector<G4double> tallyValues;
  std::vector<G4double> tallyErrors;
  for( size_t ii = 1; ii < wl.size(); ii++ ) {
    cosNames.push_back( wl[ii] );
  }
  for( size_t ii = 0; ii < wl2.size(); ii+=2 ) {
    //    G4cout << wl.size() << " " << wl2.size() << " TALLY VAL " << wl2[ii] << " +- " << wl2[ii+1] << G4endl; //GDEB
    tallyValues.push_back( GmGenUtils::GetValue(wl2[ii]) );
    tallyErrors.push_back( GmGenUtils::GetValue(wl2[ii+1]) );
    //    G4cout << " TALLY VAL2 " << wl2[ii] << " +- " << wl2[ii+1] << G4endl; //GDEB
  }
  if( cosNames.size() != tallyValues.size() ) {
    G4Exception("MCNPVTallierData::CreateTallys",
		"",
		FatalException,
		(G4String("number of cosines = " + GmGenUtils::itoa(cosNames.size()) + " <> number of tally values = " + GmGenUtils::itoa(tallyValues.size()) + " Contact GAMOS authors").c_str()));
  }
  for( size_t ii = 0; ii < cosNames.size(); ii++ ) {
    MCNPTallyData* tallyData = new MCNPTallyData(cosNames[ii], tallyValues[ii], tallyErrors[ii]);
    AddScoreData(tallyData);	  
  }
}

//------------------------------------------------------------------------
void MCNPVTallierData::CreateTotalTally()
{
  G4double tallyVal = 0.;
  G4double tallyErr = 0.;

  std::vector<ScoreData*>::const_iterator ite;
  for( ite = theScoreData.begin(); ite != theScoreData.end(); ite++ ) {
    tallyVal += (*ite)->GetScore();
    tallyErr += sqr( (*ite)->GetScoreError() * (*ite)->GetScore() );
  }
  tallyErr /= tallyVal;
  MCNPTallyData* tallyData = new MCNPTallyData("SUM_ALL:", tallyVal, tallyErr);
  AddScoreData(tallyData);	  
}

//------------------------------------------------------------------------
void MCNPVTallierData::ReadData( std::vector<G4String>& wl, GmFileIn& fin ) 
{

  int nLines = 0;
  for( ;; nLines++) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( CheckData( wl ) ) {
    } else if( wl[0] == "tally" && wl[1] == "type" ) {
      if( ScoreData::verbosity >= 2 ) G4cout << "MCNPFileData: read type " << theScoreType << G4endl;
      // check type 
      /*      if( GmGenUtils::GetInteger(wl[2]) != theType ) {
	G4Exception("MCNPVTallierData::MCNPVTallierData",
		    "Error",
		    FatalException,
		    (theName + "Wrong type " + wl[2] + ", while it should be " + GmGenUtils::itoa(theType)).c_str());
		    } */
    } else if( wl[0] == "particle(s):" ) {
      theParticle = wl[1];
      if( ScoreData::verbosity >= 2 ) G4cout << "MCNPFileData: read particle " << wl[1] << G4endl;
    } else if( !bModified && wl.size() == 1 && wl[0] == "energy" ){
      // read scores
      if( ScoreData::verbosity >= 2 ) G4cout << "MCNPFileData: read start scores " << G4endl;
      for( ;; ){       
	if( ! fin.GetWordsInLine(wl) ) break;
	MCNPTallyData* tallyData = new MCNPTallyData(wl);
	AddScoreData(tallyData);	  
	if( wl[0] == "total" ) {
	  SetNScores( GetScoreDataSize()-1 );
	  //-	  G4cout  << " MCNPTALLIERFNDATA total " << G4endl; //GDEB
	  break;
	}
      }
      
      break; // stop after all score values are read

    } else if( !bModified && wl[0] == "cosine:" ){

      std::vector<G4String> wl2;  
      //--- if cosine or 2D: cosine-energy tally
      if( ! fin.GetWordsInLine(wl2) ) break;
      if( wl2[0] != "energy" ) {
	if( ScoreData::verbosity >= 2 ) G4cout << "MCNPFileData: read start scores cosine " << G4endl;
	CreateTallys(wl,wl2);
	
	for( ;; ){       
	  if( ! fin.GetWordsInLine(wl) ) break;
	  if( wl[0] != "cosine:" ) {
	    //-- create "total" tally
	    CreateTotalTally();
	    SetNScores( GetScoreDataSize()-1 );
	    break;
	  }
	  if( ! fin.GetWordsInLine(wl2) ) break;
	  if( wl[0] == "total" ) {
	    SetNScores( GetScoreDataSize()-1 );
	    break;
	    /*	  } else if( wl[0] == "1analysis" ) { // "total" not found
	    //-- create "total" tally
	    CreateTotalTally();
	    SetNScores( GetScoreDataSize()-1 );
	    break; */
	  }
	  CreateTallys(wl,wl2);
	}
      } else {
	
	//--- energy - cosine tally




      }
      break; // stop before starting to read values (read in MCNPFileData
      
    } else if( bModified && wl.size() == 2 && GmGenUtils::IsInt(wl[0]) && GmGenUtils::IsInt(wl[1]) ){
      wl.insert( wl.begin(), "0" ); // add a tally of energy = 0
      //      AddTally(wl);
    } else {
      if( nLines > 20 ) {
	G4Exception("MCNPVTallierData",
		    "",
		    JustWarning,
		    "Reading more than 20 lines and no tally data found, stopping");
	break;
      }
    }
  }

}
