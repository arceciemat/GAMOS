// ------------------------------------------------------------

#ifndef GmWrapperProcess_h 
#define GmWrapperProcess_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VParticleChange.hh"
#include "G4WrapperProcess.hh"

class GmWrapperProcess  :public G4WrapperProcess
{
  //  A virtual class for wrapper process objects.
public: // with description
  //  constructor requires the process name and type
  GmWrapperProcess(const G4String& aName, G4VProcess* origProcess, G4double efact );

  //  copy constructor copys the name but does not copy the 
  //  physics table (0 pointer is assigned)
  //  GmWrapperProcess(const GmWrapperProcess &right);

public: 
  //  destructor 
  virtual ~GmWrapperProcess();

public: // with description
  //  Set/Get methods for the enhancement factor
  void SetEnhanceFactor( G4double ) ;
  G4double GetEnhanceFactor ( ) const; 
  //
  void SetBiasMode2(G4bool mode) {BiasMode2 = mode; };

  ////////////////////////////
  // DoIt    /////////////////
  ///////////////////////////
  virtual G4VParticleChange* PostStepDoIt(
				  const G4Track& track,
				  const G4Step&  stepData
				  );

  virtual G4VParticleChange* AlongStepDoIt(
				   const G4Track& track,
				   const G4Step& stepData
				   );
  virtual G4VParticleChange* AtRestDoIt(
				const G4Track& track,
				const G4Step& stepData
				);
  //////////////////////////
  // GPIL    //////////////
  /////////////////////////  
  virtual G4double AlongStepGetPhysicalInteractionLength(
						 const G4Track& track,
						 G4double  previousStepSize,
						 G4double  currentMinimumStep,
						 G4double& proposedSafety,
						 G4GPILSelection* selection);

  virtual G4double AtRestGetPhysicalInteractionLength(
					      const G4Track& track,
					      G4ForceCondition* condition
					      );

  virtual G4double PostStepGetPhysicalInteractionLength(
						const G4Track& track,
						G4double   previousStepSize,
						G4ForceCondition* condition
						) ;

protected:
    
    virtual void ResetNumberOfInteractionLengthLeft()
    {
      G4VProcess::theNumberOfInteractionLengthLeft =  -std::log( G4UniformRand() );
      theInitialNumberOfInteractionLength = G4VProcess::theNumberOfInteractionLengthLeft;
    }


private:

    G4double GetTotalNumberOfInteractionLengthTraversed()
    {
      return theInitialNumberOfInteractionLength
            -G4VProcess::theNumberOfInteractionLengthLeft;
    }

    G4double XBiasSurvivalProbability();
    G4double XBiasSecondaryWeight();
 
private:

  G4double eFactor; // enhancement factor to the cross-setion

  G4bool BiasMode2; // second algorithm?

  G4double theInitialNumberOfInteractionLength; 

  // New ParticleChange
  G4VParticleChange fParticleChange;

  // weight change applied at AlongStepDoIt(), mode 1
  G4double wc;

};

inline G4double GmWrapperProcess::GetEnhanceFactor () const
  { return eFactor;}
  
inline  void  GmWrapperProcess::SetEnhanceFactor (G4double dval)
  { eFactor = dval;}
#endif













