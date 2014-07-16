#ifndef GmScoringSurfaceOrb_h
#define GmScoringSurfaceOrb_h 1

#include "GmVScoringSurfaceSolid.hh"

#include "G4Orb.hh"
#include <set>

class GmScoringSurfaceOrb : public GmVScoringSurfaceSolid
{
 
public: 
  GmScoringSurfaceOrb(G4VSolid* orb);
  virtual ~GmScoringSurfaceOrb(){};

  virtual void SetSurfaces( std::vector<G4String> surfaces );
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep );
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint );
  virtual void ComputeArea();

private:
  G4bool CheckSurface(G4double localPos, G4double radius );
  G4bool CheckSurfaceAngle(G4double localAng, G4double angle );
  G4Orb* GetOrb(G4Step* aStep );

private:
  std::set<G4Orb*> theSolids;
  G4Orb* theOrb;
};

#endif
