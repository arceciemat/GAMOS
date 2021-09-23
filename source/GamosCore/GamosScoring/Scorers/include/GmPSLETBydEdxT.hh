#ifndef GmPSLETBydEdxT_hh
#define GmPSLETBydEdxT_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLETBydEdxT : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLETBydEdxT(G4String name);
  virtual ~GmPSLETBydEdxT(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
