#include "DetVerbosity.hh"

GmVerbosity DetVerb;
#include "globals.hh"
/** Constructor   
 */
DetVerbosity::DetVerbosity(){
  DetVerb.SetFilterLevel(silentVerb);
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(infoVerb) ) G4cout << " DetVerbosity " << DetVerb.GetFilterLevel() << G4endl;
#endif
}


void DetVerbosity::SetFilterLevel( int fl )
{
  DetVerb.SetFilterLevel( fl );
}

int DetVerbosity::GetFilterLevel() const
{
 return DetVerb.GetFilterLevel();
}
