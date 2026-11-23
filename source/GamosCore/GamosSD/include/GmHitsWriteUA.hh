#ifndef GmHitsWriteUA__hh
#define GmHitsWriteUA__hh

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include <iostream>
class GmVHitsIOMgr;

class GmHitsWriteUA : public GmUserEventAction
{
public:
  GmHitsWriteUA();
  ~GmHitsWriteUA(){};
  virtual void EndOfEventAction(const G4Event* anEvent);
 
private:
  GmVHitsIOMgr* theHitsIOMgr;
  G4bool bBinFile;
};

#endif
