#ifndef GmPSEdep_LETBydEdx_h
#define GmPSEdep_LETBydEdx_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
class G4EmCalculator;
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSEdep_LETBydEdx : public GmVPrimitiveScorer
{
 
public: // with description
  GmPSEdep_LETBydEdx(G4String name);
  virtual ~GmPSEdep_LETBydEdx();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
private:
  G4EmCalculator* theEmCalculator;
};
#endif
