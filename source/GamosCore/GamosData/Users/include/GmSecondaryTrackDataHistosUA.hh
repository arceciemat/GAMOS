#ifndef GmSecondaryTrackDataHistosUA_hh
#define GmSecondaryTrackDataHistosUA_hh

#include "GmVDataUserHistos.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmSecondaryTrackDataHistosUA : public GmVDataUserHistos, 
				     public GmUserRunAction,
				     public GmUserSteppingAction
{
public:
  GmSecondaryTrackDataHistosUA();
  ~GmSecondaryTrackDataHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction(const G4Run* );
  virtual void InitialiseHistoNames();
 
private:
  G4bool bClassifierOnSecondary;
};

#endif
