#include "GmUtilsUAVerbosity.hh"

GmVerbosity UtilsUAVerb;
#include "globals.hh"
/** Constructor   
 */
GmUtilsUAVerbosity::GmUtilsUAVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(infoVerb) ) G4cout << " GmUtilsUAVerbosity " << UtilsUAVerb.GetFilterLevel() << G4endl;
#endif
}


void GmUtilsUAVerbosity::SetFilterLevel( int fl )
{
  UtilsUAVerb.SetFilterLevel( fl );
}

int GmUtilsUAVerbosity::GetFilterLevel() const
{
 return UtilsUAVerb.GetFilterLevel();
}
