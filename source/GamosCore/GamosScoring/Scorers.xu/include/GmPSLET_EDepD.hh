#ifndef GmPSLET_EDepD_hh
#define GmPSLET_EDepD_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLET_EDepD : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLET_EDepD(G4String name);
  virtual ~GmPSLET_EDepD(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
