#include "GmUserStackingAction.hh"
#include "GmUserActionMgr.hh"

#include <iostream>

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"


GmUserStackingAction::GmUserStackingAction()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserStackingAction::GmUserStackingAction() " << G4endl;
#endif
  GmUserActionMgr::GetInstance()->RegisterStackingAction( this );
}

GmUserStackingAction::~GmUserStackingAction() 
{
}
