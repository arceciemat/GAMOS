#ifndef GmPSDose_LET_dEdx_h
#define GmPSDose_LET_dEdx_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSDose_LET_dEdx : public GmVPrimitiveScorer
			 , public GmComputeDEDX
{
 
public: // with description
  GmPSDose_LET_dEdx(G4String name);
  virtual ~GmPSDose_LET_dEdx();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
