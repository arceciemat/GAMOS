//---------------------------------------------------------------
//
// GmBOptnDirBremsSplitting
//
// Class Description:
//    A G4VBiasingOperation to change a process cross-section.
//    
//
//---------------------------------------------------------------
//   Initial version                         Nov. 2014 M. Verderi


#ifndef GmBOptnDirBremsSplitting_hh
#define GmBOptnDirBremsSplitting_hh 1

#include "G4VBiasingOperation.hh"
#include "G4ParticleChange.hh"


class GmBOptnDirBremsSplitting : public G4VBiasingOperation {
public:
  // -- Constructor :
  GmBOptnDirBremsSplitting(G4String name);
  // -- destructor:
  virtual ~GmBOptnDirBremsSplitting();
  
public:
  // ----------------------------------------------
  // -- Methods from G4VBiasingOperation interface:
  // ----------------------------------------------
  // -- Unused:
  virtual const G4VBiasingInteractionLaw* 
  ProvideOccurenceBiasingInteractionLaw( const G4BiasingProcessInterface*,
                                         G4ForceCondition& )
  { return 0; }
  
  // --Used:
  virtual G4VParticleChange*   ApplyFinalStateBiasing( const G4BiasingProcessInterface*,
                                                       const G4Track*,
                                                       const G4Step*,
                                                       G4bool&                          );
  
  // -- Unsued:
  virtual G4double           DistanceToApplyOperation( const G4Track*,
                                                       G4double,
                                                       G4ForceCondition*)
  {return DBL_MAX;}
  virtual G4VParticleChange* GenerateBiasingFinalState( const G4Track*,
                                                        const G4Step*   ) 
  {return 0;}
  
  G4bool IsInPlane(const G4ThreeVector dir, const G4ThreeVector pos);
  
public:
  // ----------------------------------------------
  // -- Additional methods, specific to this class:
  // ----------------------------------------------
  // -- Splitting factor:
  void     SetNSplit(G4int splittingFactor)
  { fNSplit = splittingFactor; }
  G4int GetNSplit() const { return fNSplit; }
  void     SetNKill(G4int iKill){
    fNKill = iKill;
    fNKillInv = 1./iKill;
  }
  
private:
  G4int            fNSplit;
  G4int            fNKill;
  G4double fNKillInv;
  G4ParticleChange fParticleChange;
  G4double thePlaneX;
  G4double thePlaneY;
  G4double thePlaneZ;
};

#endif
