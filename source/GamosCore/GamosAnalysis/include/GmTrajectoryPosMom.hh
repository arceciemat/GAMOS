#ifndef GmTrajectoryPosMom_H
#define GmTrajectoryPosMom_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmTrajectory.hh"
class GmTrajPointPosMom;

class GmTrajectoryPosMom : public GmTrajectory
{
public:
  GmTrajectoryPosMom();
  GmTrajectoryPosMom(const G4Track* aTrack);
  virtual ~GmTrajectoryPosMom();
 
  void Clear();

  void AddPoint( GmTrajPointPosMom* point ){
    thePoints.push_back( point ); }
 
  // Get/Set functions 
  inline G4int GetTrackID() const
  { return fTrackID; }
  inline G4int GetParentID() const
  { return fParentID; }
  inline G4String GetParticleName() const
  { return fParticleName; }
  inline G4double GetCharge() const
  { return fPDGCharge; }
  inline G4int GetPDGEncoding() const
  { return fPDGEncoding; }
  inline G4ThreeVector GetInitialMomentum() const
  { return fInitialMomentum; }
  
  const std::vector<GmTrajPointPosMom*> GetPoints() const{
    return thePoints;}
  const GmTrajPointPosMom* GetPoint(G4int ipos ){
    return thePoints[ipos];}
  G4int GetNumberOfPoints(){
    return thePoints.size();}

private:
  G4int                     fTrackID;
  G4int                     fParentID;
  G4int                     fPDGEncoding;
  G4double                  fPDGCharge;
  G4String                  fParticleName;
  G4ThreeVector             fInitialMomentum;

  std::vector<GmTrajPointPosMom*> thePoints;


};

#endif
