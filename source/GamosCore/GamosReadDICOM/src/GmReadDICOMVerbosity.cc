#include "GamosCore/GamosReadDICOM/include/GmReadDICOMVerbosity.hh"
#include "GmReadDICOMVerbosity.hh"

GmVerbosity ReadDICOMVerb;
#include "globals.hh"
/** Constructor   
 */
GmReadDICOMVerbosity::GmReadDICOMVerbosity(){
  /*#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << " GmReadDICOMVerbosity " << ReadDICOMVerb.GetFilterLevel() << G4endl;
#endif
  */
}


void GmReadDICOMVerbosity::SetFilterLevel( int fl )
{
  ReadDICOMVerb.SetFilterLevel( fl );
}

int GmReadDICOMVerbosity::GetFilterLevel() const
{
 return ReadDICOMVerb.GetFilterLevel();
}
