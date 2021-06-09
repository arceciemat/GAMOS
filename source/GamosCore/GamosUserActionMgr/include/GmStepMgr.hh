#ifndef GmStepMgr_h
#define GmStepMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmStepMgr    
Author:      P. Arce
Changes:     12/01: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Manager of copies of G4Step, for filtering in the future  */ 
//----------------------------------------------- 

#include <vector>
#include <map>
#include <set>
#include "globals.hh"
class GmFutureFilter;
class G4Step;
class G4StepPoint;
class G4Track;
class G4TouchableHistory;
typedef std::multimap<G4int,G4Step*> mmis;

//------------------------------------------------------------------------
class GmStepMgr 
{ 
private:
  GmStepMgr();

public:
  static GmStepMgr* GetInstance();
  ~GmStepMgr(){};

  void SaveStep( const G4Step* aStep );
  G4Track* SaveTrack( const G4Track* aTrack );
  std::vector<G4Step*> GetSteps( GmFutureFilter* filter, const G4Step* currentStep );
  G4Step* GetAncestorStep( const G4Step* currentStep );

  void ClearObjects();

private:
  static GmStepMgr* theInstance;

protected:
  std::multimap<G4int,G4Step*> theNewSteps; // store track ID for easier ancestor list building
  std::vector<G4StepPoint*> theNewStepPoints;
  std::map<G4int,G4Track*> theNewTracks;
  std::vector<G4TouchableHistory*> theNewTouchables;
  std::map<G4int,G4Step*> theCreatorStep;

};

#endif
