#ifndef __GmRandomByTimeUA__
#define __GmRandomByTimeUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

class G4Run;
class G4Event;
class G4Track;

class GmRandomByTimeUA : public GmUserEventAction
{
public:
  GmRandomByTimeUA();
  ~GmRandomByTimeUA(){};
  virtual void BeginOfEventAction(const G4Event* );

private:

};

#endif
