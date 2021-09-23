#include "globals.hh"
#include <vector>
#include<map>
#include "include/GmAOData.hh"
#include "include/GmAODataSearch.hh"
#include "G4tgrFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

void ReadAnalysisDescription( G4String& fname );
void ProcessMetaLine( const std::vector<G4String>& wl );
void ReadFiles();
void ProcessData();

std::vector<GmAODataSearch*> theDataSearches;
std::vector<G4String> theTxtFiles;
typedef std::multimap<GmAODataSearch*,GmAOData*> mmdsd;
//mmdsd theData;

#include "../include/GmAOParam.hh"

//-----------------------------------------------------------------------
int main(int argc,char** argv) 
{
  G4cout << "VERBOSE " << verbose << G4endl;
  //  new GmParameterMessenger;
  if(argc != 2 ) {
    G4Exception("analyseOutput",
		"Error in argument",
		FatalErrorInArgument,
		"You must ONE argument, the filename ");
  }
  G4String fname = argv[1];

  ReadAnalysisDescription( fname );

  ReadFiles();

  ProcessData();
}

//-----------------------------------------------------------------------
void ReadAnalysisDescription( G4String& fname )
{

  if( verbose > 1 ) G4cout << " ReadAnalysisDescription " << fname << G4endl;
/*  theTxtFilePrefix = "";
  theTxtFileSuffix = "";
  theRootFilePrefix = "";
  theRootFileSuffix = "";
*/

  G4tgrFileIn fin = G4tgrFileIn::GetInstance(fname);

  std::vector<G4String> wl;
  for(;;) {
    if(! fin.GetWordsInLine( wl ) ) break; 
    ProcessMetaLine( wl );
  }
}

//-----------------------------------------------------------------------
void ProcessMetaLine( const std::vector<G4String>& wl )
{
  if( verbose > 1 ) G4cout << " ProcessMetaLine " << wl.size() << G4endl;

  G4String wl0 = wl[0];
  /*  for( size_t ii = 0; ii < wl0.length(); ii++ ){
    wl0[ii] = toupper( wl0[ii] );
    } */

  //------------------------------- 
  if( wl0 == ":FILE_TXT" ) {
    //t    CheckNWords( wl, 2 );
    theTxtFiles.push_back( wl[1] );
    /*  } else if( wl0 == ":FILE_ROOT" ) {
    CheckNWords( wl, 2 );
    theRootFiles.push_back( wl[1] );
  } else if( wl0 == ":FILE_TXT_PREFIX" ) {
    CheckNWords( wl, 2 );
    theTxtFilePrefix = wl[1];
  } else if( wl0 == ":FILE_TXT_SUFFIX" ) {
    CheckNWords( wl, 2 );
    theTxtFileSuffix = wl[1];
  } else if( wl0 == ":FILE_ROOT_PREFIX" ) {
    CheckNWords( wl, 2 );
    theRootFilePrefix = wl[1];
  } else if( wl0 == ":FILE_ROOT_SUFFIX" ) {
    CheckNWords( wl, 2 );
    theRootFileSuffix = wl[1];
  } else if( wl0 == ":TXT" ) {
    CheckNWords( wl, 2 );
  } else if( wl0 == ":FILE_ROOT" ) {
  CheckNWords( wl, 2 ); */
  } else {
    theDataSearches.push_back( new GmAODataSearch(wl) );
  }

}


//-----------------------------------------------------------------------
void ReadFiles()
{
  if( verbose > 1 ) G4cout << " ReadFiles " << G4endl;

  //---- Loop to files
  std::vector<G4String> wl;
  unsigned int ndat = theDataSearches.size();
  for( unsigned int ii = 0; ii < theTxtFiles.size(); ii++ ){
    G4String fileName = theTxtFiles[ii];
    if( verbose >= 1 ) G4cout << " @@@ OPENING FILE " << fileName << G4endl;
    GmFileIn fin = GmFileIn::GetInstance(fileName);
    fin.SetSuppressQuotes(0);
    for(;;) {
      if(! fin.GetWordsInLine( wl ) ) break; 
      for( unsigned int jj = 0; jj < ndat; jj++ ){

	if ( wl.size() > 2 && wl[0]=="Number" &&  wl[1]=="of"  &&  wl[2]=="events") {
	  GmAOData::theNEvents[fileName] = GmGenUtils::GetValue (wl[5]);
	  //	G4cout << " NEvents " << nevents << G4endl; 
	}
	
	if( theDataSearches[jj]->CheckLine( wl, theTxtFiles[ii] ) ) {
	  theDataSearches[jj]->AddData( new GmAOData( wl, theDataSearches[jj], fileName ));
	  //	  theData.insert(std::multimap<GmAODataSearch*,GmAOData*>::value_type(
	  //				     				      theDataSearches[jj], new GmAOData( wl, theDataSearches[jj], fileName )));
	}
      }
    }
  }

}

//-----------------------------------------------------------------------
void ProcessData()
{
  if( verbose > 1 ) G4cout << " ProcessData " << G4endl;

  //---- loop to data searches
  std::vector<GmAODataSearch*>::const_iterator iteds;
  mmdsd::const_iterator itedsd;
  for(iteds = theDataSearches.begin(); iteds != theDataSearches.end(); iteds++ ){    
    (*iteds)->ProcessData();
  }

  exit(0);

}
