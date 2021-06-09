// GmUserSteppingAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserSteppingAction_h
#define GmUserSteppingAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
#include "G4SteppingManager.hh"
class G4Step;
#include "G4TrackVector.hh"

class GmUserSteppingAction : public virtual GmUserAction
{
 public:
  GmUserSteppingAction();
  virtual ~GmUserSteppingAction();
  
  virtual void UserSteppingAction(const G4Step* ) = 0;

  G4TrackVector GetStepSecondaries();
  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetSteppingManager( G4SteppingManager* fptm ){
    fpSteppingManager = fptm; }

protected:
  G4SteppingManager* fpSteppingManager;

};

#endif
