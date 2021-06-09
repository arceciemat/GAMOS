///////////////////////////////////////////////////////////////////////////////
// File : GmSaveStepUA.h
// Author: P.Arce  12.09.01
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef GmSaveStepUA_h
#define GmSaveStepUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "globals.hh"

#include <string>
#include <map>

class G4Event;
class G4Track;
class G4Step;

class GmSaveStepUA : public GmUserEventAction, 
		     public GmUserTrackingAction, 
		     public GmUserSteppingAction
{

 public:
  GmSaveStepUA();
  ~GmSaveStepUA();

 public:
 virtual void EndOfEventAction(const G4Run* );
 virtual void PreUserTrackingAction(const G4Track* aTrack);
 virtual void PostUserTrackingAction(const G4Track* aTrack);
 virtual void UserSteppingAction(const G4Step* aStep);


};

#endif
