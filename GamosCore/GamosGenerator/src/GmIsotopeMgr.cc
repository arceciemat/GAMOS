
#include "GamosCore/GamosGenerator/include/GmIsotopeMgr.hh"
#include "GamosCore/GamosGenerator/include/GmIsotope.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeSource.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeDecay.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorMessenger.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
GmIsotopeMgr* GmIsotopeMgr::theInstance = 0;

//----------------------------------------------------------------------
GmIsotopeMgr* GmIsotopeMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmIsotopeMgr;
  }

  return theInstance;

}

//----------------------------------------------------------------------
void GmIsotopeMgr::ReadIsotopes( const G4String& filename )
{
  char* pathc = getenv( "GAMOS_SEARCH_PATH" );
  if( !pathc ) G4Exception("GmIsotopeMgr::ReadIsotopes",
			   "Wrong argument",
			   FatalErrorInArgument,
			   "No GAMOS_SEARCH_PATH variable defined, please define it as in config/confgamos.sh or config/confgamos.csh");
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  GmFileIn fin = GmFileIn::GetInstance( GmGenUtils::FileInPath( path, filename ));
  ReadIsotopes( fin );
}

//----------------------------------------------------------------------
void GmIsotopeMgr::ReadIsotopes( GmFileIn& fin )
{
  std::vector<G4String> wl;
  GmIsotope* currentIsotope;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    for( unsigned int ii = 0; ii < wl.size(); ii++ ) {
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(infoVerb) ) G4cout << " : " << wl[ii];
#endif
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << G4endl;
#endif

    if( wl[0] == ":ISOTOPE" ) {
      if( wl.size() != 2 ) {
	G4cerr << " GmIsotopeMgr::ReadIsotopes line without 2 words " << wl.size() << ": " << G4endl;
	for( unsigned int ii = 0; ii < wl.size(); ii++ ) {
	  G4cout << wl[ii] << " ";
	}
	G4cout << G4endl;
	G4Exception("GmIsotopeMgr::ReadIsotopes",
		    "Wrong argument",
		    FatalErrorInArgument,
		    "");
      }
      
      currentIsotope = new GmIsotope( wl[1] );
      theIsotopes.push_back( currentIsotope );
    } else {
      if( wl.size() != 3 ) {
	G4cerr << " GmIsotopeMgr::ReadIsotopes line without 3 words " << wl.size() << ": " << G4endl;
	for( unsigned int ii = 0; ii < wl.size(); ii++ ) {
	  G4cout << wl[ii] << " ";
	}
	G4cout << G4endl;
	G4Exception("GmIsotopeMgr::ReadIsotopes",
		    "Wrong argument",
		    FatalErrorInArgument,
		    "");
      }
      GmIsotopeDecay* dec = new GmIsotopeDecay( wl[0], wl[1], wl[2] );
      currentIsotope->AddDecay( dec );
    }
  }

}


//----------------------------------------------------------------------
GmIsotopeSource* GmIsotopeMgr::AddActiveIsotopeSource( const G4String& sourceName, const G4String& isoName, const G4double activity )
{
  GmIsotope * iso = GetIsotope( isoName );
  if( iso == 0 ) G4Exception("GmIsotopeMgr::AddActiveIsotope",
			     "Wrong argument",
			     FatalErrorInArgument,
			     G4String("Isotope not found " + isoName ).c_str());
  GmIsotopeSource* isoSource = new GmIsotopeSource( sourceName, iso, activity );
  //  theActiveIsotopes.push_back( isoSource );
  return isoSource;
}

/*
//----------------------------------------------------------------------
void GmIsotopeMgr::SetIsotopeActivity( double Activity )
{
  std::vector<GmIsotopeSource*>::const_iterator ite;
  for( ite = theActiveIsotopes.begin(); ite != theActiveIsotopes.end(); ite++ ){
    (*ite)->SetActivity( Activity );
  }
}
*/

//----------------------------------------------------------------------
GmIsotope* GmIsotopeMgr::GetIsotope( const G4String& isoName )
{
  std::vector<GmIsotope*>::const_iterator ite;
  GmIsotope* isotope = 0;
  for( ite = theIsotopes.begin(); ite != theIsotopes.end(); ite++ ){
    if( (*ite)->GetName() == isoName ){
      isotope = *ite;
    }
  }

  if( isotope == 0 ) {
    G4cerr << " !! WARNING GmIsotopeMgr::GetIsotope: isotope not found "<< isoName << G4endl;
  }

  return isotope;
}
