#include "GmDNAVerbosity.hh"

GmVerbosity DNAVerb;
#include "globals.hh"
/** Constructor   
 */
GmDNAVerbosity::GmDNAVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(infoVerb) ) G4cout << " GmDNAVerbosity " << DNAVerb.GetFilterLevel() << G4endl;
#endif
}


void GmDNAVerbosity::SetFilterLevel( int fl )
{
  DNAVerb.SetFilterLevel( fl );
}

int GmDNAVerbosity::GetFilterLevel() const
{
 return DNAVerb.GetFilterLevel();
}
