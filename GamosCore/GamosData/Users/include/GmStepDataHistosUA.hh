#ifndef GmStepDataHistosUA_hh
#define GmStepDataHistosUA_hh

#include "GmVDataUserHistos.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

#include <set>

class GmStepDataHistosUA : public GmVDataUserHistos,
			   public GmUserSteppingAction,
			   public GmUserRunAction
{
public:
  GmStepDataHistosUA();
  ~GmStepDataHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:

};

#endif
