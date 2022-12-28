#ifndef GmComputeRBEPhenom_h
#define GmComputeRBEPhenom_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
class G4Step;

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmComputeRBEPhenom : public GmComputeDEDX
{
 
public: // with description
  GmComputeRBEPhenom(G4String name);
  virtual ~GmComputeRBEPhenom();
  
protected: // with description
  G4double GetRBE(G4Step* aStep );
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX ) = 0 ;
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX ) = 0;
  
private:
  G4double theAlphaBetaX;
};
#endif
