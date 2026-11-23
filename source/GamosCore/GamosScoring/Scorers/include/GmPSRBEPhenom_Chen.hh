#ifndef GmPSRBEPhenom_Chen_h
#define GmPSRBEPhenom_Chen_h 1

#include "GmVPSRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_Chen : public GmVPSRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_Chen(G4String name);
  virtual ~GmPSRBEPhenom_Chen();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMaxAlpha( G4double LETd, G4double alphaX );
  virtual G4bool bIsOnlyAlpha() { return true; };  
private:
};
#endif
