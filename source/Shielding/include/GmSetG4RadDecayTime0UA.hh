//
#ifndef GmSetG4RadDecayTime0UA_hh
#define GmSetG4RadDecayTime0UA_hh
#include <map>
class G4ParticleDefinition;

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include <set>

class SHRadDecayChainSet;

class GmSetG4RadDecayTime0UA : public GmUserSteppingAction
{
public:
  GmSetG4RadDecayTime0UA();
  ~GmSetG4RadDecayTime0UA(){};
  virtual void UserSteppingAction(const G4Step* aStep );

private:
  void SetTime( const G4Step* aStep );

};

#endif
