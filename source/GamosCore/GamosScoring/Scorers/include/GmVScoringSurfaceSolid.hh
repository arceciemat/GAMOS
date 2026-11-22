#ifndef GmVScoringSurfaceSolid_h
#define GmVScoringSurfaceSolid_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmPSWithDirection.hh"
#include "G4THitsMap.hh"

#include "G4Sphere.hh"
#include <set>

class GmVScoringSurfaceSolid 
{
 
public: 
  GmVScoringSurfaceSolid();
  virtual ~GmVScoringSurfaceSolid(){};

  G4ThreeVector GetLocalPoint( G4ThreeVector pos, G4Step* aStep );
  virtual void SetSurfaces( std::vector<G4String> surfaces ) = 0;
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep ) = 0;
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint, G4StepPoint* volumeStepPoint ) = 0;
  virtual void ComputeArea() = 0;
  G4double GetArea(){
    return theArea;
  }

protected:
  std::set<G4String> theSurfaces;
  G4double theSurfaceTolerance;
  G4double theAngularTolerance;
  G4double theArea;
  G4ThreeVector theNormal;
  G4int theNSurfaceMax;
  G4PSFluxFlag theDirection;
};

#endif
