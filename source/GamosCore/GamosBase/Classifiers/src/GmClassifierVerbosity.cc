#include "GmClassifierVerbosity.hh"

GmVerbosity ClassifierVerb;
#include "globals.hh"
/** Constructor   
 */
GmClassifierVerbosity::GmClassifierVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(infoVerb) ) G4cout << " GmClassifierVerbosity " << ClassifierVerb.GetFilterLevel() << G4endl;
#endif
}

void GmClassifierVerbosity::SetFilterLevel( int fl )
{
  ClassifierVerb.SetFilterLevel( fl );
}

int GmClassifierVerbosity::GetFilterLevel() const
{
 return ClassifierVerb.GetFilterLevel();
}
