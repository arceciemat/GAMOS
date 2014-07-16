#ifndef GmPDSUA_h
#define GmPDSUA_h

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GmPDSProcessHelper.hh"

#include <map>

class GmAnalysisMgr;
class GmPDSScore;
class GmPDSGeantinoProcess;
class GmPDSNeutronProcess;
class GmPDSGammaProcess;
class GmPDSDetector;
class GmPDSProcessHelper;

class GmPDSUA : public GmUserRunAction,
		public GmUserEventAction
{
  public:
  GmPDSUA();
  ~GmPDSUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfEventAction(const G4Event*);
  virtual void EndOfRunAction(const G4Run*);

private:
  void AddNeutronScoringProcess();
  void AddGammaScoringProcess();
  std::map<G4int,GmPDSDetector*> BuildDetectors();

private:
  GmPDSGeantinoProcess* theNeutronGeantinoScoringProcess;
  GmPDSNeutronProcess* theNeutronScoringProcess;
  GmPDSGeantinoProcess* theGammaGeantinoScoringProcess;
  GmPDSGammaProcess* theGammaScoringProcess;

  std::map<G4String,GmPDSScore*>* theScoresN;
  std::map<G4String,GmPDSScore*>* theScoresG;

  G4bool bInitialised;

  GmPDSProcessHelper* theNeutronHelper;
  GmPDSProcessHelper* theGammaHelper;

  G4bool bScoreNeutrons;
  G4bool bScoreGammas;

};
#endif
