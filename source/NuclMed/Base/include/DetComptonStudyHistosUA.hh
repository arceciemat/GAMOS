#ifndef DetComptonStudyHistosUA_H
#define DetComptonStudyHistosUA_H

#include "globals.hh"
#include "G4ThreeVector.hh"
class GmAnalysisMgr;
class G4Event;
class G4Track;
class G4Step;
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "DetRecHitSet.hh"
#include <map>
class GmTrajStep;
class DetVComptAlgorithmVariable;
class DetVComptClassificationVariable;

typedef std::multimap<GmTrajStep*,GmRecHit*> mmiterh;                     

class DetComptonStudyHistosUA : public GmVHistoBuilder,
				public GmUserEventAction,
				public GmUserTrackingAction,
				public GmUserSteppingAction
{

public:
  DetComptonStudyHistosUA();
  virtual ~DetComptonStudyHistosUA();
  virtual void BeginOfEventAction(const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);
  void BookHistos();
  void FillHistos(G4int nh, std::vector<G4double>& vars);
  //  void PlotDist511RecHits();
  void FillDist511RecHits( DetRecHitSetList& rh511 );
 
private:
  void BookHistosForVariable(G4String var, G4double hlimMin, G4double hlimMax, G4int nh);
  void BookOtherHistos();
  G4int GetTrackID( GmRecHit* rhit );
  void AnalyseRecHitInteractionAssociation( mmiterh recHitOrderedTemp, DetRecHitSet rhitset );

private:
  G4int isOriginalGamma;
  typedef std::multimap<G4int,GmTrajStep*> mmits;
  mmits theInteractions;

  std::vector<DetVComptAlgorithmVariable*> theCAlgoVars;
  std::vector<DetVComptClassificationVariable*> theCClassifVars;

};

#endif

