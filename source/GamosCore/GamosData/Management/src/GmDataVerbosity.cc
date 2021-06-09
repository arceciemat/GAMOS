#include "GmDataVerbosity.hh"

GmVerbosity DataVerb;
#include "globals.hh"
/** Constructor   
 */
GmDataVerbosity::GmDataVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmDataVerbosity " << DataVerb.GetFilterLevel() << G4endl;
#endif
}


void GmDataVerbosity::SetFilterLevel( int fl )
{
  DataVerb.SetFilterLevel( fl );
}

int GmDataVerbosity::GetFilterLevel() const
{
 return DataVerb.GetFilterLevel();
}
