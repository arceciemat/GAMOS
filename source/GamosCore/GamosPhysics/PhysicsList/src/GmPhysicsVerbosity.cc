#include "GmPhysicsVerbosity.hh"

GmVerbosity PhysicsVerb;
#include "globals.hh"
/** Constructor   
 */
GmPhysicsVerbosity::GmPhysicsVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << " GmPhysicsVerbosity " << PhysicsVerb.GetFilterLevel() << G4endl;
#endif
}


void GmPhysicsVerbosity::SetFilterLevel( int fl )
{
  PhysicsVerb.SetFilterLevel( fl );
}

int GmPhysicsVerbosity::GetFilterLevel() const
{
 return PhysicsVerb.GetFilterLevel();
}
