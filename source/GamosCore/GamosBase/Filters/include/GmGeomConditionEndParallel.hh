#ifndef GmGeomConditionEndParallel_h
#define GmGeomConditionEndParallel_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed
// 
///////////////////////////////////////////////////////////////////////////////
#include "G4ParallelWorldScoringProcess.hh"

#include "GmVGeomCondition.hh"
class G4TouchableHistory;

class GmGeomConditionEndParallel : public GmVGeomCondition
{
public: // with description
  GmGeomConditionEndParallel();
  
  virtual ~GmGeomConditionEndParallel(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4ParallelWorldScoringProcess* theParallelWorldProcess;
  
};
#endif
