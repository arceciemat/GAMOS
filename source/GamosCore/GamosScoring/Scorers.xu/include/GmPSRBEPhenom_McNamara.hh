#ifndef GmPSRBEPhenom_McNamara_h
#define GmPSRBEPhenom_McNamara_h 1

#include "GmVPSRBEPhenom.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSRBEPhenom_McNamara : public GmVPSRBEPhenom
{
 
public: // with description
  GmPSRBEPhenom_McNamara(G4String name);
  virtual ~GmPSRBEPhenom_McNamara();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX );
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX );
  virtual G4bool bIsOnlyAlpha() { return false; };
  
private:
};
#endif
