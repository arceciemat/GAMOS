#ifndef GmHistosGammaAtSD_hh
#define GmHistosGammaAtSD_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "GamosCore/GamosAnalysis/include/GmTrajectorySteps.hh"
class GmAnalysisMgr;
class G4Event;
class GmCheckOriginalGamma;
class GmEventClassifierByGammaInteraction;
#include <map>

class GmHistosGammaAtSD : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  GmHistosGammaAtSD();
  ~GmHistosGammaAtSD(){};
  virtual void EndOfRunAction( const G4Run* aRun );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
 
  virtual void UserSteppingAction(const G4Step* aStep);
 
private:
 G4bool IsInterestingStep(const G4Step* aStep);
  void FillHistos();  
 
private:
  GmAnalysisMgr* theAnaMgr;

  G4bool bOneStepInSD;
  GmEventClassifierByGammaInteraction* theEventClassifier;
  GmTrajectorySteps theInteractions;

  G4int isOriginalGamma;
  GmCheckOriginalGamma* theCheckOriGamma;
  G4bool bRAYL, bCONV;
  G4int cRAYL, cCONV;

  G4int theNGammasInSD;
  std::map<G4int,G4int> cGammasInSD;
  std::map<G4int,G4int> cClassification;

  std::vector<G4double> theEnteringEnergies;
 
};

#endif
