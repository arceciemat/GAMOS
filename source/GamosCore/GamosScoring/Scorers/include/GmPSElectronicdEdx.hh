#ifndef GmPSElectronicdEdx_h
#define GmPSElectronicdEdx_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSElectronicdEdx : public GmVPrimitiveScorer
	       , public GmComputeDEDX
{
 
public: // with description
  GmPSElectronicdEdx(G4String name);
  virtual ~GmPSElectronicdEdx();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
