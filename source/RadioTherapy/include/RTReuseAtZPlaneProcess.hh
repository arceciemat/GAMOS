#ifndef RTReuseAtZPlaneProcess_h 
#define RTReuseAtZPlaneProcess_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleChange.hh"
class GmVFilter;
enum MWR {NOMir, MirX, MirY, MirXY};

class RTReuseAtZPlaneProcess : public G4VDiscreteProcess
{
  public: // with description
  //  constructor requires the process name and type
  RTReuseAtZPlaneProcess( const G4String& aName =  "RTReuseAtZPlane",
                 G4ProcessType   aType = fUserDefined );

  public: 
  //  destructor 
  virtual ~RTReuseAtZPlaneProcess(){};

  public:

  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );
  
  virtual G4VParticleChange* PostStepDoIt(
                             const G4Track& track,
                             const G4Step&  stepData
                            );

  virtual G4double GetMeanFreePath(const G4Track& aTrack,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                                                               );

private:
  G4double theZPlane;
  G4int theNReuse;
  MWR MirrorWhenReuse;
  GmVFilter* theFilter;
};

#endif
