#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

GmVerbosity SDVerb;
#include "globals.hh"
/** Constructor   
 */
GmSDVerbosity::GmSDVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmSDVerbosity " << SDVerb.GetFilterLevel() << G4endl;
#endif
}


void GmSDVerbosity::SetFilterLevel( int fl )
{
  SDVerb.SetFilterLevel( fl );
}

int GmSDVerbosity::GetFilterLevel() const
{
 return SDVerb.GetFilterLevel();
}
