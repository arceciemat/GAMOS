#ifndef GmStackDataHistosUA_hh
#define GmStackDataHistosUA_hh

#include "GmVDataUserHistos.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"

class GmStackDataHistosUA : public GmVDataUserHistos,
			    public GmUserRunAction,
			    public GmUserStackingAction
{
public:
  GmStackDataHistosUA();
  ~GmStackDataHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* );

  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();

private:

};

#endif
