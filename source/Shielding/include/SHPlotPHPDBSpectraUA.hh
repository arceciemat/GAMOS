#ifndef SHPlotPHPDBSpectraUA_hh
#define SHPlotPHPDBSpectraUA_hh

#include "SHPrintPHPChannelXSUA.hh"

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmChangeEnergies.hh"
#include <map>
#include <vector>

class G4PVPlacement;
class G4HadronicProcess;
class G4Material;
class GmAnalysisMgr;
class G4ParticleDefinition;

class SHPlotPHPDBSpectraUA : public GmUserRunAction,
			     public GmUserEventAction,
			     public GmVHistoBuilder,
			     public GmChangeEnergies
{
  public:
  SHPlotPHPDBSpectraUA();
  ~SHPlotPHPDBSpectraUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfRunAction(const G4Run*);

private:
  //  void BookHistos();
private:
  G4String thePrimaryParticleName;

  G4int theNEvents;
  G4int theMateIdxMax;
  G4int theIsotIdxMax;
  G4int theProdIdxMax;
  G4int theProcIdxMax;
  G4int theProductMaxAZ;

};
#endif
