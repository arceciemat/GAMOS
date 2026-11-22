#ifndef __GmPDSScoringTrackInfo_hh
#define __GmPDSScoringTrackInfo_hh

#include "GamosCore/GamosUtils/include/GmWeightTrackInfo.hh"
#include "G4Point3D.hh"
#include <map>

class GmPDSScoringTrackInfo : public GmWeightTrackInfo
{
public:
  GmPDSScoringTrackInfo(G4String& procName, G4Point3D& point ){
    theProcessName = procName; 
    thePoint = point;
  }
  ~GmPDSScoringTrackInfo(){}

  virtual void Print() const{};

  G4String GetProcessName() const {
    return theProcessName; }
  void SetProcessName( G4String procName ) {
    theProcessName = procName; }
  G4Point3D GetPoint() const { 
    return thePoint; }
  void SetPoint( G4Point3D& pt ) {
    thePoint = pt;
  }

private:
  G4String theProcessName;
  G4Point3D thePoint;
};

#endif
