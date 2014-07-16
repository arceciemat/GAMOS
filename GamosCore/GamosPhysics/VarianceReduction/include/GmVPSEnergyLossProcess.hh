#ifndef GmVPSEnergyLossProcess_hh
#define GmVPSEnergyLossProcess_hh 1
//
// Class Description
//
// This is the base class to implement energy loss particle splitting processes

#include "globals.hh"
#include "G4ios.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ParticleChange.hh"
class GmVPSModelHelper;

class GmVPSEnergyLossProcess : public G4VEnergyLossProcess
{
public: // with description
  //  constructor requires the process name and type
  GmVPSEnergyLossProcess( const G4String& aName = "GmParticleSplitting",
			  G4ProcessType aType = fUserDefined );

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition* p,
					   const G4ParticleDefinition* p2) = 0;

public:
  //  destructor
  virtual ~GmVPSEnergyLossProcess(){};
  void SetWrappedProcess( G4VEnergyLossProcess* wp ) {
    theWrappedProcess = wp; }

public:
  virtual G4VParticleChange* PostStepDoIt(const G4Track& track, const G4Step& stepData);

  GmVPSModelHelper* GetPSModel() const {
    return thePSModel; }

  void AddPSEmModel( GmVPSModelHelper* psmod ) {
    thePSModel = psmod; 
    //     G4cout << this << " AddPSEmModel " << thePSModel << " " << GetProcessName() << G4endl;
  }

protected:
  G4VEnergyLossProcess* theWrappedProcess;

  //  G4ParticleChange fParticleChange;
  GmVPSModelHelper* thePSModel;
};

#endif
