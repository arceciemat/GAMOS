#ifndef GmPDSGeantinoProcess_h
#define GmPDSGeantinoProcess_h

#include "GmPDSVProcess.hh"

class G4SteppingManager;
class G4HadronicProcess;
class G4ParticleDefinition;
class GmPDSInvertGeantinoStackUA;

class GmPDSGeantinoProcess : public GmPDSVProcess
{
public:
  
  GmPDSGeantinoProcess(const G4String&, G4SteppingManager*, G4ParticleDefinition* origParticle );
  
  virtual ~GmPDSGeantinoProcess();
  
  virtual G4VParticleChange* PostStepDoIt(
			     const G4Track& ,
			     const G4Step& 
			     );

  void ChangeGeantinoWeight(const G4Step* aStep);


protected:// with description
  virtual G4double GetMeanFreePath(const G4Track& aTrack,
				   G4double   previousStepSize,
				   G4ForceCondition* condition
				   );
  //  Calculates from the macroscopic cross section a mean
  //  free path, the value is returned in units of distance.

private:
  void SplitOriginalParticle( const G4Track& aTrack, const G4Step& aStep, G4ParticleChange& aParticleChange );

private:
  /*  GmAnalysisMgr* theAnaMgr;
  std::map<G4String,G4int> theVolumes;
  */

  G4double theMinimumGeantinoWeight;
  G4double theInvMinimumGeantinoWeightRR;

  G4bool bUseOriginalParticleSplitting;
  G4bool bUseMinimumGeantinoWeight;
  GmPDSInvertGeantinoStackUA* theInverseStackingUA;

};
#endif
