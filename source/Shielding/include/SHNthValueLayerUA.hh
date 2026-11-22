///////////////////////////////////////////////////////////////////////////////
// File : SHNthValueLayerUA.h
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef SHNthValueLayerUA_h
#define SHNthValueLayerUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include "globals.hh"

#include <map>
#include <set>
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class SHNthValueLayerUA : public GmUserRunAction, 
				  public GmUserEventAction, 
				public GmUserSteppingAction
{

 public:
  SHNthValueLayerUA();
  ~SHNthValueLayerUA();

 public:
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );
  virtual void UserSteppingAction(const G4Step* aStep);

 private:
  std::map< G4int,G4int> theNSteps;

  std::set<G4int> theLayers;

  std::vector<G4double> theReductions;

  G4double theLayerWidth;
  G4String thePrimaryParticleName;
  G4double thePrimaryEnergy;

  G4int theNEvents;

};

#endif
