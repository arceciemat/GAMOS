#ifndef GmVGeomCondition_h
#define GmVGeomCondition_h 1

#include "globals.hh"
class G4VTouchable;
class G4Track;
class G4Step;
class G4StepPoint;

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Base class to manage geometry conditions for GmVolumeConditionFilter
// 
///////////////////////////////////////////////////////////////////////////////

class GmVGeomCondition 
{
public: // with description
  GmVGeomCondition(){};
  
  virtual ~GmVGeomCondition(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*) = 0;
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*) = 0;
  const G4VTouchable* GetTouchableFromStepPoint(const G4StepPoint*);
  
};

#endif
