#include "GmUserEventAction.hh"
#include "GmUserActionMgr.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

GmUserEventAction::GmUserEventAction()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserEventAction::GmUserEventAction() " << G4endl;
#endif
  GmUserActionMgr::GetInstance()->RegisterEventAction( this );
}

GmUserEventAction::~GmUserEventAction() 
{
}
