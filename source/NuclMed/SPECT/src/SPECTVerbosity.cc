#include "SPECTVerbosity.hh"

GmVerbosity SPECTVerb;
#include "globals.hh"
/** Constructor   
 */
SPECTVerbosity::SPECTVerbosity(){
  SPECTVerb.SetFilterLevel(silentVerb);
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(infoVerb) ) G4cout << " SPECTVerbosity " << SPECTVerb.GetFilterLevel() << G4endl;
#endif
}


void SPECTVerbosity::SetFilterLevel( int fl )
{
  SPECTVerb.SetFilterLevel( fl );
  GmVVerbosity::SetVerbosityLevel( "DetVerbosity", fl );
}

int SPECTVerbosity::GetFilterLevel() const
{
 return SPECTVerb.GetFilterLevel();
}
