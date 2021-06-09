#include "GmAnalysisVerbosity.hh"

GmVerbosity AnaVerb;
#include "globals.hh"
/** Constructor   
 */
GmAnalysisVerbosity::GmAnalysisVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( AnaVerb(infoVerb) ) G4cout << " GmAnalysisVerbosity " << AnaVerb.GetFilterLevel() << G4endl;
#endif
}


void GmAnalysisVerbosity::SetFilterLevel( int fl )
{
  AnaVerb.SetFilterLevel( fl );
}

int GmAnalysisVerbosity::GetFilterLevel() const
{
 return AnaVerb.GetFilterLevel();
}
