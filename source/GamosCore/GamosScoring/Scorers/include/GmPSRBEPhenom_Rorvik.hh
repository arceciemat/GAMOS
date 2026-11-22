#ifndef GmPSRBEPhenom_Rorvik_h
#define GmPSRBEPhenom_Rorvik_h 1

#include "GmVPSRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_Rorvik : public GmVPSRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_Rorvik(G4String name);
  virtual ~GmPSRBEPhenom_Rorvik();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  virtual G4bool bIsOnlyAlpha() { return false; };
  
private:
};
#endif
