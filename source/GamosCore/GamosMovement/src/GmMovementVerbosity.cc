#include "GmMovementVerbosity.hh"

GmVerbosity MoveVerb;
#include "globals.hh"
/** Constructor   
 */
GmMovementVerbosity::GmMovementVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( MoveVerb(infoVerb) ) G4cout << " GmMovementVerbosity " << MoveVerb.GetFilterLevel() << G4endl;
#endif
}


void GmMovementVerbosity::SetFilterLevel( int fl )
{
  MoveVerb.SetFilterLevel( fl );
}

int GmMovementVerbosity::GetFilterLevel() const
{
 return MoveVerb.GetFilterLevel();
}
