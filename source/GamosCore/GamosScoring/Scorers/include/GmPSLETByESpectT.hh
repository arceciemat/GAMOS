#ifndef GmPSLETByESpectT_hh
#define GmPSLETByESpectT_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLETByESpectT : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLETByESpectT(G4String name);
  virtual ~GmPSLETByESpectT(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
