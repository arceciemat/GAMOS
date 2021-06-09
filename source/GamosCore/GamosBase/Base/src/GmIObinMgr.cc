#include "GmIObinMgr.hh"
#include "GmParameterMgr.hh"
#include "GmBaseVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <iomanip>
#include <iostream>

//-----------------------------------------------------------------------
GmIObinMgr::GmIObinMgr()
{
  Init();
}

//-----------------------------------------------------------------------
GmIObinMgr::~GmIObinMgr()
{
  CloseFileIn();
  CloseFileOut();
}

//-----------------------------------------------------------------------
void GmIObinMgr::Init()
{
  //  theFile = new std::ofstream(theFileName,std::ios::binary|std::ios::app);
  theFileIn = 0;
  theFileOut = 0;

  InitDebugging();
}

//-----------------------------------------------------------------------
void GmIObinMgr::OpenFileIn( const G4String& fname )
{
  if( theFileIn != 0 ){
    G4Exception("GmIObinMgr::OpenFileIn",
		"Wrong argument",
		FatalErrorInArgument,
		"File already opened");
  } 

  SetFileNameIn( fname );
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  theFileNameIn = GmGenUtils::FileInPath( path, theFileNameIn );

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " GmIObinMgr::OpenFileIn(): " << theFileNameIn << G4endl;
#endif
  theFileIn = fopen(theFileNameIn,"rb");
  /* use ofstream ..
  if( !theFileIn->is_open() ) {
    //  if( !theFileIn ) {
    G4Exception("GmIObinMgr::OpenFileIn",
		"File not opened ",
		FatalErrorInArgument,
		G4String("FILE: " + fname).c_str());
  }
  */
}

//-----------------------------------------------------------------------
void GmIObinMgr::OpenFileOut( const G4String& fname )
{
  if( theFileOut != 0 ){
    G4Exception("GmIObinMgr::OpenFileOut",
		"Wrong argument",
		FatalErrorInArgument,
		"File already opened");
  }

  SetFileNameOut( fname );

  theFileOut = fopen(theFileNameOut,"wb");
  if( !theFileOut ) {
    G4Exception("GmIObinMgr::OpenFileOut",
		"Wrong filename",
		FatalErrorInArgument,
		(G4String("File cannot be opened ") + theFileNameOut).c_str());
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " GmIObinMgr::OpenFileOut(): file name " << theFileNameOut << G4endl;
#endif
}

//-----------------------------------------------------------------------
void GmIObinMgr::CloseFileIn()
{
  if( theFileIn ) {
    fclose( theFileIn );
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " GmIObinMgr::CloseFileIn() file closed " << theFileNameIn << G4endl;
#endif
  }
  theFileIn = 0;
}

//-----------------------------------------------------------------------
void GmIObinMgr::CloseFileOut()
{
  if( theFileOut ) {
    fclose( theFileOut );
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " GmIObinMgr::CloseFileOut() file closed " << theFileNameOut << G4endl;
#endif
  }
  theFileOut = 0;
}


//-----------------------------------------------------------------------
void GmIObinMgr::OpenFileOutBin( const G4String& fname )
{
  if( theFileOut != 0 ){
    G4Exception("GmIObinMgr::OpenFileOut",
		"Wrong argument",
		FatalErrorInArgument,
		"File already opened");
  }

  SetFileNameOut( fname );

  theFileOutBin = new std::ofstream(theFileNameOut, std::ios::out | std::ios::binary);

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " GmIObinMgr::OpenFileOut(): file name " << theFileNameOut << G4endl;
#endif
}

//----------------------------------------------------------------
void GmIObinMgr::WriteHeaderCheck(std::ofstream* fOut)
{
  //  G4String endianChar;
  char endianChar[1];
  if( GmGenUtils::IsLittleEndian() ) {
    endianChar[0] = 'L';
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " MACHINE WORKS IN LITTLE ENDIAN " << G4endl;
#endif
  } else {
    endianChar[0] = 'B';
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " MACHINE WORKS IN BIG ENDIAN " << G4endl;
#endif
  }
  fOut->write(endianChar, sizeof(char));

  int wint = 1234567890;
  fOut->write( (char*)&wint, sizeof(int) );

  //  float wfloat = 3.4028234e+38;
  float wfloat = 3.40282e+38;
  fOut->write( (char*)&wfloat, sizeof(float) );

  //  double wdouble = 1.79769313486231490980915042343e+308;
  double wdouble = 1.79769e+308;
  //  wdouble = DBL_MAX;
  //  G4cout << " WDOUBLE " << wdouble <<" " <<  wdouble-DBL_MAX << G4endl;
  fOut->write( (char*)&wdouble, sizeof(double) );

}
