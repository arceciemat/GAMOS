#ifndef GmTrajectorySteps_H
#define GmTrajectorySteps_H

#include "GmVTrajectory.hh"
class GmTrajStep;

class GmTrajectorySteps : public GmVTrajectory
{
public:
  GmTrajectorySteps();
  GmTrajectorySteps(const G4Track* aTrack);
  virtual ~GmTrajectorySteps();
 
  void Clear();

  void AddStep( GmTrajStep* point ){
    theSteps.push_back( point ); }
 
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
  
  const std::vector<GmTrajStep*> GetSteps() const{
    return theSteps;}
  const GmTrajStep* GetStep(G4int ipos ){
    return theSteps[ipos];}
  G4int GetNumberOfSteps(){
    return theSteps.size();}

protected:
  G4int                     fTrackID;
  G4int                     fParentID;
  G4int                     fPDGEncoding;
  G4double                  fPDGCharge;
  G4String                  fParticleName;
  G4ThreeVector             fInitialMomentum;

  std::vector<GmTrajStep*> theSteps;


};

#endif
