//---------------------------------------------------------------
//
// GmBOptnEWBS
//
// Class Description:
//    A G4VBiasingOperation to change a process cross-section.
//    
//
//---------------------------------------------------------------
//   Initial version                         Nov. 2014 M. Verderi


#ifndef GmBOptnEWBS_hh
#define GmBOptnEWBS_hh 1

#include "GmVEWBiasingOperation.hh"
#include "G4ParticleChange.hh"
#include "G4UImessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

class G4BiasingProcessInterface;

class GmBOptnEWBS : public GmVEWBiasingOperation, public G4UImessenger {
public:
  // -- Constructor :
  GmBOptnEWBS(G4String name);
  // -- destructor:
  virtual ~GmBOptnEWBS();
  
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

  void SetNewValue(G4UIcommand * command,G4String newValues);
  
  void SetNSplit(G4int splittingFactor);

private:
  GmUIcmdWithAString* addPartProcCmd;
  std::map<G4String,GmVEWBiasingOperation*> theOperationNames;
  std::map<const G4BiasingProcessInterface*,GmVEWBiasingOperation*> theOperations;
  GmUIcmdWithAString* addSplittingProcCmd;

};

#endif
