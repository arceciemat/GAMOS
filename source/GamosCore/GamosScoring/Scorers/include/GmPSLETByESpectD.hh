#ifndef GmPSLETByESpectD_hh
#define GmPSLETByESpectD_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLETByESpectD : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLETByESpectD(G4String name);
  virtual ~GmPSLETByESpectD(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
