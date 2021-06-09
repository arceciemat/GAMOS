#ifndef GmScoringSurfaceTubs_h
#define GmScoringSurfaceTubs_h 1

#include "GmVScoringSurfaceSolid.hh"

#include "G4Tubs.hh"
#include <set>

class GmScoringSurfaceTubs : public GmVScoringSurfaceSolid
{
 
public: 
  GmScoringSurfaceTubs(G4VSolid* tubs);
  virtual ~GmScoringSurfaceTubs(){};

  virtual void SetSurfaces( std::vector<G4String> surfaces );
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep );
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint );
  virtual void ComputeArea();

private:
  G4bool CheckSurfaceRadius(G4ThreeVector localPos, G4double radius );
  G4bool CheckSurfaceCap(G4double localPos, G4double dim );
  G4bool CheckSurfacePhi(G4double localPhi, G4double phi );

private:
  G4Tubs* theTubs;
  G4ThreeVector theNormal;
};

#endif
