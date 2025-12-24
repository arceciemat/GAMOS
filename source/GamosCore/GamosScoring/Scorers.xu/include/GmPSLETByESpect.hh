#ifndef GmPSLETByESpect_h
#define GmPSLETByESpect_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include <map>
class GmNumericDistributionLogLog;

class GmPSLETByESpect : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSLETByESpect(G4String name);
  virtual ~GmPSLETByESpect();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  void BuildMaterialConversionNames( G4String convFN );
  void InitialiseE2LET();
  G4double GetLETFromEnergy(G4Step* aStep);
    
protected:
  std::map<G4String,G4String> theNameConversions;
  std::map<G4String,GmNumericDistributionLogLog*> theDistributions;
};
#endif
