#ifndef GmTrajectory_H
#define GmTrajectory_H

#include "GmVTrajectory.hh"
class GmTrajPoint;

class GmTrajectory : public GmVTrajectory
{
public:
  GmTrajectory();
  GmTrajectory(const G4Track* aTrack);
  virtual ~GmTrajectory();
 
  void Clear();

  void AddPoint( GmTrajPoint* point ){
    thePoints.push_back( point ); }
 
  // Get/Set functions 
  const std::vector<GmTrajPoint*> GetPoints() const{
    return thePoints;}
  const GmTrajPoint* GetPoint(G4int ipos ){
    return thePoints[ipos];}
  G4int GetNumberOfPoints(){
    return thePoints.size();}

private:
  std::vector<GmTrajPoint*> thePoints;


};

#endif
