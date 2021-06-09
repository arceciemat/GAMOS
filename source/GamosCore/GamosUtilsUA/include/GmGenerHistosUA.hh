#ifndef GmGenerHistosUA_h
#define GmGenerHistosUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
class GmAnalysisMgr;

class GmGenerHistosUA : public GmUserEventAction, 
			public GmVHistoBuilder
{

 public:
  GmGenerHistosUA();
  ~GmGenerHistosUA();

  virtual void BeginOfEventAction(const G4Event* );
  virtual void InitialiseHistoNames();

 private:
  void BookHistos();

  G4double theTimeOld;
};

#endif
