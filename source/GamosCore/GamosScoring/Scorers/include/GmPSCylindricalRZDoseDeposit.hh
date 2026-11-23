#ifndef GmPSCylindricalRZDoseDeposit_hh
#define GmPSCylindricalRZDoseDeposit_hh 1

#include <map>
#include "G4THitsMap.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4RotationMatrix.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   Primitive scorer class for scoring dose deposit in the geometry volume, with corrections of energy loss and msc for the case of regular navigation skipping of voxel frontiers
//
// Created: 2008-11-10 Pedro Arce
// 
///////////////////////////////////////////////////////////////////////////////
class G4EnergyLossForExtrapolator;
class GmGeometryUtils;

class GmPSCylindricalRZDoseDeposit : public GmVPrimitiveScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSCylindricalRZDoseDeposit(G4String name);
  virtual ~GmPSCylindricalRZDoseDeposit(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  void SetParameters( const std::vector<G4String>& params);

public: 
  virtual G4double GetUnit() const {
    return 1.E12; }

  virtual G4String GetUnitName() const {
    return "Gy"; }

private:

  GmGeometryUtils* theGeomUtils;

  G4int theNBinsR;
  G4double theMinR;
  G4double theStepR;
  G4int theNBinsZ;
  G4double theMinZ;
  G4double theStepZ;
  G4ThreeVector theCentre;
  G4RotationMatrix theAxisRM;
  G4bool bTotalVolume;
  std::map<G4int,G4double> theVolumes;
  G4double theTotalVolume;
};

#endif
