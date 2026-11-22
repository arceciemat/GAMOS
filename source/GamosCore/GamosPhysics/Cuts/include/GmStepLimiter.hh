// class description
//
// A "process" to be registered to the process manager of each particle,
// in the UserPhysicsList, in order to take into account the MaxAllowedStep
// defined in the G4UserLimits object attached to a logical volume.
//
// ------------------------------------------------------------
//                  23 Jan. 2004  H.Kurashige
// ------------------------------------------------------------
#ifndef GmStepLimiter_h
#define GmStepLimiter_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VProcess.hh"

class GmStepLimiter : public G4VProcess 
{
  public:  // with description     

     GmStepLimiter(const G4String& processName ="GmStepLimiter" );

     virtual ~GmStepLimiter();

     virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );

     virtual G4VParticleChange* PostStepDoIt(
                             const G4Track& ,
                             const G4Step& 
                            );
                            
  public:  // without description 
                                 
     //  no operation in  AtRestGPIL
     virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& ,
                             G4ForceCondition* 
                            ){ return -1.0; };
                            
     //  no operation in  AtRestDoIt      
     virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& ,
                             const G4Step&
                            ){return 0;};

     //  no operation in  AlongStepGPIL
     virtual G4double AlongStepGetPhysicalInteractionLength(
                             const G4Track&,
                             G4double  ,
                             G4double  ,
                             G4double& ,
                             G4GPILSelection*
                            ){ return -1.0; };

     //  no operation in  AlongStepDoIt
     virtual G4VParticleChange* AlongStepDoIt(
                             const G4Track& ,
                             const G4Step& 
                            ) {return 0;};

  private:
  
  // hide assignment operator as private 
      GmStepLimiter(GmStepLimiter&);
      GmStepLimiter& operator=(const GmStepLimiter& right);

};

#endif
