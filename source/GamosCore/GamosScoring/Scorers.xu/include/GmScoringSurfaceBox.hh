#ifndef GmScoringSurfaceBox_h
#define GmScoringSurfaceBox_h 1

#include "GmVScoringSurfaceSolid.hh"

#include "G4Box.hh"
#include <set>

class GmScoringSurfaceBox : public GmVScoringSurfaceSolid
{
 
public: 
  GmScoringSurfaceBox(G4VSolid* box);
  virtual ~GmScoringSurfaceBox(){};

  virtual void SetSurfaces( std::vector<G4String> surfaces );
  virtual G4bool IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep );
  virtual G4double GetAngleFactor( G4StepPoint* stepPoint, G4StepPoint* volumeStepPoint );
  virtual void ComputeArea();


private:
  G4bool CheckSurface(G4double localPos, G4double dim );

private:
  G4Box* theBox;
};

#endif
