#ifndef SHPrintEMDataUA_h
#define SHPrintEMDataUA_h

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmChangeEnergies.hh"
#include <map>
#include <vector>

class G4EmCalculator;

class SHPrintEMDataUA : public GmUserRunAction,
			     public GmUserEventAction,
			     public GmVHistoBuilder,
			     public GmChangeEnergies
{
  public:
  SHPrintEMDataUA();
  ~SHPrintEMDataUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfRunAction(const G4Run*);

private:

private:
  G4String thePrimaryParticleName;

  G4EmCalculator* theEmCalculator;
};
#endif
