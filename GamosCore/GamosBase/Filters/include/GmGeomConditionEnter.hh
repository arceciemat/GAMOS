#ifndef GmGeomConditionEnter_h
#define GmGeomConditionEnter_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of entering particle (if in GeomBoundary)
// 
///////////////////////////////////////////////////////////////////////////////

#include "GmVGeomCondition.hh"

class GmGeomConditionEnter : public GmVGeomCondition
{
public: // with description
  GmGeomConditionEnter(){};
  
  virtual ~GmGeomConditionEnter(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);
  
};
#endif
