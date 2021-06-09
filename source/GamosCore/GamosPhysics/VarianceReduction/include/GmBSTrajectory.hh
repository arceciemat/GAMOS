#ifndef GmBSTrajectory_H
#define GmBSTrajectory_H

#include "GamosCore/GamosAnalysis/include/GmVTrajectory.hh"
#include "G4ThreeVector.hh"

class GmBSTrajectory : public GmVTrajectory
{
public:
  GmBSTrajectory();
  GmBSTrajectory(const G4Track* aTrack);
  virtual ~GmBSTrajectory();

  G4double GetPosX() const { return fInitialPosition.x(); }
  G4double GetPosY() const { return fInitialPosition.y(); }
  G4double GetPosZ() const { return fInitialPosition.z(); }
  G4double GetDirectionTheta() const { return fInitialMomentum.theta(); }
  G4double GetDirectionPhi() const { return fInitialMomentum.phi(); }
  G4double GetEnergy() const { return fEnergy; }

  void Clear();

  // Get/Set functions 

private:

  G4ThreeVector fInitialPosition;
  G4double fEnergy;

};

#endif
