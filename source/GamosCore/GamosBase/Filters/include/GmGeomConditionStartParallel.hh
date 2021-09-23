#ifndef GmGeomConditionStartParallel_h
#define GmGeomConditionStartParallel_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed
// 
///////////////////////////////////////////////////////////////////////////////
#include "G4ParallelWorldScoringProcess.hh"

#include "GmVGeomCondition.hh"
class G4TouchableHistory;
class G4Navigator;

class GmGeomConditionStartParallel : public GmVGeomCondition
{
public: // with description
  GmGeomConditionStartParallel();
  
  virtual ~GmGeomConditionStartParallel(){delete theTouchable;};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;
  G4Navigator* theNavigator;

  G4ParallelWorldScoringProcess* theParallelWorldProcess;
  
};
#endif
