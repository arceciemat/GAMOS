#ifndef GmComputeDEDX_h
#define GmComputeDEDX_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
class G4Step;

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmComputeDEDX
{
 
public: // with description
  GmComputeDEDX();
  virtual ~GmComputeDEDX();
  
protected: // with description
  G4double GetDEDX(G4Step* aStep, G4bool bRestricted);
  G4double GetElectronicDEDX(G4Step* aStep, G4bool bRestricted);
  G4double GetSecondaryElectronEnergy(G4Step* aStep);
  
private:
};
#endif
