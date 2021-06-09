#ifndef GmTrajStepEner_H
#define GmTrajStepEner_H

#include <vector>
#include "globals.hh"
#include "GmVTrajStep.hh"
class G4Step;

class GmTrajStepEner : public GmVTrajStep
{
public:
  GmTrajStepEner( const G4Step* , TSPUse pointToUse = TSPUPrePost);
  virtual ~GmTrajStepEner(){};

  G4double GetKineticEnergyPre() const { return theKineticEnergyPre;}
  G4double GetKineticEnergyPost() const { return theKineticEnergyPost;}
  G4double GetDepositedEnergy() const { return theDepositedEnergy;}
  G4double GetDeltaKineticEnergy() const;

private:
  G4double theKineticEnergyPre;
  G4double theKineticEnergyPost;
  G4double theDepositedEnergy;

};

#endif
