// GmUserStackingAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserStackingAction_h
#define GmUserStackingAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
class G4Track;
#include "G4StackManager.hh"

class GmUserStackingAction : public virtual GmUserAction
{
 public:
  GmUserStackingAction();
  virtual ~GmUserStackingAction();
  
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) = 0;
  virtual void NewStage(){};
  virtual void PrepareNewEvent(){}; 

  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetStackManager( G4StackManager* fsm ){
    stackManager = fsm; 
  }
  
 protected:
  G4StackManager* stackManager;

};

#endif
