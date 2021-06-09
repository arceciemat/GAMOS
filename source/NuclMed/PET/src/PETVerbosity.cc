#include "PETVerbosity.hh"

GmVerbosity PETVerb;
#include "globals.hh"
/** Constructor   
 */
PETVerbosity::PETVerbosity(){
  PETVerb.SetFilterLevel(silentVerb);
#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(infoVerb) ) G4cout << " PETVerbosity " << PETVerb.GetFilterLevel() << G4endl;
#endif
}


void PETVerbosity::SetFilterLevel( int fl )
{
  PETVerb.SetFilterLevel( fl );
  GmVVerbosity::SetVerbosityLevel( "DetVerbosity", fl );
}

int PETVerbosity::GetFilterLevel() const
{
 return PETVerb.GetFilterLevel();
}
