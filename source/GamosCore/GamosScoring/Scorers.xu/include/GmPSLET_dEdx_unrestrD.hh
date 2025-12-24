#ifndef GmPSLET_dEdx_unrestrD_hh
#define GmPSLET_dEdx_unrestrD_hh 1

class GmScoringMgr;
#include <vector>
#include "GamosCore/GamosScoring/Management/include/GmCompoundScorer.hh"

class GmPSLET_dEdx_unrestrD : public GmCompoundScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmPSLET_dEdx_unrestrD(G4String name);
  virtual ~GmPSLET_dEdx_unrestrD(){ };
  
protected: // with description

  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual void SetParameters( const std::vector<G4String>& params );
  
};

#endif
