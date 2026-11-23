#include "GmVGeomCondition.hh"
#include "G4VTouchable.hh"
#include "G4StepPoint.hh"

const G4VTouchable* GmVGeomCondition::GetTouchableFromStepPoint(const G4StepPoint* sp)
{
  return sp->GetTouchable();
}
