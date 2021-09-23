#ifndef __RTExtraInfoProviderZLastNoMaterial__
#define __RTExtraInfoProviderZLastNoMaterial__

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "RTVExtraInfoProviderFloat.hh"
#include <set>

class RTExtraInfoProviderZLastNoMaterial : public GmUserSteppingAction, public RTVExtraInfoProviderFloat
{
public:
  RTExtraInfoProviderZLastNoMaterial();
  ~RTExtraInfoProviderZLastNoMaterial(){}; 
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void CheckZStop( G4double zstop );

private:
  G4double theWordPrev;

  std::set<G4String> theExcludedMaterials;
};

#endif
