#include "CCVerbosity.hh"

GmVerbosity CCVerb;
#include "globals.hh"
/** Constructor   
 */
CCVerbosity::CCVerbosity(){
  CCVerb.SetFilterLevel(silentVerb);
#ifndef GAMOS_NO_VERBOSE
  if( CCVerb(infoVerb) ) G4cout << " CCVerbosity " << CCVerb.GetFilterLevel() << G4endl;
#endif
}


void CCVerbosity::SetFilterLevel( int fl )
{
  CCVerb.SetFilterLevel( fl );
  GmVVerbosity::SetVerbosityLevel( "DetVerbosity", fl );
}

int CCVerbosity::GetFilterLevel() const
{
 return CCVerb.GetFilterLevel();
}
