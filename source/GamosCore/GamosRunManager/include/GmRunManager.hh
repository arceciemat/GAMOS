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

  void SelectGeometry( const G4String& );
  void SelectPhysicsList( const G4String& );
  void SelectGenerator( const G4String& );
  void SelectUserAction( const G4String& );
  void SelectVerbosity( const G4String& );
  void SetRandomSeeds( G4int index, G4int nTries );
  void RestoreRandomSeeds( G4int index, G4int nTries );

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
