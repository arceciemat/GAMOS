#ifndef GmRecHitsWriteUA__hh
#define GmRecHitsWriteUA__hh

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include <iostream>
class GmVRecHitsIOMgr;

class GmRecHitsWriteUA : public GmUserEventAction
{
public:
  GmRecHitsWriteUA();
  ~GmRecHitsWriteUA(){};
  virtual void EndOfEventAction(const G4Event* anEvent);
 
private:
 GmVRecHitsIOMgr* theRecHitsIOMgr;
  G4bool bBinFile;
};

#endif
