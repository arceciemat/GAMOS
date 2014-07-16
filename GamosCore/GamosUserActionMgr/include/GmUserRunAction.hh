// GmUserRunAction:
// Author: P.Arce  02/03/2002
// Abstract base class for all CMS G4 user stepping actions

#ifndef GmUserRunAction_h
#define GmUserRunAction_h 1

#include "globals.hh"
#include "GmUserAction.hh"
class G4Track;

class GmUserRunAction : public virtual GmUserAction
{
 public:
  GmUserRunAction();
  virtual ~GmUserRunAction();
   
  virtual void BeginOfRunAction(const G4Run* ){};
  virtual void EndOfRunAction(const G4Run* ){};
  virtual G4Run* GenerateRun(){ return 0;};

  //  virtual void DefineName( const G4String& ) = 0;

};

#endif
