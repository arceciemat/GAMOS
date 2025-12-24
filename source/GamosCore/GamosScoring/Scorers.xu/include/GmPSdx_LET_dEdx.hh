#ifndef GmPSdx_LET_dEdx_h
#define GmPSdx_LET_dEdx_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSdx_LET_dEdx : public GmVPrimitiveScorer
		       , public GmComputeDEDX
{
 
public: // with description
  GmPSdx_LET_dEdx(G4String name);
  virtual ~GmPSdx_LET_dEdx();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
