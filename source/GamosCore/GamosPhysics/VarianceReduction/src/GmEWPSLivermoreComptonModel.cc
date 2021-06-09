#include "GmEWPSLivermoreComptonModel.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
#include "G4ParticleChangeForGamma.hh"
#include "G4VEMDataSet.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSLivermoreComptonModel::GmEWPSLivermoreComptonModel(const G4ParticleDefinition* part,
						 const G4String& nam)
  :G4LivermoreComptonModel(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmEWPSLivermoreComptonModel::~GmEWPSLivermoreComptonModel()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void GmEWPSLivermoreComptonModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries, 
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicGamma,
		    G4double ,
		    G4double ,
		    const G4Track& track)
{
  // The scattered gamma energy is sampled according to Klein - Nishina formula.
  // then accepted or rejected depending on the Scattering Function multiplied
  // by factor from Klein - Nishina formula.
  // Expression of the angular distribution as Klein Nishina
  // angular and energy distribution and Scattering fuctions is taken from
  // D. E. Cullen "A simple model of photon transport" Nucl. Instr. Meth.
  // Phys. Res. B 101 (1995). Method of sampling with form factors is different
  // data are interpolated while in the article they are fitted.
  // Reference to the article is from J. Stepanek New Photon, Positron
  // and Electron Interaction Data for GEANT in Energy Range from 1 eV to 10
  // TeV (draft).
  // The random number techniques of Butcher & Messel are used
  // (Nucl Phys 20(1960),15).

  G4double wgt = track.GetWeight() * theNSplitInv;   //PS

  G4ThreeVector position = track.GetPosition();

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Original photon wgt " << wgt << G4endl;
#endif

  theNewNSplit = theNSplit;

  //----- See in which splitting zone is the particle and set some configurations
  idZone = GetZone(position.z());
  if (idZone == 1) {
    // for non-fat photons and non-gas material, do Russian Roulette before the event
    // for fat photons, do splitting, ie, keep theNewNSplit = theNSplit
    if (wgt < 1.0) {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Original photon is non-fat" << G4endl;
#endif
      wgt = CheckGasNonGas(track);
      if (wgt == -1.0) {
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Killing photon, rejected by Russian Roul." << G4endl;
#endif
	fParticleChange->ProposeTrackStatus(fStopAndKill);
	return;
      }
    }
  } else {            // zones 2 and 3: splitting is done only for fat photons
    if (wgt < 1.0) {
      theNewNSplit = 1;
    }
  }

  //- In zone 1, all photons that get here are FAT, except those that are in a gas
  //- For all zones, non-fat photons are not split

  //----- Loop to generate multiple secondaries
  G4double wgt_ii;
  G4int good_gammas=0, good_elecs=0;
  G4bool bgood_gamma;

  G4double photonEnergy0 = aDynamicGamma->GetKineticEnergy();

  if (verboseLevel > 3) {
    G4cout << "GmEWPSLivermoreComptonModel::SampleSecondaries() E(MeV)= " 
	   << photonEnergy0/MeV << " in " << couple->GetMaterial()->GetName() 
	   << G4endl;
  }
  
  // low-energy gamma is absorpted by this process
  if (photonEnergy0 <= lowEnergyLimit) 
    {
      fParticleChange->ProposeTrackStatus(fStopAndKill);
      fParticleChange->SetProposedKineticEnergy(0.);
      fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0);
      return ;
    }

  G4double e0m = photonEnergy0 / electron_mass_c2 ;
  G4ParticleMomentum photonDirection0 = aDynamicGamma->GetMomentumDirection();

  // Select randomly one element in the current material
  //  G4int Z = crossSectionHandler->SelectRandomAtom(couple,photonEnergy0);
  const G4ParticleDefinition* particle =  aDynamicGamma->GetDefinition();
  const G4Element* elm = SelectRandomAtom(couple,particle,photonEnergy0);
  G4int Z = (G4int)elm->GetZ();

  G4double epsilon0 = 1. / (1. + 2. * e0m);
  G4double epsilon0Sq = epsilon0 * epsilon0;
  G4double alpha1 = -std::log(epsilon0);
  G4double alpha2 = 0.5 * (1. - epsilon0Sq);

  G4double wlPhoton = h_Planck*c_light/photonEnergy0;

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: idZone/NSplit/NSplitInv/NewNSplit " << idZone << " / " << theNSplit << " / " << theNSplitInv << " / " << theNewNSplit << G4endl;
#endif
  G4double epsilon;
  G4double epsilonSq;
  G4double oneCosT;
  G4double sinT2;
  G4double gReject;

  for (G4int ii=0; ii<theNewNSplit; ii++) {
    bgood_gamma=FALSE;
    if (theNewNSplit==1) {
      wgt_ii = wgt;
    } else {
      wgt_ii = wgt*theNSplitInv;
    }
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Initial wgt of new photon in splitting loop " << wgt_ii << G4endl;
#endif
    fParticleChange->SetVerboseLevel(0);
    
    // Sample the energy of the scattered photon
    
    do 
      {
	if ( alpha1/(alpha1+alpha2) > G4UniformRand())
	  {
	    // std::pow(epsilon0,G4UniformRand())
	    epsilon = std::exp(-alpha1 * G4UniformRand());  
	    epsilonSq = epsilon * epsilon;
	  }
	else
	  {
	    epsilonSq = epsilon0Sq + (1. - epsilon0Sq) * G4UniformRand();
	    epsilon = std::sqrt(epsilonSq);
	  }
	
	oneCosT = (1. - epsilon) / ( epsilon * e0m);
	sinT2 = oneCosT * (2. - oneCosT);
	G4double x = std::sqrt(oneCosT/2.) / (wlPhoton/cm);
	G4double scatteringFunction = scatterFunctionData->FindValue(x,Z-1);
	gReject = (1. - epsilon * sinT2 / (1. + epsilonSq)) * scatteringFunction;
	
      } while(gReject < G4UniformRand()*Z);
    
    G4double cosTheta = 1. - oneCosT;
    G4double sinTheta = std::sqrt (sinT2);
    G4double phi = twopi * G4UniformRand() ;
    G4double dirx = sinTheta * std::cos(phi);
    G4double diry = sinTheta * std::sin(phi);
    G4double dirz = cosTheta ;
    
    // Update G4VParticleChange for the scattered photon
    G4ThreeVector photonDirection1(dirx,diry,dirz);
    photonDirection1.rotateUz(photonDirection0);
    
    // check if new photon points towards the field of interest
    if ( AcceptGamma(photonDirection1,position) ) {
      ++good_gammas;
      bgood_gamma = TRUE;
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << " Photon accepted by direction, wgt unchanged " << wgt_ii << G4endl;
#endif
    } else {
      wgt_ii = RussRoul();
      if (wgt_ii > 0.) {
        ++good_gammas;
        bgood_gamma=TRUE;
#ifndef GAMOS_NO_VERBOSE
        if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Photon accepted by Russian Roul. and made fat, wgt " << wgt_ii << G4endl;
#endif
      }
    }
    
    G4double photonEnergy1 = photonEnergy0; // no Doppler broadening
    
    if ( bgood_gamma && photonEnergy1>0. ) 
      {
	//only once    fParticleChange->ProposeMomentumDirection(photonDirection1);
	// create G4DynamicParticle object for the photon
	G4DynamicParticle* dp_g = new G4DynamicParticle(G4Gamma::Gamma(),photonDirection1,photonEnergy1);
	secondaries->push_back(dp_g);
	if (theNewNSplit == 1) {
	  secondaryWeights.push_back(wgt);
	} else {
	  secondaryWeights.push_back(wgt_ii);
	}
     	//only once      fParticleChange->SetProposedKineticEnergy(photonEnergy1) ;
      }
    
    // Kinematics of the scattered electron
    //----- For scattered ELECTRON
    //- Zone 1:
    //  First, Russian Roulette with electron. If it survives, it is made fat. Originally,
    //  it has the weight of the original photon, so it can be fat or non fat until here
    //- Zones 2 and 3:
    //  No Russian Roulette
    
    if ( (idZone==1 && RussRoul()>0.) || (idZone==2) || (idZone==3) ) {
      G4double eKineticEnergy = photonEnergy0 - photonEnergy1;
	// protection against negative final energy: no e- is created
	if(eKineticEnergy < 0.0) {
	  fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0 - photonEnergy1);
	  return;
	}	  
	++good_elecs;
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Accepted electron in zone " << idZone << G4endl;
#endif
	
	G4double eTotalEnergy = eKineticEnergy + electron_mass_c2;
	
	G4double electronE = photonEnergy0 * (1. - epsilon) + electron_mass_c2; 
	G4double electronP2 = electronE*electronE - electron_mass_c2*electron_mass_c2;
	G4double sinThetaE = -1.;
	G4double cosThetaE = 0.;
	if (electronP2 > 0.)
	  {
	    cosThetaE = (eTotalEnergy + photonEnergy1 )* (1. - epsilon) / std::sqrt(electronP2);
	    sinThetaE = -1. * sqrt(1. - cosThetaE * cosThetaE); 
	  }
	
	G4double eDirX = sinThetaE * std::cos(phi);
	G4double eDirY = sinThetaE * std::sin(phi);
	G4double eDirZ = cosThetaE;
	
	G4ThreeVector eDirection(eDirX,eDirY,eDirZ);
	eDirection.rotateUz(photonDirection0);
	
	// SI - The range test has been removed wrt original G4LowEnergyCompton class
	// create G4DynamicParticle object for the electron
	G4DynamicParticle* dp_e = new G4DynamicParticle(G4Electron::Electron(),eDirection,eKineticEnergy);
	secondaries->push_back(dp_e);
	//- Zone 1: Always put the weight of the original photon, since:
	//  If theNewNSplit=1 (non-fat photon, in gas), do normal Compton (no splitting, no change in weight)
	//  Otherwise, first split (div. by Nsplit) and then make fat (mult. by Nsplit)
	//- Zones 2 and 3: electron will always be low-weight (since it comes from either a low-weight
	//  photon or from a fat photon which is split
	if (idZone == 1) {
	  secondaryWeights.push_back(wgt);
	} else {
	  if (wgt < 1.0) {
	    secondaryWeights.push_back(wgt);
	  } else {
	    secondaryWeights.push_back(wgt*theNSplitInv);
	  }
	}
      } else {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreComptonModel: Electron rejected and killed by Russian Roul." << G4endl;
#endif
    } 
  } // end splitting loop
  
}
//??   fParticleChange->ProposeLocalEnergyDeposit(bindingE);
  
