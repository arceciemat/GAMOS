#ifndef SHPrintPHPChannelXSUA_h
#define SHPrintPHPChannelXSUA_h

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

class SHPrintPHPChannelXSUA : public GmUserRunAction,
			     public GmUserEventAction,
			     public GmVHistoBuilder,
			     public GmChangeEnergies
{
  public:
  SHPrintPHPChannelXSUA();
  ~SHPrintPHPChannelXSUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfRunAction(const G4Run*);

private:
  //  void BookHistos();
  void InitialiseProcessSecoParticles();

private:
  G4String thePrimaryParticleName;

  G4bool bNormaliseToTotalXS;
  G4bool bOnlyTotalXS;
  G4bool bPrintXS0;
  G4bool bYields;

  G4int theProcIdxMax;
  G4int theMateIdxMax;
  G4int theIsotIdxMax;
  G4int theProdIdxMax;
  G4int theProductMaxAZ;

};
#endif
