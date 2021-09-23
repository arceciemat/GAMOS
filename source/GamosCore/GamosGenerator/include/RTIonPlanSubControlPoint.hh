#ifndef RTIonPlanSubControlPoint_H
#define RTIonPlanSubControlPoint_H
#include "globals.hh"
#include <map>
#include <vector>

class RTIonPlanControlPoint;
#include "RTVPlanSubControlPoint.hh"

class RTIonPlanSubControlPoint : public RTVPlanSubControlPoint 
{
public:
  RTIonPlanSubControlPoint( const std::vector<G4String> wl, RTIonPlanControlPoint* CP);
  ~RTIonPlanSubControlPoint(){}

  G4double GetPosX() const {
    return thePosX;
  }
  G4double GetPosY() const {
    return thePosY;
  }

  virtual RTBeamStateData GetBeamStateData();
  
private:
  G4double thePosX;
  G4double thePosY;
};

#endif // RTIonPlanSubControlPoint_H
