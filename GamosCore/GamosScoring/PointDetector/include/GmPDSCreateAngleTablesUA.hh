#ifndef GmPDSCreateAngleTablesUA_h
#define GmPDSCreateAngleTablesUA_h

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include <set>
class G4Material;

class GmPDSCreateAngleTablesUA : public GmUserRunAction, 
				 public GmUserEventAction,
				 public GmUserSteppingAction
{
  public:
  GmPDSCreateAngleTablesUA();
  ~GmPDSCreateAngleTablesUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  void BookHistos(G4String energyName, G4String mateName );
  G4Track* GetSecondaryTrackSameType();
  void Initialise( const G4Event* evt );

private:
  G4String thePrimaryParticleName;
  G4String theSecondaryParticleName;

  GmAnalysisMgr* theAnaMgr;

  std::map<G4String,int> theProcesses;

  G4double theLastEnergy;
  G4Material* theLastMaterial;
  G4int theCurrentEnergyID;
  G4int theCurrentMateID;
  std::map<G4double,int> theEnergyIDs;
  std::map<G4Material*,int> theMaterialIDs;

  const G4ProcessVector* theProclis;
  G4bool bInitialised;

  std::set<G4String> theHistos;
};
#endif
