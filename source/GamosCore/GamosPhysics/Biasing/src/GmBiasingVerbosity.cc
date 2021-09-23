#include "GmBiasingVerbosity.hh"

GmVerbosity BiasingVerb;
#include "globals.hh"
/** Constructor   
 */
GmBiasingVerbosity::GmBiasingVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(infoVerb) ) G4cout << " GmBiasingVerbosity " << BiasingVerb.GetFilterLevel() << G4endl;
#endif
}


void GmBiasingVerbosity::SetFilterLevel( int fl )
{
  BiasingVerb.SetFilterLevel( fl );
}

int GmBiasingVerbosity::GetFilterLevel() const
{
 return BiasingVerb.GetFilterLevel();
}
