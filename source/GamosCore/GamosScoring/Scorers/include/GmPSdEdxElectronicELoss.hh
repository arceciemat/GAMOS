#ifndef GmPSdEdxElectronicELoss_h
#define GmPSdEdxElectronicELoss_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSdEdxElectronicELoss : public GmVPrimitiveScorer
	       , public GmComputeDEDX
{
 
public: // with description
  GmPSdEdxElectronicELoss(G4String name);
  virtual ~GmPSdEdxElectronicELoss();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
