#ifndef GmPSLET_ESpect_h
#define GmPSLET_ESpect_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include <map>
class GmVNumericDistribution;
struct matSPFile
{
  G4String particle;
  G4String fileName;
  G4int column;
  G4double unit;
  G4String energyType;
  G4String distType;
};

class GmPSLET_ESpect : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSLET_ESpect(G4String name);
  virtual ~GmPSLET_ESpect();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  void BuildMaterialStoppingPowerNames( G4String convFN );
  void InitialiseE2LET();
  G4double GetLETFromEnergy(G4Step* aStep);
    
protected:
  std::map<G4String,G4String> theNameSPFiles;
  std::map<G4String,GmVNumericDistribution*> theDistributions;
};
#endif
