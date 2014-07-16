#ifndef GmGeomConditionTraverseParallel_h
#define GmGeomConditionTraverseParallel_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed
// 
///////////////////////////////////////////////////////////////////////////////
#define private public
#include "G4ParallelWorldScoringProcess.hh"

#include "GmVGeomCondition.hh"
class G4TouchableHistory;

class GmGeomConditionTraverseParallel : public GmVGeomCondition
{
public: // with description
  GmGeomConditionTraverseParallel();
  
  virtual ~GmGeomConditionTraverseParallel(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;

  G4ParallelWorldScoringProcess* theParallelWorldProcess;
  
};
#endif
