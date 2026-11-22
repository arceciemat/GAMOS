#ifndef SHPrintPHPChannelThickYieldUA_h
#define SHPrintPHPChannelThickYieldUA_h

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosBase/Base/include/GmChangeEnergies.hh"
#include <map>
#include <vector>

class G4EmCalculator;

class SHPrintPHPChannelThickYieldUA : public GmUserRunAction,
			     public GmUserEventAction,
			     public GmChangeEnergies
{
  public:
  SHPrintPHPChannelThickYieldUA();
  ~SHPrintPHPChannelThickYieldUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfRunAction(const G4Run*);

private:
  //  void BookHistos();
  void InitialiseProcessSecoParticles();

private:
  G4bool bPrintXS0;
  G4EmCalculator* theEmCalculator;

  std::map<G4Material*, std::map<G4String,G4double>* > theThickYields; // channel name '_' particle , thick yield

};
#endif
