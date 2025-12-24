#ifndef GmPSLET_dEdx_unrestr_h
#define GmPSLET_dEdx_unrestr_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSLET_dEdx_unrestr : public GmVPrimitiveScorer
		    , public GmComputeDEDX
{
 
public: // with description
  GmPSLET_dEdx_unrestr(G4String name);
  virtual ~GmPSLET_dEdx_unrestr();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    
private:
  G4EmCalculator* theEmCalculator;
};
#endif
