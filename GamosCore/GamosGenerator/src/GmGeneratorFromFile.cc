#include "GmGeneratorFromFile.hh"
#include "GmGenerVerbosity.hh"

#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "G4tgrFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmGeneratorFromFile::GmGeneratorFromFile()
{
  bInitialised = false;
}


void GmGeneratorFromFile::ReadIsotopeAtomicNumbers() 
{
  // Get atomic number from list of isotopes
  G4String fileName = "isotopes.geom";
  char* pathc = getenv( "GAMOS_SEARCH_PATH" );
  if( !pathc ) G4Exception("GmGeneratorFromFile::ReadIsotopeAtomicNumbers",
			   "Error",
			   FatalException,
			   "No GAMOS_SEARCH_PATH variable defined, please define it as in config/confgamos.sh or confgamos.csh");
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  //  G4tgrFileIn fin = G4tgrFileIn::GetInstance( GmGenUtils::FileInPath( path, fileName, "GmGeneratorFromFile::ReadIsotopeAtomicNumbers" ));
  G4tgrFileIn fin = G4tgrFileIn::GetInstance( GmGenUtils::FileInPath( path, fileName ));
  std::vector<G4String> wl;
  for( ;; ) {
    if( !fin.GetWordsInLine( wl ) ) break;
    if( wl[0] == ":ISOT" ) {
      G4String::size_type ic1 = wl[1].find("_");
      G4int icN;
      for( unsigned int ii = 0; ii < ic1; ii++ ) {
	if( GmGenUtils::IsNumber( wl[1][ii] ) ) {
	  icN = ii;
	  break;
	}
      }
      //      G4cout << " IC1 " << ic1 << " ICN " << icN << G4endl;
      theIsotNameZ[wl[1].substr(0,icN)] = G4int(GmGenUtils::GetValue(wl[2]));
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) )
      G4cout << " ISOT READ " << wl[1].substr(0,icN) << " : " << GmGenUtils::GetValue(wl[2])<< G4endl;
#endif
    }
  }
  fin.Close();
}
