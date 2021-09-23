#include "MCNPFileData.hh"
#include "MCNPTallierFNData.hh"
#include "MCNPTallierF5Data.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4tgrUtils.hh"
#include <vector>

//----------------------------------------------------------
MCNPFileData::MCNPFileData( const G4String fileName)
{
  theFileName = fileName; 
  theNEvents = 1;

  GmFileIn fin = GmFileIn::GetInstance(fileName);

  //  bool bReadingTally = false;
  //  G4int Reading1Energy = 0;
  ScorerData* tallier = 0;
  std::vector<G4String> wl;
  std::set<G4String> theModifiedTallies;

  G4String tallierName = "";
  for( ;; ){
    if( ! fin.GetWordsInLine(wl) ) break;
    if( ScoreData::verbosity >= 3 )  G4cout << " WL " << wl.size() << " " << wl[0] << G4endl;
    // look for tally start
    //    if( !bReadingTally ) {
      if( wl.size() >= 4 
	  && wl[0] == "1tally" && wl[2] == "nps" ) {
	tallierName = wl[1];
	// 1tally  15        nps =     1000
	G4int type = GmGenUtils::GetInteger(wl[1])%10;
	//--- read lines until start of flux values
	if( type == 5 ) {	  
	  tallier = new MCNPTallierF5Data(wl, fin, theModifiedTallies.find(tallierName) != theModifiedTallies.end() );
	} else if( type == 1 ||
		   type == 2 ||
		   type == 4 ||
		   type == 6 ) {
	  tallier = new MCNPTallierFNData(wl, fin, theModifiedTallies.find(tallierName) != theModifiedTallies.end() );
	} else { 
	  G4Exception("MCNPFileData::MCNPFileData",
		      "Error",
		      FatalException,
		      ("tally type not supported " 
		       + GmGenUtils::itoa(type) 
		       + ", contact author").c_str());
	}
	theScorerData.push_back(tallier);

      } else if( wl[0].substr(0,2) == "DE" || wl[0].substr(0,2) == "DF" ) {
	tallierName = wl[0].substr(2,999);
	if( GmGenUtils::IsInt(tallierName) ) {
	  theModifiedTallies.insert( tallierName );
	}

	/*   } else if( wl[0] == "uncollided" ){
	if( ! fin.GetWordsInLine(wl) ) break;
	if( wl.size() != 1 || wl[0] != "energy" ) {
	  G4Exception("MCNPFileData::MCNPFileData",
		      "Error",
		      FatalException,
		      ("line starting uncollided scores should be 'energy', while it is " + wl[0]).c_str());
	}
	for( ;; ){       
	  if( ! fin.GetWordsInLine(wl) ) break;
	  t	  MCNPTallyData* tallyData = new MCNPTallyData(wl);
	  tallier->AddScorerData(wl);
	  if( wl[0] == "total" ) break;
	  
	  //  G4bool bNotFinal = tallier->AddUncollidedTallyData(wl);
	  //	  if( !bNotFinal ) break;
	  } */

	// score contribution  by cell
      } else if( wl.size() >= 7 && wl[0] == "number" && wl[2] == "nonzero" ){
	MCNPTallierFNData* tallierF4 = static_cast<MCNPTallierFNData*>(tallier);
	if( tallierF4 ) {
	  tallierF4->SetNonZeroHist( GmGenUtils::GetInteger(wl[6]) );
	}
      } else if( wl.size() >= 4 && wl[0] == "run" && wl[1] == "terminated" && wl[2] == "when" ){
	if( wl.size() == 8 ) {
	  theNEvents = GmGenUtils::GetInteger(wl[3]);
	} else { 	// when1000000000
	  G4String evtStr = wl[2].substr(4,wl[2].length());
	  theNEvents = GmGenUtils::GetInteger(evtStr);
	}
	// check if the theNEvents is not set for a tallier at construction because too big :  nps =*********
	std::vector<ScorerData*>::iterator ites; 
	for( ites = theScorerData.begin(); ites !=  theScorerData.end(); ites++ ){
	  if( (*ites)->GetNEvents() == 0 ) {
	    (*ites)->SetNEvents(theNEvents);
	  }
	}
	//      }else {
	//	if( wl.size() == 4 ) 	G4cout << " CONTRI " << wl.size() << " " << wl[0] << " " << wl[1] << G4endl;
      }
      //}
  } 

  //  G4cout << " ENDED MCNPFILE DATA " << G4endl; 
  /*  std::vector<ScorerData*>::iterator itet;
  for( itet = theScorerData.begin(); itet != theScorerData.end(); itet++ ){
    (*itet)->SetNEvents( theNEvents );
    } */

}
 

//----------------------------------------------------------
void MCNPFileData::Print( std::ofstream&  )
{
  /*  PrintHStar("neutron", fout);
  PrintHStar("photon", fout);
  PlotHistograms("neutron", true );
  PlotHistograms("photon", true ); //LOG
    //    file->PlotHistograms("photon", false ); /LIN
    */
  
}


/*//----------------------------------------------------------
void MCNPFileData::BuildHStar( G4String particle, G4String fNameDose, G4int interp )
{
  std::vector<ScorerData*>::iterator itet;
  for( ite = theScorerData.begin(); ite != theScorerData.end(); ite++ ) {
    if( (*ite)->GetParticle() == particle ) (*ite)->BuildHStar( fNameDose, interp );
  }
}

//----------------------------------------------------------
void MCNPFileData::PrintHStar( G4String particle, std::ofstream& fout )
{
  std::vector<MCNPTallyData*>::const_iterator ite;
  for( ite = theScorerData.begin(); ite != theScorerData.end(); ite++ ) {
    if( (*ite)->GetParticle() == particle ) (*ite)->PrintHStar( fout );
  }
}


//----------------------------------------------------------
MCNPTallyData* MCNPFileData::GetTally( G4String tallyName ) const
{
  std::vector<MCNPTallyData*>::const_iterator itet;
  for( itet = theScorerData.begin(); itet != theScorerData.end(); itet++) {
    if( (*itet)->GetName() == tallyName ) {
      return (*itet);
    }
  }

  for( itet = theScorerData.begin(); itet != theScorerData.end(); itet++) {
    G4cerr << " TALLY " << (*itet)->GetName() << G4endl;
  }
  G4Exception("MCNPFileData::GetTally", 
	      "Error",
	      FatalException,
	      ("No tally found with name " + tallyName + " in file " + theName).c_str());

  return (MCNPTallyData*)0;
}
*/

