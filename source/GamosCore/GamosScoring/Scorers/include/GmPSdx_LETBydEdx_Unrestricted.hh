#ifndef GmPSdx_LETBydEdx_Unrestricted_h
#define GmPSdx_LETBydEdx_Unrestricted_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSdx_LETBydEdx_Unrestricted : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSdx_LETBydEdx_Unrestricted(G4String name);
  virtual ~GmPSdx_LETBydEdx_Unrestricted();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
