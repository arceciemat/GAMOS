#include "DicomVerbosity.hh"

GmVerbosity DicomVerb;
#include "globals.hh"
/** Constructor   
 */
DicomVerbosity::DicomVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) )  G4cout << " DicomVerbosity " << DicomVerb.GetFilterLevel() << G4endl;  
#endif
  DicomVerb.SetFilterLevel(testVerb);
}


void DicomVerbosity::SetFilterLevel( int fl )
{
  DicomVerb.SetFilterLevel( fl );
}

int DicomVerbosity::GetFilterLevel() const
{
  return DicomVerb.GetFilterLevel();
}
