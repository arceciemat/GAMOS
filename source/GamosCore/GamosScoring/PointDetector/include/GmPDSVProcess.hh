#ifndef GmPDSVProcess_h
#define GmPDSVProcess_h

#include "G4VDiscreteProcess.hh"
#include "GmPDSProcessHelper.hh"

class G4SteppingManager;
class GmPDSInteractionAngleManager;
class GmAnalysisMgr;
class GmVFilter;
class GmVClassifier;

#include <map>
#include <set>
class G4ParticleDefinition;
class G4VMscModel;

class GmPDSVProcess : public G4VDiscreteProcess
{
public:
  
  GmPDSVProcess(const G4String&, G4SteppingManager* );
  
  virtual ~GmPDSVProcess();

  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			     );

  virtual G4double GetMeanFreePath(const G4Track& aTrack,
				   G4double   previousStepSize,
				   G4ForceCondition* condition
				   );

  
    virtual G4double AtRestGetPhysicalInteractionLength(const G4Track& ,
                                                G4ForceCondition* condition)
    {
      *condition = Forced;
      return 0.;
    }

  virtual G4VParticleChange* AtRestDoIt(const G4Track& track, const G4Step& aStep);

  virtual G4bool IsApplicable(const G4ParticleDefinition& ) {
    return true;
  }

  //  Calculates from the macroscopic cross section a mean
  //  free path, the value is returned in units of distance.
  virtual G4VParticleChange* PostStepDoIt( const G4Track& ,
					   const G4Step& 
					   );

  virtual void AddHelper( GmPDSProcessHelper* helper );

  void SetFilters( std::vector<GmVFilter*> filts ) {
    theFilters = filts;
  };

protected:
  void ApplyRussianRoulette( G4Track* aTracknc, 
			     double weight 
			     );

  void CreateGeantino( const G4Track& aTrack, 
		       const G4double primPreEner, 
		       const G4StepPoint* stepPoint2, 
		       const G4double secoEner, 
		       const G4int detID, 
		       const G4String procDefStepName,
		       G4int classifierIndex );

  std::vector<G4Track*> GetSecondaryAndPrimaryTracks( const G4Track* aTrack);
  
  void FillHistoEachStep(const G4Step& aStep);

  G4String GetProcessDefiningStepName(const G4Step& aStep);

  G4ParticleDefinition* GetOriginalParticle() const {
    return theOriginalParticle;
  }
  void SetOriginalParticle( G4ParticleDefinition* op ){
    theOriginalParticle = op;
  }

  G4VParticleChange* CheckSecondaries(const G4Track& track, const G4Step& aStep);

protected:
  G4ParticleDefinition* theOriginalParticle;

  GmPDSInteractionAngleManager* theInteractionAngleManager;

  std::map<G4String,G4int> theVolumes;

  G4double theMinimumWeightForSplitting;
  G4double theMaximumWeightForSplitting;
  G4double theMaximumSplitting;
  G4double theSplittingFactor;

  G4bool bUseMaximumDistance;
  G4double theMaximumDistance;
  G4double theInvMaximumDistanceRR;

  G4double theExclusionRadius;

  G4SteppingManager* fpSteppingManager;

  std::map<PDS1aryType, GmPDSProcessHelper*> theHelpers;
  GmPDSProcessHelper* theCurrentHelper;

  std::vector<GmVFilter*> theFilters;
  G4bool bFiltersOnTrack;

  GmVClassifier* theClassifier;
  G4bool bClassifierOnTrack;

  std::map<G4ParticleDefinition*,G4VMscModel*>theMscModels;

};
#endif
