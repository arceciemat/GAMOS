#ifndef GmPSEquivDose_h
#define GmPSEquivDose_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"

#include <set>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Volume Flux.
//   The flux is given in the unit of area.
//
///////////////////////////////////////////////////////////////////////////////
class GmGeometryUtils;

class GmPSEquivDose : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSEquivDose(G4String name);
  virtual ~GmPSEquivDose(){};
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public:
  virtual G4double GetUnit() const {
    return 100.; }
  
  virtual G4String GetUnitName() const {
    return "[cm^-2]"; }
  
private:
  GmGeometryUtils* theGeometryUtils; 
};

#endif
