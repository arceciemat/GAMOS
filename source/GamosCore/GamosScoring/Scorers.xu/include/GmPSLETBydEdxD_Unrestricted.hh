#ifndef GmPSLETBydEdxD_Unrestricted_hh
#define GmPSLETBydEdxD_Unrestricted_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLETBydEdxD_Unrestricted : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLETBydEdxD_Unrestricted(G4String name);
  virtual ~GmPSLETBydEdxD_Unrestricted(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
