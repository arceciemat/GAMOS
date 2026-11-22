#include "GmGeomVerbosity.hh"

GmVerbosity GeomVerb;
#include "globals.hh"
/** Constructor   
 */
GmGeomVerbosity::GmGeomVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << " GmGeomVerbosity " << GeomVerb.GetFilterLevel() << G4endl;
#endif
}


void GmGeomVerbosity::SetFilterLevel( int fl )
{
  GeomVerb.SetFilterLevel( fl );
}

int GmGeomVerbosity::GetFilterLevel() const
{
 return GeomVerb.GetFilterLevel();
}
