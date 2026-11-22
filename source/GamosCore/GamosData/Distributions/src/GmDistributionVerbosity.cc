#include "GmDistributionVerbosity.hh"

GmVerbosity DistVerb;
#include "globals.hh"
/** Constructor   
 */
GmDistributionVerbosity::GmDistributionVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(infoVerb) ) G4cout << " GmDistributionVerbosity " << DistVerb.GetFilterLevel() << G4endl;
#endif
}


void GmDistributionVerbosity::SetFilterLevel( int fl )
{
  DistVerb.SetFilterLevel( fl );
}

int GmDistributionVerbosity::GetFilterLevel() const
{
 return DistVerb.GetFilterLevel();
}
