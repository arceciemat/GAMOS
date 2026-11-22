#ifndef GmPSLETBydEdx_h
#define GmPSLETBydEdx_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSLETBydEdx : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSLETBydEdx(G4String name);
  virtual ~GmPSLETBydEdx();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    
private:
  G4EmCalculator* theEmCalculator;
};
#endif
