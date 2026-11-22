#ifndef GmHitsReadUA__hh
#define GmHitsReadUA__hh

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include <iostream>
class GmHitsIOtextMgr;
class GmHitsIObinMgr;

class GmHitsReadUA : public GmUserEventAction
{
public:
  GmHitsReadUA();
  ~GmHitsReadUA(){};
  virtual void BeginOfEventAction(const G4Event* anEvent);
 
private:
  // GmHitsIObinMgr* theHitsIOMgr;
 GmHitsIOtextMgr* theHitsIOMgr;

};

#endif
