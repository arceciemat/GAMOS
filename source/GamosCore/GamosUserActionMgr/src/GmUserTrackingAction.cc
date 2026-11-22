#include "GmUserTrackingAction.hh"
#include "GmUserActionMgr.hh"

#include <iostream>

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"


GmUserTrackingAction::GmUserTrackingAction()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserTrackingAction::GmUserTrackingAction() " << G4endl;
#endif
  GmUserActionMgr::GetInstance()->RegisterTrackingAction( this );
}

GmUserTrackingAction::~GmUserTrackingAction() 
{
}
