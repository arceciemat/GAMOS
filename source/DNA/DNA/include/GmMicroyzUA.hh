#ifndef __GmMicroyzUA__
#define __GmMicroyzUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class G4Run;
class GmMicroyzSD;

class GmMicroyzUA : public GmUserRunAction
{
public:
  GmMicroyzUA(GmMicroyzSD* sd);
  ~GmMicroyzUA(){};
  virtual void EndOfRunAction(const G4Run* );

private:
  GmMicroyzSD* theSD;
};

#endif
