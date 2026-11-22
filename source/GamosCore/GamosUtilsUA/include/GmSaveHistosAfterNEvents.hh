#ifndef __GmSaveHistosAfterNEvents__
#define __GmSaveHistosAfterNEvents__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

class G4Run;
class G4Event;
class G4Track;

class GmSaveHistosAfterNEvents : public GmUserRunAction, 
			public GmUserEventAction
{
public:
  GmSaveHistosAfterNEvents();
  ~GmSaveHistosAfterNEvents(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );

private:
  G4int theEachNEvent;
  G4int theFirstEvent;

};

#endif
