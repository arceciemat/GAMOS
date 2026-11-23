#ifndef GmStopRunAfterTimeUA_hh
#define GmStopRunAfterTimeUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
class G4Timer;

class GmStopRunAfterTimeUA : public GmUserEventAction
{
public:
  GmStopRunAfterTimeUA();
  ~GmStopRunAfterTimeUA(){}; 
  virtual void EndOfEventAction( const G4Event* );

private:
  G4double theTimeToStop;
  G4double theCurrentTime;
  G4Timer* theTimer;

};

#endif
