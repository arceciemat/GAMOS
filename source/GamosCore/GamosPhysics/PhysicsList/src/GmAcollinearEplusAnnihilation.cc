#include "GmAcollinearEplusAnnihilation.hh"
#include "G4PhysicalConstants.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4eeToTwoGammaModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmAcollinearEplusAnnihilation::GmAcollinearEplusAnnihilation(const G4String& name)
  : G4VEmProcess(name), isInitialised(false)
{
  theGamma = G4Gamma::Gamma();
  SetIntegral(true);
  SetBuildTableFlag(false);
  SetStartFromNullFlag(false);
  SetSecondaryParticle(theGamma);
  SetProcessSubType(fAnnihilation);
  enableAtRestDoIt = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmAcollinearEplusAnnihilation::~GmAcollinearEplusAnnihilation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmAcollinearEplusAnnihilation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double GmAcollinearEplusAnnihilation::AtRestGetPhysicalInteractionLength(
									   const G4Track&, G4ForceCondition* condition)
{
  *condition = NotForced;
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmAcollinearEplusAnnihilation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    if(!EmModel(0)) { SetEmModel(new G4eeToTwoGammaModel()); }
    EmModel(0)->SetLowEnergyLimit(MinKinEnergy());
    EmModel(0)->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, EmModel(0));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmAcollinearEplusAnnihilation::PrintInfo()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* GmAcollinearEplusAnnihilation::AtRestDoIt(const G4Track& aTrack,
							     const G4Step& )
//
// Performs the e+ e- annihilation when both particles are assumed at rest.
// It generates two photons with energy = electron_mass and with an angular difference
// between their directions following the distribution as measured by Colombino et al. (1965).
// The angular distribution is isotropic.
// GEANT4 internal units
//
// Note : Effects due to binding of atomic electrons are negliged.
{

  fParticleChange.InitializeForPostStep(aTrack);

  G4DynamicParticle *aGammaDP1, *aGammaDP2;
   
  G4double epsil = 0.5; //this variable can be used in the future to change the 
  //portion of the total energy going to the two photons according to the residual
  //center of mass momentum of the ee system at the time of annihilation 
  //and the direction of the two photons in the lab system.

  G4double TotalAvailableEnergy = 2.0*electron_mass_c2; //this should be modified in case
  //one wants to take into account the residual center of mass momentum of the ee system at the time of annihilation
  
  G4double Phot1Energy = epsil*TotalAvailableEnergy;
  
  G4double cosTeta = 2.*G4UniformRand()-1.; 
  G4double sinTeta = sqrt((1.-cosTeta)*(1.0 + cosTeta));
  G4double phi     = twopi * G4UniformRand();
  G4ThreeVector dir(sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
  G4ThreeVector normal_dir(-sinTeta*sin(phi), sinTeta*cos(phi), 0); //needed as rotation axis
  //  G4double theta = pi * G4UniformRand();
  //  G4double phi  = twopi * G4UniformRand();
  //  G4ThreeVector dir(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
  //  G4ThreeVector normal_dir(-sin(theta)*sin(phi), sin(theta)*cos(phi), 0); //needed as rotation axis
  if(sinTeta == 0)
    {
      G4ThreeVector x_dir(1,0,0);
      normal_dir = x_dir; //correct the rotation axis in case the previous gives a null vector
    }
  // e+ parameters
  G4double weight = aTrack.GetWeight();
  G4double time   = aTrack.GetGlobalTime();

  fParticleChange.SetNumberOfSecondaries(2);

  //first photon direction and polarization
  G4ThreeVector Phot1Direction = dir.unit();
  aGammaDP1 = new G4DynamicParticle (theGamma,Phot1Direction.unit(), Phot1Energy);
  phi = twopi * G4UniformRand();
  G4ThreeVector pol1(cos(phi), sin(phi), 0.0); //not clear to me how the polarization works.  copied from the G4eeToTwoGammaModel. 
  pol1.rotateUz(Phot1Direction.unit());
  aGammaDP1->SetPolarization(pol1.x(),pol1.y(),pol1.z());
  G4Track* track = new G4Track(aGammaDP1, time, aTrack.GetPosition());
  track->SetTouchableHandle(aTrack.GetTouchableHandle());
  track->SetWeight(weight); 
  pParticleChange->AddSecondary(track);

  //get the random angular difference between the two gamma directions according to the empirical formula
  //for B(theta) distribution for room temperature water in the article from Colombino et al,
  //Nuovo Cimento, vol.38 num.2, 16 July 1965
  bool good_delta_theta = false;
  G4double delta_theta = 0;
  while(!good_delta_theta)
    {
      delta_theta = 0.023*(2*G4UniformRand()-1); //limit between -23 and 23 mRad for 0.001 accuracy
      G4double draw = G4UniformRand();
      G4double prob = (346.5*pow(pow(0.124*delta_theta*1000,2)+1,2)+5330*(pow(0.124*delta_theta*1000,2)+1)-4264)/pow(pow(0.124*delta_theta*1000,2)+1,5)/(346.5+5330-4264);
      if(draw<prob)
	{
	  good_delta_theta = true;
	}
    }

  //second photon direction and polarization
  G4double Phot2Energy =(1.-epsil)*TotalAvailableEnergy;
  G4ThreeVector Phot2Direction = Phot1Direction;
  Phot2Direction.rotate(pi+delta_theta,normal_dir.unit());
  G4double rotation = pi*G4UniformRand();
  Phot2Direction.rotate(rotation,Phot1Direction);
  aGammaDP2 = new G4DynamicParticle (theGamma,Phot2Direction.unit(), Phot2Energy);
  aGammaDP2->SetPolarization(-pol1.x(),-pol1.y(),-pol1.z());
  track = new G4Track(aGammaDP2, time, aTrack.GetPosition());
  track->SetTouchableHandle(aTrack.GetTouchableHandle());
  track->SetWeight(weight); 
  pParticleChange->AddSecondary(track);

  // Kill the incident positron
  //
  fParticleChange.ProposeTrackStatus(fStopAndKill);
  return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
