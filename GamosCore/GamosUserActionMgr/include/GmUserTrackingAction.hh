// GmUserTrackingAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserTrackingAction_h
#define GmUserTrackingAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
#include "G4TrackingManager.hh"
class G4Track;

class GmUserTrackingAction : public virtual GmUserAction
{
public:
  GmUserTrackingAction();
  virtual ~GmUserTrackingAction();
  
  virtual void PreUserTrackingAction(const G4Track* ){};
  virtual void PostUserTrackingAction(const G4Track* ){};

  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetTrackingManager( G4TrackingManager* fptm ){
    fpTrackingManager = fptm; }

protected:
  G4TrackingManager* fpTrackingManager;

};

#endif
