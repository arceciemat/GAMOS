#ifndef GmShowerShapeUA_hh
#define GmShowerShapeUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <set>
#include <map>
#include <iostream>
#include "GmSSData.hh"

enum  SSIT{ SSITPre, SSITPost,  SSITLinear };
  
class GmShowerShapeUA : public GmUserRunAction, 
			public GmUserSteppingAction, 
			public GmUserEventAction, 
			public GmVHistoBuilder
{
public:
  GmShowerShapeUA();
  ~GmShowerShapeUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event* );
  virtual void InitialiseHistoNames();

  GmSSData* CheckWhichShower( const G4Step* aStep );

private:
  void BookHistos(G4int index);

  G4ThreeVector GetShowerDirection( GmSSData* ssdata);
  void DumpText( const G4Event* evt  );
  void DumpBin( const G4Event* evt  );
 
private:
  std::map<G4int, GmSSData*> theShowerData;

  TSPUse thePointToUse;
  SSIT theInterpolationType;
  G4bool bShowerDirection1stTrack;
  G4bool bUsePostInFirstTrack;
  G4bool bIncludeOtherStepsOfFirstTrack;

  std::set<G4double> theRadii;

  std::set<G4int> theHistos;

  G4String theTextFileName;
  std::ostream* theTextFileOut;

  G4String theBinFileName;
  FILE* theBinFileOut;

};

#endif
