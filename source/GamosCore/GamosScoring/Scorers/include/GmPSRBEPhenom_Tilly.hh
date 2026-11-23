#ifndef GmPSRBEPhenom_Tilly_h
#define GmPSRBEPhenom_Tilly_h 1

#include "GmVPSRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_Tilly : public GmVPSRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_Tilly(G4String name);
  virtual ~GmPSRBEPhenom_Tilly();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  virtual G4bool bIsOnlyAlpha() { return false; };
  
private:
};
#endif
