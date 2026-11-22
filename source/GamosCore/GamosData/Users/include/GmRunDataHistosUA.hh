#ifndef GmRunDataHistosUA_hh
#define GmRunDataHistosUA_hh

#include "GmVDataUserHistos.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmRunDataHistosUA : public GmVDataUserHistos,
			    public GmUserRunAction,
			    public GmUserSteppingAction

{
public:
  GmRunDataHistosUA();
  ~GmRunDataHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:

};

#endif
