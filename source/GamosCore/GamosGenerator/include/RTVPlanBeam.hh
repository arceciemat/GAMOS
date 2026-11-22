#ifndef RTVPlanBeam_H
#define RTVPlanBeam_H
#include "RTVPlanControlPoint.hh"
#include "RTPlanParamReader.hh"
class RTVPlanSource;

class RTVPlanBeam : public RTPlanParamReader
{
public: 
  RTVPlanBeam(){}
  ~RTVPlanBeam(){}
  std::vector<RTVPlanControlPoint*> GetControlPoints() const {
    return theControlPoints;
  }
  G4int GetIndex() const {
    return theIndex;
  }

  G4int GetNoControlPoints() const {
    return theControlPoints.size();
  }
  RTVPlanControlPoint* GetControlPoint( G4int ii );
  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fn ) {
    theFileName = fn;
  }
  void SetSource( RTVPlanSource* sour ) {
    theSource = sour;
  }

  G4int GetCurrentControlPointIndex() const {
    return iCurrentControlPoint;
  }

protected:
  virtual void ReorderControlPoints() = 0;

  std::vector<RTVPlanControlPoint*> theControlPoints;

  G4int theIndex;
  G4int iCurrentControlPoint;
  G4String theFileName;
  RTVPlanSource* theSource;
};

#endif // RTVPlanBeam_H
