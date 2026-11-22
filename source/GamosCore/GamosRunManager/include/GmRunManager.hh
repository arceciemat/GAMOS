#ifndef GmRunManager_H
#define GmRunManager_H

#include "G4RunManager.hh"
class GmSDMessenger;
class GmAnalysisMessenger;
class GmRunMessenger;

class GmRunManager
{
public:
  GmRunManager();
  virtual ~GmRunManager();

  static GmRunManager* GetRunManager();

  void Initialise();
  virtual void CreateG4RunManager();

  // Multithreading: called by GmActionInitialization::Build() in each worker thread
  void BuildForWorker();
  // Creates the GAMOS messengers; on worker threads only those whose commands
  // must be replayed per thread (user actions, generator, SD, scoring, analysis...)
  void CreateMessengers( G4bool bMaster );

  void SelectGeometry( const G4String& );
  void SelectPhysicsList( const G4String& );
  void SelectGenerator( const G4String& );
  void SelectUserAction( const G4String& );
  void SelectUserTimeStepAction( const G4String& values );
  void SelectVerbosity( const G4String& );
  void SetRandomSeeds( G4int index, G4int nTries );
  void RestoreRandomSeeds( G4int index, G4int nTries );
  // MT: the G4MTRunManager/G4TaskRunManager seeds the workers with the engine it had when created;
  // after CLHEP::HepRandom::setTheEngine it must be told about the new engine
  static void UpdateMTMasterRandomEngine();

  G4RunManager* GetG4RunManager() const {
    return theG4RunManager;
  }

protected:
  void InitialiseFactories();

protected:
  static GmRunManager* theRunManager;
  G4RunManager* theG4RunManager;

  G4String theGeometryName;
  G4String thePhysicsListName;
  G4String theGeneratorName;

  GmSDMessenger* theGmSDMessenger;
  GmAnalysisMessenger* theGmAnalysisMessenger;
  GmRunMessenger* theGmRunMessenger;

  G4double theNSeedsInFile;
  G4bool bRestoreSeedsUsed;
  
};


#endif
