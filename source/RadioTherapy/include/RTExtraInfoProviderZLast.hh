#ifndef __RTExtraInfoProviderZLast__
#define __RTExtraInfoProviderZLast__


#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "RTVExtraInfoProviderFloat.hh"

class RTExtraInfoProviderZLast : public GmUserSteppingAction, public RTVExtraInfoProviderFloat
{
public:
  RTExtraInfoProviderZLast();
  ~RTExtraInfoProviderZLast(){}; 
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void CheckZStop( G4double zstop );

private:
  G4double theWordPrev;

};

#endif
