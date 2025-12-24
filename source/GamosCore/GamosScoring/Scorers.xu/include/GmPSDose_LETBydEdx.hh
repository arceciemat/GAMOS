#ifndef GmPSDose_LETBydEdx_h
#define GmPSDose_LETBydEdx_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSDose_LETBydEdx : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSDose_LETBydEdx(G4String name);
  virtual ~GmPSDose_LETBydEdx();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
