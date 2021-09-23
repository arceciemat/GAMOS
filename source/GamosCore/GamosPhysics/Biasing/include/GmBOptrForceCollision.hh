// ---------------------------------------------------------------
//
// GmBOptrForceCollision
//
// Class Description:
//    A GmVBiasingOperator that implements a "force collision" a la
// MCNP. This is meant for neutral particles.
//    When the track enters the volume, it is cloned. One copy makes
// a forced free flight up to the volume exit. The other copy makes
// a forced collision inside the volume.
//
// ---------------------------------------------------------------
//   Initial version                         Nov. 2013 M. Verderi


#ifndef GmBOptrForceCollision_hh
#define GmBOptrForceCollision_hh 1

#include "GmVBiasingOperator.hh"
class G4BOptnForceFreeFlight;
class G4BOptnForceCommonTruncatedExp;
class G4BOptnCloning;
class G4VProcess;
class G4BiasingProcessInterface;
class G4ParticleDefinition;
#include <vector>
#include <map>
#include "G4ThreeVector.hh"
class GmBOptrForceCollisionTrackData;

class GmBOptrForceCollision : public GmVBiasingOperator {
public:
  GmBOptrForceCollision(G4String name);
  ~GmBOptrForceCollision();
  
private:  // -- Mandatory from base class :
  virtual G4VBiasingOperation* ProposeNonPhysicsBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess) final;
  virtual G4VBiasingOperation*  ProposeOccurenceBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess) final;
  virtual G4VBiasingOperation* ProposeFinalStateBiasingOperation(const G4Track* track, const G4BiasingProcessInterface* callingProcess) final;
  // -- optional methods from base class:
public:
  //  virtual void           Configure() final;
  //  virtual void  ConfigureForWorker() final;
  virtual void            StartRun() final;
  virtual void       StartTracking( const G4Track* track ) final;
  //  virtual void         ExitBiasing( const G4Track*, const G4BiasingProcessInterface* ) final {};
  //  virtual void         EndTracking() final;

  // -- operation applied:
  //  void OperationApplied( const G4BiasingProcessInterface*            callingProcess, G4BiasingAppliedCase                      biasingCase,
  //			 G4VBiasingOperation*                      operationApplied, const G4VParticleChange*        particleChangeProduced ) final;
  //  void OperationApplied( const G4BiasingProcessInterface*            callingProcess, G4BiasingAppliedCase                      biasingCase,
  // 			 G4VBiasingOperation*             occurenceOperationApplied, G4double                 weightForOccurenceInteraction,
  //			 G4VBiasingOperation*            finalStateOperationApplied, const G4VParticleChange*        particleChangeProduced ) final;
  

private:
  G4int                                      fForceCollisionModelID;
  const G4Track*                                      fCurrentTrack;
  GmBOptrForceCollisionTrackData*                 fCurrentTrackData;
  std::map< const G4BiasingProcessInterface*, G4BOptnForceFreeFlight* > fFreeFlightOperations;
  G4BOptnForceCommonTruncatedExp*  fSharedForceInteractionOperation;
  G4BOptnCloning*                                 fCloningOperation;
  G4double                                      fInitialTrackWeight;
  G4bool                                                     fSetup;
  const G4ParticleDefinition*                       fParticleToBias;
  G4bool                            fBiasOnlyOnce;
  G4int                             nInteractions;
};

#endif
