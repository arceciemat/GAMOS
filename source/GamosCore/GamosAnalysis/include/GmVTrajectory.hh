#ifndef GmVTrajectory_H
#define GmVTrajectory_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
class G4Track;

class GmVTrajectory
{
public:
  GmVTrajectory();
  GmVTrajectory(const G4Track* aTrack);
  virtual ~GmVTrajectory(){};
 
  virtual void Clear() = 0;

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
  inline G4double GetVertexKineticEnergy() const
  { return fVertexKineticEnergy; }
 inline G4ThreeVector GetVertexMomentumDirection() const
  { return fVertexMomentumDirection; }
  
protected:
  G4int                     fTrackID;
  G4int                     fParentID;
  G4int                     fPDGEncoding;
  G4double                  fPDGCharge;
  G4String                  fParticleName;
  G4ThreeVector             fInitialMomentum;
  G4double                  fVertexKineticEnergy;
  G4ThreeVector             fVertexMomentumDirection;

};

#endif
