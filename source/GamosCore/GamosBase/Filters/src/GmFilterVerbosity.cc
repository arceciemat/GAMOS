#include "GmFilterVerbosity.hh"

GmVerbosity FilterVerb;
#include "globals.hh"
/** Constructor   
 */
GmFilterVerbosity::GmFilterVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(infoVerb) ) G4cout << " GmFilterVerbosity " << FilterVerb.GetFilterLevel() << G4endl;
#endif
}

void GmFilterVerbosity::SetFilterLevel( int fl )
{
  FilterVerb.SetFilterLevel( fl );
}

int GmFilterVerbosity::GetFilterLevel() const
{
 return FilterVerb.GetFilterLevel();
}
