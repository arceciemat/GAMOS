#ifndef GmGeomConditionExitParallel_h
#define GmGeomConditionExitParallel_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed
// 
///////////////////////////////////////////////////////////////////////////////
#include "G4ParallelWorldScoringProcess.hh"

#include "GmVGeomCondition.hh"
class G4TouchableHistory;

class GmGeomConditionExitParallel : public GmVGeomCondition
{
public: // with description
  GmGeomConditionExitParallel();
  
  virtual ~GmGeomConditionExitParallel(){delete theTouchable;};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;

  G4ParallelWorldScoringProcess* theParallelWorldProcess;
  
};
#endif
