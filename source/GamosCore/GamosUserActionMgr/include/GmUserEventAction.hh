// GmUserEventAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserEventAction_h
#define GmUserEventAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
#include "G4EventManager.hh"
class G4Event;

class GmUserEventAction : public virtual GmUserAction
{
 public:
  GmUserEventAction();
  virtual ~GmUserEventAction();
  
  virtual void BeginOfEventAction(const G4Event* ){};
  virtual void EndOfEventAction(const G4Event* ){};

  //  virtual void DefineName( const G4String& ) = 0;

public:
  void SetEventManager( G4EventManager* fptm ){
    fpEventManager = fptm; }

protected:
  G4EventManager* fpEventManager;

};

#endif
