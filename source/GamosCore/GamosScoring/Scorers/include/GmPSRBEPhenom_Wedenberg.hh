#ifndef GmPSRBEPhenom_Wedenberg_h
#define GmPSRBEPhenom_Wedenberg_h 1

#include "GmVPSRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_Wedenberg : public GmVPSRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_Wedenberg(G4String name);
  virtual ~GmPSRBEPhenom_Wedenberg();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  virtual G4bool bIsOnlyAlpha() { return false; };
  
private:
};
#endif
