#include "GmUserRunAction.hh"
#include "GmUserActionMgr.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"


GmUserRunAction::GmUserRunAction()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserRunAction::GmUserRunAction() " << G4endl;
#endif
  GmUserActionMgr::GetInstance()->RegisterRunAction( this );
}

GmUserRunAction::~GmUserRunAction() 
{
}
