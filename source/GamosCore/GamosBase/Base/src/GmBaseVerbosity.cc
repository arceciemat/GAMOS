#include "GmBaseVerbosity.hh"

GmVerbosity BaseVerb;
#include "globals.hh"
/** Constructor   
 */
GmBaseVerbosity::GmBaseVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " GmBaseVerbosity " << BaseVerb.GetFilterLevel() << G4endl;
#endif
}

void GmBaseVerbosity::SetFilterLevel( int fl )
{
  BaseVerb.SetFilterLevel( fl );
}

int GmBaseVerbosity::GetFilterLevel() const
{
 return BaseVerb.GetFilterLevel();
}
