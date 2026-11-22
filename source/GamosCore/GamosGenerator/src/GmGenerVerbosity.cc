#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"

GmVerbosity GenerVerb;
#include "globals.hh"
/** Constructor   
 */
GmGenerVerbosity::GmGenerVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerVerbosity " << GenerVerb.GetFilterLevel() << G4endl;
#endif
}


void GmGenerVerbosity::SetFilterLevel( int fl )
{
  GenerVerb.SetFilterLevel( fl );
}

int GmGenerVerbosity::GetFilterLevel() const
{
 return GenerVerb.GetFilterLevel();
}
