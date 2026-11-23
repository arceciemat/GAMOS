 #ifndef GmVChangeWorldAndSource_h
#define GmVChangeWorldAndSource_h

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
class G4PVPlacement;
#include <map>

class GmVChangeWorldAndSource : public GmUserEventAction, 
				public GmUserStackingAction
{
  public:
  GmVChangeWorldAndSource();
  ~GmVChangeWorldAndSource(){};
  virtual void BeginOfEventAction(const G4Event* evt);
  virtual void EndOfEventAction(const G4Event* evt);
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void InitialiseRun();

protected:
  void BuildWorlds();
  G4PVPlacement* BuildWorld(G4Material* mate);
  void BuildSources();
  virtual void ChangeWorldOrSource();
  virtual void ChangeWorld();
  virtual void ChangeSource();

protected:
  G4bool bInitialised;
  G4String thePrimaryParticleName;
  G4String theSecondaryParticleName;
  G4int theCurrentWorld;
  G4int theCurrentSource;
  G4double theNEventsPerRun;
  G4double theMinE, theMaxE;
  unsigned int theNstepsE;
  G4bool bLogE;
  std::map<G4Material*,G4PVPlacement*> theWorlds;
  std::map<G4double,G4String> theSourceCommands;
  G4bool bTooManyEvents;

};
#endif
