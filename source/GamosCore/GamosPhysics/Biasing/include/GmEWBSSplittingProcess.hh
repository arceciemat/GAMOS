#ifndef GmEWBSSplittingProcess_HH
#define GmEWBSSplittingProcess_HH 1

/////////////
// Includes
/////////////

#include "G4VDiscreteProcess.hh"

/////////////////////
// Class Definition
/////////////////////

class GmEWBSSplittingProcess : public G4VDiscreteProcess
{

public:

        ////////////////////////////////
        // Constructors and Destructor
        ////////////////////////////////

        GmEWBSSplittingProcess(const G4String& processName = "GmEWBSSplittingProcess",
                             G4ProcessType type = fUCN);
	virtual ~GmEWBSSplittingProcess();

private:

        GmEWBSSplittingProcess(const GmEWBSSplittingProcess &right);

        //////////////
        // Operators
        //////////////

        GmEWBSSplittingProcess& operator=(const GmEWBSSplittingProcess &right);

public:

        ////////////
        // Methods
        ///////////

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable' only for an UCN.

  /*        G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4double ,
  		     G4ForceCondition* condition);
  */
        G4double GetMeanFreePath(const G4Track& aTrack,
                                 G4double ,
                                 G4ForceCondition* condition);

        G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
                                        const G4Step&  aStep);

private:
  G4double thePlaneMinZ;
  G4double thePlaneMaxZ;
  G4int theNSplit;
  G4double theMinimumWeight;
  
};

#endif

