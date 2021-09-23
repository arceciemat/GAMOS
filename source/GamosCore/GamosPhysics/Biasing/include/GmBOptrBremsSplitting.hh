#ifndef GmBOptrBremsSplitting_hh
#define GmBOptrBremsSplitting_hh 1

#include "GmVBiasingOperator.hh"
class GmBOptnBremsSplitting;

class GmBOptrBremsSplitting : public GmVBiasingOperator {
public:
  GmBOptrBremsSplitting( G4String name );
  virtual ~GmBOptrBremsSplitting() {}
  
public:
  // -------------------------
  // Optional from base class:
  // -------------------------
  // -- Call at run start:
  virtual void      StartRun();
  // -- Call at each track starting:
  virtual void StartTracking( const G4Track* track );

private:
  // -----------------------------
  // -- Mandatory from base class:
  // -----------------------------
  // -- Unused:
  virtual G4VBiasingOperation*
  ProposeNonPhysicsBiasingOperation(const G4Track* /* track */,
                                    const G4BiasingProcessInterface* /* callingProcess */)
  { return 0; }
  virtual G4VBiasingOperation* 
  ProposeOccurenceBiasingOperation (const G4Track* /* track */,
                                    const G4BiasingProcessInterface* /* callingProcess */)
  { return 0; }
  // -- Used:
  virtual G4VBiasingOperation*
  ProposeFinalStateBiasingOperation(const G4Track* track,
                                    const G4BiasingProcessInterface* callingProcess);
  
private:
  // -- Avoid compiler complaining for (wrong) method shadowing,
  // -- this is because other virtual method with same name exists.
  using GmVBiasingOperator::OperationApplied;

private:
  GmBOptnBremsSplitting* fBremsSplittingOperation;
  G4int                          fNSplit;
  G4bool                         fBiasPrimaryOnly;
  G4bool                            fBiasOnlyOnce;
  G4int                             nInteractions;
  
};

#endif
