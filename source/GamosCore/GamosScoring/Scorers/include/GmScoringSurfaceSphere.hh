#ifndef GmScoringSurfaceSphere_h
#define GmScoringSurfaceSphere_h 1

#include "GmVScoringSurfaceSolid.hh"

#include "G4Sphere.hh"
#include <set>

class GmScoringSurfaceSphere : public GmVScoringSurfaceSolid
{
 
public: 
  GmScoringSurfaceSphere(G4VSolid* sphere);
  virtual ~GmScoringSurfaceSphere(){};

  virtual void SetSurfaces( std::vector<G4String> surfaces );
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep );
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint, G4StepPoint* volumeStepPoint );
  virtual void ComputeArea();

private:
  G4bool CheckSurface(G4double localPos, G4double radius );
  G4bool CheckSurfaceAngle(G4double localAng, G4double angle );
  G4Sphere* GetSphere(G4Step* aStep );

private:
  std::set<G4Sphere*> theSolids;
  G4Sphere* theSphere;
};

#endif
