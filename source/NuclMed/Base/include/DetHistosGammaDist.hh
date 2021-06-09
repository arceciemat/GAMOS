#ifndef __DetHistosGammaDist__
#define __DetHistosGammaDist__

#include "GamosCore/GamosAnalysis/include/GmTrajPointPosMom.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class G4Event;
class GmAnalysisMgr;

class DetHistosGammaDist : public GmUserEventAction,  public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  DetHistosGammaDist();
  ~DetHistosGammaDist(){};
  virtual void EndOfEventAction( const G4Event* evt );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);
  
private:
  G4bool IsInterestingStep(const G4Step* aStep);
  void FillHistos();  
  G4ThreeVector ExtrapolateToRing( GmTrajPointPosMom& inter );

private:
  GmAnalysisMgr* theAnaMgr;

  std::map<int,GmTrajPointPosMom> theInteractions;
  G4ThreeVector theEventVertex;
  G4double theRingRadius;
  G4int theNgamma;

  G4int isOriginalGamma;

};

#endif
