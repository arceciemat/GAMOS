#ifndef RTPlanSubControlPoint_H
#define RTPlanSubControlPoint_H
#include "globals.hh"
#include <map>
#include <vector>

class RTPlanControlPoint;
#include "RTVPlanSubControlPoint.hh"

class RTPlanSubControlPoint : public RTVPlanSubControlPoint
{
public:
  RTPlanSubControlPoint( G4double ms, RTPlanControlPoint* CP);
  ~RTPlanSubControlPoint(){}

  virtual RTBeamStateData GetBeamStateData();

private:

  G4double thePosX;
  G4double thePosY;
};

#endif // RTPlanSubControlPoint_H
