#ifndef GmPSRBEPhenom_Peeler_h
#define GmPSRBEPhenom_Peeler_h 1

#include "GmVPSRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_Peeler : public GmVPSRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_Peeler(G4String name);
  virtual ~GmPSRBEPhenom_Peeler();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  virtual G4bool bIsOnlyAlpha() { return false; };
  
private:
};
#endif
