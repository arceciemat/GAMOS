#include "GmEWPSKleinNishinaComptonModel.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4DataVector.hh"
#include "G4ParticleChangeForGamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmEWPSKleinNishinaComptonModel::GmEWPSKleinNishinaComptonModel(const G4ParticleDefinition* part,
							       const G4String& nam)
  : G4KleinNishinaCompton(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSKleinNishinaComptonModel::~GmEWPSKleinNishinaComptonModel()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmEWPSKleinNishinaComptonModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries, 
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicGamma,
		    G4double energyCut,
		    G4double maxEnergy,
		    const G4Track& track)
{
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif
#endif

  G4ThreeVector position = track.GetPosition();

  G4bool bIsApplicable = IsApplicable(position.z());   //PS<
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreBremsstrahlungModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
#endif
    SampleSecondaries(secondaries, couple, aDynamicGamma, energyCut, maxEnergy);
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }   //PS>
  
  // The scattered gamma energy is sampled according to Klein - Nishina formula.
  // The random number techniques of Butcher & Messel are used 
  // (Nuc Phys 20(1960),15).
  // Note : Effects due to binding of atomic electrons are negliged.
  theWgt = track.GetWeight();   //PS

  theNewNSplit = theNSplit;

#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: Original photon wgt " << theWgt << G4endl;
#endif
#endif

  //----- See in which splitting zone is the particle and set some configurations
  idZone = GetZone(position.z());   //PS<
  if (idZone == PSZBeforeRR) {
    // for non-fat photons and non-gas material, do Russian Roulette before the event
    // for fat photons, do splitting, ie, keep theNewNSplit = theNSplit
    if (theWgt < 1.0) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: Original photon is non-fat" << G4endl;
#endif
#endif
      theWgt = CheckGasNonGas(track);
      if (theWgt == 0.) { 
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: Killing photon, rejected by Russian Roul." << G4endl;
#endif
#endif
	fParticleChange->ProposeTrackStatus(fStopAndKill);
	return;
      }
    } else {
      theWgt *= theNSplitInv;
    }
  } else {            // zones 2 and 3: splitting is done only for fat photons
    if (theWgt < 1.0) {
      theNewNSplit = 1;
    }
  }   //PS>

  //- In zone 1, all photons that get here are FAT, except those that are in a gas
  //- For all zones, non-fat photons are not split

  G4int good_gammas=0, good_elecs=0;
  G4bool bgood_gamma;
 
  G4double gamEnergy0 = aDynamicGamma->GetKineticEnergy();
  G4double E0_m = gamEnergy0 / electron_mass_c2 ;

  G4ThreeVector gamDirection0 = aDynamicGamma->GetMomentumDirection();

  //
  // sample the energy rate of the scattered gamma 
  //
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: idZone/NSplit/NSplitInv/NewNSplit " << idZone << " / " << theNSplit << " / " << theNSplitInv << " / " << theNewNSplit << G4endl;
#endif
#endif

  G4double epsilon, epsilonsq, onecost, sint2, greject ;

  G4double epsilon0   = 1./(1. + 2.*E0_m);
  G4double epsilon0sq = epsilon0*epsilon0;
  G4double alpha1     = - log(epsilon0);
  G4double alpha2     = 0.5*(1.- epsilon0sq);

  //----- Loop to generate multiple secondaries
  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS<
    bgood_gamma=FALSE;
    theNewWgt = theWgt;  //PS>
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel Initial wgt of new photon in splitting loop " << theNewWgt << G4endl;
#endif
#endif

    fParticleChange->SetVerboseLevel(0);
    do {
      if ( alpha1/(alpha1+alpha2) > G4UniformRand() ) {
	epsilon   = exp(-alpha1*G4UniformRand());   // epsilon0**r
	epsilonsq = epsilon*epsilon; 
	
      } else {
	epsilonsq = epsilon0sq + (1.- epsilon0sq)*G4UniformRand();
	epsilon   = sqrt(epsilonsq);
      };
      
      onecost = (1.- epsilon)/(epsilon*E0_m);
      sint2   = onecost*(2.-onecost);
      greject = 1. - epsilon*sint2/(1.+ epsilonsq);
      
    } while (greject < G4UniformRand());
 
    //
    // scattered gamma angles. ( Z - axis along the parent gamma)
    //
    
    G4double cosTeta = 1. - onecost; 
    G4double sinTeta = sqrt (sint2);
    G4double Phi     = twopi * G4UniformRand();
    G4double dirx = sinTeta*cos(Phi), diry = sinTeta*sin(Phi), dirz = cosTeta;
    
    //
    // update G4VParticleChange for the scattered gamma
    //
    
    G4ThreeVector gamDirection1 ( dirx,diry,dirz );
    gamDirection1.rotateUz(gamDirection0);

    // check if new gamma points towards the field of interest   //PS<
    if ( AcceptGamma(gamDirection1,position) ) {
      ++good_gammas;
      bgood_gamma = TRUE;
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel Photon accepted by direction, wgt unchanged " << theNewWgt << G4endl;
#endif
#endif
    } else {
      theNewWgt = RussRoul();
      if (theNewWgt > 0.) {
	++good_gammas;
	bgood_gamma = TRUE;
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel Photon accepted by Russian Roul. and made fat, wgt " << theNewWgt << G4endl;
#endif
#endif
      }
    }   //PS>

    G4double gamEnergy1 = 0.;
    //    if ( bgood_gamma && gamEnergy1>0. ) 
    if ( bgood_gamma ) {
      gamEnergy1 = epsilon*gamEnergy0;
      G4DynamicParticle* dp_g = new G4DynamicParticle(G4Gamma::Gamma(),gamDirection1,gamEnergy1);
      secondaries->push_back(dp_g);
      //      if (theNewNSplit == 1) {
      //	secondaryWeights.push_back(theWgt);
      //      } else {
	secondaryWeights.push_back(theNewWgt);
	//      }
    }

  //
  // kinematic of the scattered electron
  //

    //----- For scattered ELECTRON
    //- Zone 1:
    //  First, Russian Roulette with electron. If it survives, it is made fat. Originally,
    //  it has the weight of the original photon, so it can be fat or non fat until here
    //- Zones 2 and 3:
    //  No Russian Roulette
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: scattered electron in zone " << idZone << G4endl;
#endif
#endif
    if ( (idZone == PSZBeforeRR && RussRoul() > 0.) || (idZone != PSZBeforeRR) ) {
      G4double eKinEnergy = gamEnergy0 - gamEnergy1;

      if(eKinEnergy > DBL_MIN) {
	++good_elecs;
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: Accepted electron in zone " << idZone << G4endl;
#endif
#endif

	G4ThreeVector eDirection = gamEnergy0*gamDirection0 - gamEnergy1*gamDirection1;
	eDirection = eDirection.unit();
	theElectron = G4Electron::Electron();
	G4DynamicParticle* dp_e = new G4DynamicParticle(theElectron,eDirection,eKinEnergy);
	secondaries->push_back(dp_e);
	//- Zone 1: Always put the weight of the original photon, since:
	//  If theNewNSplit=1 (non-fat photon, in gas), do normal Compton (no splitting, no change in weight)
	//  Otherwise, first split (div. by Nsplit) and then make fat if pass RussRoul (mult. by Nsplit)
	//- Zones 2 and 3: electron will always be low-weight (since it comes from either a low-weight
	//  photon or from a fat photon which is split
	if (idZone == PSZBeforeRR) {
	  secondaryWeights.push_back(track.GetWeight());
	} else {
	  secondaryWeights.push_back(track.GetWeight()*theNSplitInv);
	}
      } else {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSKleinNishinaComptonModel: Electron rejected and killed by Russian Roul." << G4endl;
#endif
#endif
      }

    }      
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
