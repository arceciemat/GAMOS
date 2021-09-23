#ifndef GmPSEnergyDeposit_VPrimaryGamma1stInter_h
#define GmPSEnergyDeposit_VPrimaryGamma1stInter_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "G4THitsMap.hh"
#include <set>
class G4Event;
class G4SteppingManager;
class G4VPhysicalVolume;
class G4EmCalculator;

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a base primitive scorer class for scoring energy deposit from a primary gamma first interaction, or their secondaries at first interaction
// 
///////////////////////////////////////////////////////////////////////////////

enum ExitingState {ESNotExited,ESExitingNow,ESExited};

class GmPSEnergyDeposit_VPrimaryGamma1stInter : public GmVPrimitiveScorer,
						  public GmUserEventAction, 
						  public GmUserTrackingAction,
						  public GmUserSteppingAction
{
 
 public: // with description
      GmPSEnergyDeposit_VPrimaryGamma1stInter(G4String name);
      virtual ~GmPSEnergyDeposit_VPrimaryGamma1stInter();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*) = 0;
  
public: 

  virtual G4double GetUnit() const {
    return theUnit; }
  
  virtual G4String GetUnitName() const {
    return theUnitName; }

  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PreUserTrackingAction(const G4Track* aTrack );
  virtual void EndOfEventAction(const G4Event* );
  
  G4bool IsFromGoodGamma(const G4Track* aTrack);
  virtual void SetParameters( const std::vector<G4String>& params );
  
protected:
  void ComputeProbAtt(const G4Step* aStep);
  void CheckProcessInCavity(const G4Step* aStep);
  
  std::set<G4int> theGoodSecondaries;
  std::vector<G4ThreeVector> theGoodSteps;

  G4bool bIsFirstInteraction;
  G4double thePrimaryGammaVtxEnergy;
  G4SteppingManager* fpSteppingManager;
  G4bool bCheckPrimary; // check if primary is gamma and there is only one
  G4bool bCountAllInteractions;
  std::set<G4VPhysicalVolume*> theChamberPVs;

  G4bool bComputeProbAtt;
  G4double theStepLength;
  G4double theProbAtt;
  G4EmCalculator* theEmCalculator;
  G4double theStepLengthTotal;
  G4double theDistanceToBorder;

  G4bool bBadIfHasExited;
  ExitingState theExitingState;
  G4bool bBadIfComptCavity; // Do not count it if it Compton in cavity
  G4bool bBadIfRaylCavity; //  Do not count it if it Rayleigh in cavity
  G4bool bBadIfPhotCavity; //  Do not count it if it PhotoElectric in cavity

};
#endif
