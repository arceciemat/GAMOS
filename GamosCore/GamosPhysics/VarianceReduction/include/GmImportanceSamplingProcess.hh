#ifndef GmImportanceSamplingProcess_h
#define GmImportanceSamplingProcess_h

#include "G4VDiscreteProcess.hh"

class GmAnalysisMgr;
class GmVDistribution;
class GmVFilter;
class GmInheritTrackInfoUA;

class GmImportanceSamplingProcess : public G4VDiscreteProcess
{
public:
  
  GmImportanceSamplingProcess(const G4String&, GmVDistribution*  );
  
  virtual ~GmImportanceSamplingProcess();

  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			     );

  virtual G4double GetMeanFreePath(const G4Track& aTrack,
				   G4double   previousStepSize,
				   G4ForceCondition* condition
				   );
  //  Calculates from the macroscopic cross section a mean
  //  free path, the value is returned in units of distance.
 
  virtual G4VParticleChange* PostStepDoIt( const G4Track& ,
					   const G4Step& 
					   );

private:
  void FillHistoEachStep(const G4Step& aStep);

private:
  GmVDistribution* theDistribution;

  G4int theMaxSplitTimes;
  G4bool bSplitAtSeveralSteps;
  G4bool bApplyRussianRoulette;
  G4bool bApplySplitting;

  std::vector<GmVFilter*> theFilters;

  static GmInheritTrackInfoUA* theInheritTrackInfoUA;

};
#endif
