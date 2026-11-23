#ifndef GmNoUseG4RadDecayTimePrimaryUA_hh
#define GmNoUseG4RadDecayTimePrimaryUA_hh
#include <map>
class G4ParticleDefinition;

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmNoUseG4RadDecayTimePrimaryUA :   public GmUserSteppingAction,
			     public GmUserRunAction
{
public:
  GmNoUseG4RadDecayTimePrimaryUA();
  ~GmNoUseG4RadDecayTimePrimaryUA(){};
  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction(const G4Step* aStep );

private:

};

#endif
