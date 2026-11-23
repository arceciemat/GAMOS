#ifndef GmGeomConditionExit_h
#define GmGeomConditionExit_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of exiting particle (if in GeomBoundary)
// 
///////////////////////////////////////////////////////////////////////////////

#include "GmVGeomCondition.hh"

class GmGeomConditionExit : public GmVGeomCondition
{
public: // with description
  GmGeomConditionExit(){};
  
  virtual ~GmGeomConditionExit(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);
  
};
#endif
