#include "ShieldingVerbosity.hh"

GmVerbosity SHVerb;
#include "globals.hh"
/** Constructor   
 */
ShieldingVerbosity::ShieldingVerbosity(){
  SHVerb.SetFilterLevel(silentVerb);
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(infoVerb) ) G4cout << " ShieldingVerbosity " << SHVerb.GetFilterLevel() << G4endl;
#endif
}


void ShieldingVerbosity::SetFilterLevel( int fl )
{
  SHVerb.SetFilterLevel( fl );
}

int ShieldingVerbosity::GetFilterLevel() const
{
 return SHVerb.GetFilterLevel();
}
