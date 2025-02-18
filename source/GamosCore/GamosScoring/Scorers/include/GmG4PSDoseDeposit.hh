#ifndef GmG4PSDoseDeposit_hh
#define GmG4PSDoseDeposit_hh 1

#include <vector>
#include <map>
#include "G4THitsMap.hh"
class GmVFilter;
//#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
class GmVPSPrinter;
class GmVClassifier;
class G4PhantomParameterisation;
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   Primitive scorer class for scoring dose deposit in the geometry volume, with corrections of energy loss and msc for the case of regular navigation skipping of voxel frontiers
//
// Created: 2008-11-10 Pedro Arce
// 
///////////////////////////////////////////////////////////////////////////////
class   G4EnergyLossForExtrapolator;
class GmGeometryUtils;

class GmG4PSDoseDeposit : public GmVPrimitiveScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmG4PSDoseDeposit(G4String name);
  virtual ~GmG4PSDoseDeposit(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  G4double GetGeom2TrueStepLength( G4double kinEnergy ) ;

  public: 

  virtual G4double GetUnit() const {
    return 1.E12; }

  virtual G4String GetUnitName() const {
    return "Gy"; }

private:

  G4EnergyLossForExtrapolator* theElossExt;

  G4PhantomParameterisation* thePhantomParam;

  GmGeometryUtils* theGeomUtils;

};

#endif
