#ifndef GmPSRBEPhenom_Carabe_h
#define GmPSRBEPhenom_Carabe_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_Carabe : public GmVPrimitiveScorer
			   ,public GmComputeRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_Carabe(G4String name);
  virtual ~GmPSRBEPhenom_Carabe();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  
private:
};
#endif
