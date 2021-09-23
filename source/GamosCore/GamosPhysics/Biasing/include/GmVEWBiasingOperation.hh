//---------------------------------------------------------------
//
// GmVEWBiasingOperation
//


#ifndef GmVEWBiasingOperation_hh
#define GmVEWBiasingOperation_hh 1

#include "G4VBiasingOperation.hh"
#include "G4ParticleChange.hh"


class GmVEWBiasingOperation : public G4VBiasingOperation {
public:
  // -- Constructor :
  GmVEWBiasingOperation(G4String name);
  // -- destructor:
  virtual ~GmVEWBiasingOperation();
  
public:
  G4bool IsInPlane(const G4ThreeVector dir, const G4ThreeVector pos);
  
public:
  // ----------------------------------------------
  // -- Additional methods, specific to this class:
  // ----------------------------------------------
  // -- Splitting factor:
  void SetNSplitAndInverse(G4int splittingFactor)
  {
    fNSplit = splittingFactor;
    fNSplitInv = 1./splittingFactor;
  }
  G4int GetNSplit() const { return fNSplit; }
  
protected:
  G4int fNSplit;
  G4double fNSplitInv;
  G4ParticleChange fParticleChange;
  G4double thePlaneX;
  G4double thePlaneY;
  G4double thePlaneZ;
  G4double theGasDensity;

  G4double theRRZ;

};

#endif
