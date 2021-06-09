#ifndef GmTrajStepPosMom_H
#define GmTrajStepPosMom_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVTrajStep.hh"
class G4Step;

class GmTrajStepPosMom : public GmVTrajStep
{
public:
  GmTrajStepPosMom( const G4Step* , TSPUse pointToUse = TSPUPrePost);
  virtual ~GmTrajStepPosMom();

  G4ThreeVector GetPositionPre() const { return thePositionPre;}
  G4ThreeVector GetMomentumPre() const { return theMomentumPre;}
  G4ThreeVector GetMomentumDirectionPre() const;
  G4ThreeVector GetPositionPost() const { return thePositionPost;}
  G4ThreeVector GetMomentumPost() const { return theMomentumPost;}
  G4ThreeVector GetMomentumDirectionPost() const;
  G4ThreeVector GetPosition() const;
  G4ThreeVector GetMomentum() const;
  G4ThreeVector GetMomentumDirection() const;

  G4ThreeVector GetDeltaPosition() const;
  G4ThreeVector GetDeltaMomentum() const;
  G4ThreeVector GetDeltaMomentumDirection() const;
  G4double GetDeltaAngle() const;

private:
  G4ThreeVector thePositionPre;
  G4ThreeVector theMomentumPre;
  G4ThreeVector thePositionPost;
  G4ThreeVector theMomentumPost;

};

#endif
