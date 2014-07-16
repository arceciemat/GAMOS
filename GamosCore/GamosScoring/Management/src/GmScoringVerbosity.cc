#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

GmVerbosity ScoringVerb;
#include "globals.hh"
/** Constructor   
 */
GmScoringVerbosity::GmScoringVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmScoringVerbosity " << ScoringVerb.GetFilterLevel() << G4endl;
#endif
}


void GmScoringVerbosity::SetFilterLevel( int fl )
{
  ScoringVerb.SetFilterLevel( fl );
}

int GmScoringVerbosity::GetFilterLevel() const
{
 return ScoringVerb.GetFilterLevel();
}
