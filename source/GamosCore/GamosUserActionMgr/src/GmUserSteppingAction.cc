#include "GmUserSteppingAction.hh"
#include "GmUserActionMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <iostream>

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

//---------------------------------------------------------------------
GmUserSteppingAction::GmUserSteppingAction()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserSteppingAction::GmUserSteppingAction() " << G4endl;
#endif
  GmUserActionMgr::GetInstance()->RegisterSteppingAction( this );
}

//---------------------------------------------------------------------
GmUserSteppingAction::~GmUserSteppingAction() 
{
}

//---------------------------------------------------------------------
G4TrackVector GmUserSteppingAction::GetStepSecondaries()
{

  return GmG4Utils::GetSecondariesOfCurrentStep( fpSteppingManager );
}
