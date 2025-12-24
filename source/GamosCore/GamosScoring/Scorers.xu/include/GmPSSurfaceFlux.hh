#ifndef GmPSSurfaceFlux_h
#define GmPSSurfaceFlux_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmPSWithDirection.hh"
#include "G4THitsMap.hh"

#include "G4Sphere.hh"
#include "G4PSDirectionFlag.hh"

#include <set>
#include <vector>

class GmVScoringSurfaceSolid;

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Flux.
//   The flux is given in the unit of area.
//
///////////////////////////////////////////////////////////////////////////////

class GmPSSurfaceFlux : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSSurfaceFlux(G4String name);
  virtual ~GmPSSurfaceFlux(){};
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual G4double GetUnit() const {
    return 100.; }
  
  virtual G4String GetUnitName() const {
    return "[cm^-2]"; }
  
private:
  G4bool ProcessHits(G4StepPoint* aStepPoint, G4Step* aStep, G4int aDirFlag );

private:
  std::vector<G4String> theSurfaces;
  std::map<G4VSolid*, GmVScoringSurfaceSolid*> theScoringSolids;
  GmVScoringSurfaceSolid* theScoringSolid;
  G4PSFluxFlag theDirection;
  G4bool bArea;
  G4bool bAngleFactor;
};

#endif
