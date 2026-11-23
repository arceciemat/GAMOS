#ifndef GmHistosAtCavityUA_hh
#define GmHistosAtCavityUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmPSEnergyDeposit_VPrimaryGamma1stInter.hh" // theExitingState

class GmHistosAtCavityUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  GmHistosAtCavityUA();
  ~GmHistosAtCavityUA(){};
  virtual void EndOfRunAction( const G4Run* aRun );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
 
  virtual void UserSteppingAction(const G4Step* aStep);
 
private:
  void SetParameters();

  G4bool bCheckPrimary;

  std::map<size_t,size_t> theCounters;
  std::map<size_t,size_t> theCountersE1;
  std::map<size_t,size_t> theCountersE2;
  std::map<size_t,size_t> theCountersE12;
  ExitingState theExitingState;
  std::set<G4VPhysicalVolume*> theChamberPVs;
  std::set<G4VPhysicalVolume*> theCavityPVs;
  size_t theClass;
  size_t theNInteractions;
  G4double theEdepo1;
  G4double theEdepo2;
  G4double theEdepo12;
  
  std::set<G4int> theGoodSecondaries1;
  std::set<G4int> theGoodSecondaries2;
  std::vector<G4ThreeVector> theGoodSteps1;
  std::vector<G4ThreeVector> theGoodSteps2;

 
};

#endif
