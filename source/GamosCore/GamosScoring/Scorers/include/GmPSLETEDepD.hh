#ifndef GmPSLETEDepD_hh
#define GmPSLETEDepD_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLETEDepD : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLETEDepD(G4String name);
  virtual ~GmPSLETEDepD(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
