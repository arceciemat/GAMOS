#ifndef KillNegativeLengthUA_hh
#define KillNegativeLengthUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <set>
#include <map>
#include <iostream>
#include "GmSSData.hh"

class KillNegativeLengthUA : public GmUserSteppingAction
{
public:
  KillNegativeLengthUA();
  ~KillNegativeLengthUA(){};
  virtual void UserSteppingAction( const G4Step* aStep );

};

#endif
