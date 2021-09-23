//---------------------------------------------------------------
//
// GmBOptnBremsSplitting
//
// Class Description:
//    A G4VBiasingOperation to change a process cross-section.
//    
//
//---------------------------------------------------------------
//   Initial version                         Nov. 2014 M. Verderi


#ifndef GmBOptnBremsSplitting_hh
#define GmBOptnBremsSplitting_hh 1

#include "G4VBiasingOperation.hh"
#include "G4ParticleChange.hh"


class GmBOptnBremsSplitting : public G4VBiasingOperation {
public:
  // -- Constructor :
  GmBOptnBremsSplitting(G4String name);
  // -- destructor:
  virtual ~GmBOptnBremsSplitting();
  
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
  
  
public:
  // ----------------------------------------------
  // -- Additional methods, specific to this class:
  // ----------------------------------------------
  // -- Splitting factor:
  void     SetNSplit(G4int splittingFactor)
  { fNSplit = splittingFactor; }
  G4int GetNSplit() const { return fNSplit; }
  
private:
  G4int            fNSplit;
  G4ParticleChange fParticleChange;
};

#endif
