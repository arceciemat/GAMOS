#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

GmVerbosity UAVerb;
#include "globals.hh"


GmUAVerbosity::GmUAVerbosity(){
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUAVerbosity " << UAVerb.GetFilterLevel() << G4endl;
#endif
}


void GmUAVerbosity::SetFilterLevel( int fl )
{
#ifndef GAMOS_NO_VERBOSE
  //  G4cout << " GmUAVerbosity SetFilterLevel " << fl << " old " << UAVerb.GetFilterLevel() << G4endl;
#endif
  UAVerb.SetFilterLevel( fl );
}

int GmUAVerbosity::GetFilterLevel() const
{
 return UAVerb.GetFilterLevel();
}
