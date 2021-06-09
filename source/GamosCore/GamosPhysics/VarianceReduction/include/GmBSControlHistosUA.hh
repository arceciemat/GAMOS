#ifndef __GmBSControlHistosUA__
#define __GmBSControlHistosUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class G4Run;
class G4Step;
class GmAnalysisMgr;
#include <map>

class GmBSControlHistosUA : public GmUserRunAction, public GmUserSteppingAction
{
public:
  GmBSControlHistosUA();
  ~GmBSControlHistosUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
 
private:
  void BookHistos();

  G4bool IsSecondaryGamma(const G4Track* aTrack );

  GmAnalysisMgr* theAnaMgr;

};

#endif
