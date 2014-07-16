#include "GmEWPSBetheHeitlerModel.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4DataVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4LossTableManager.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSBetheHeitlerModel::GmEWPSBetheHeitlerModel(const G4ParticleDefinition* part,
					 const G4String& nam)
  : G4BetheHeitlerModel(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmEWPSBetheHeitlerModel::~GmEWPSBetheHeitlerModel()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void GmEWPSBetheHeitlerModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries,
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicGamma,
		    G4double energyCut,
		    G4double maxEnergy,
		    const G4Track& track)
{
 
  //PS<
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " GmEWPSBetheHeitlerModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif

  G4double positionZ = track.GetPosition().z();
  G4bool bIsApplicable = IsApplicable(positionZ);   //PS<
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    SampleSecondaries(secondaries, couple, aDynamicGamma, energyCut, maxEnergy);
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }   //PS>
  
  theWgt = track.GetWeight();   //PS
  theNewNSplit = 1;     // only split fat photons in zones 2 and 3
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel: Original photon wgt " << theWgt << G4endl;
#endif

  // The secondaries e+e- energies are sampled using the Bethe - Heitler
  // cross sections with Coulomb correction.  A modified version of the
  // random number techniques of Butcher & Messel is used (Nuc Phys
// 20(1960),15).
//
// GEANT4 internal units.
//
// Note 1 : Effects due to the breakdown of the Born approximation at
//          low energy are ignored.
// Note 2 : The differential cross section implicitly takes account of 
//          pair creation in both nuclear and atomic electron fields.
//          However triplet prodution is not generated.

  //- See in which splitting zone is the particle and set some configurations
  idZone = GetZone(positionZ);   //PS<
  if (idZone == PSZBeforeRR) {
    if (theWgt < 1.0) {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel: Original photon is non-fat." << G4endl;
#endif
      theWgt = CheckGasNonGas(track);
      if (theWgt == 0.) {
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel: Killing photon, rejected by Russian Roul." << G4endl;
#endif
	fParticleChange->ProposeTrackStatus(fStopAndKill);
	return;
      } else {
	//-	fParticleChange->ProposeParentWeight(theWgt);
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel: New wgt for photon " << theWgt << G4endl;
#endif
      }
    }
  } else {            // zones 2 and 3
    if (theWgt > 0.99) {
      theNewNSplit = theNSplit;
      theWgt *= theNSplitInv;
    }
  }   //PS>

  const G4Material* aMaterial = couple->GetMaterial();

  G4double GammaEnergy = aDynamicGamma->GetKineticEnergy();
  G4ParticleMomentum GammaDirection = aDynamicGamma->GetMomentumDirection();

  G4double epsil ;
  G4double epsil0 = electron_mass_c2/GammaEnergy ;
  if(epsil0 > 1.0) return;

  // do it fast if GammaEnergy < 2. MeV
  static const G4double Egsmall=2.*MeV;

  //----- Loop to generate multiple secondaries
  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS
    // select randomly one element constituing the material
    const G4Element* anElement = SelectRandomAtom(aMaterial, theGamma, GammaEnergy);

    if (GammaEnergy < Egsmall) {
      
      epsil = epsil0 + (0.5-epsil0)*G4UniformRand();
      
    } else {
      // now comes the case with GammaEnergy >= 2. MeV
      
      // Extract Coulomb factor for this Element
      G4double FZ = 8.*(anElement->GetIonisation()->GetlogZ3());
      if (GammaEnergy > 50.*MeV) FZ += 8.*(anElement->GetfCoulomb());
      
      // limits of the screening variable
      G4double screenfac = 136.*epsil0/(anElement->GetIonisation()->GetZ3());
      G4double screenmax = exp ((42.24 - FZ)/8.368) - 0.952 ;
      G4double screenmin = std::min(4.*screenfac,screenmax);
      
      // limits of the energy sampling
      G4double epsil1 = 0.5 - 0.5*sqrt(1. - screenmin/screenmax) ;
      G4double epsilmin = std::max(epsil0,epsil1) , epsilrange = 0.5 - epsilmin;
      
      //
      // sample the energy rate of the created electron (or positron)
      //
      //G4double epsil, screenvar, greject ;
      G4double  screenvar, greject ;
      
      G4double F10 = ScreenFunction1(screenmin) - FZ;
      G4double F20 = ScreenFunction2(screenmin) - FZ;
      G4double NormF1 = std::max(F10*epsilrange*epsilrange,0.); 
      G4double NormF2 = std::max(1.5*F20,0.);
      
      do {
	if ( NormF1/(NormF1+NormF2) > G4UniformRand() ) {
	  epsil = 0.5 - epsilrange*pow(G4UniformRand(), 0.333333);
	  screenvar = screenfac/(epsil*(1-epsil));
	  greject = (ScreenFunction1(screenvar) - FZ)/F10;
	  
	} else { 
	  epsil = epsilmin + epsilrange*G4UniformRand();
	  screenvar = screenfac/(epsil*(1-epsil));
	  greject = (ScreenFunction2(screenvar) - FZ)/F20;
	}
	
      } while( greject < G4UniformRand() );
      
    }   //  end of epsil sampling
    
    //
    // fixe charges randomly
    //
    
    G4double ElectTotEnergy, PositTotEnergy;
    if (G4UniformRand() > 0.5) {
      
      ElectTotEnergy = (1.-epsil)*GammaEnergy;
      PositTotEnergy = epsil*GammaEnergy;
      
    } else {
    
      PositTotEnergy = (1.-epsil)*GammaEnergy;
      ElectTotEnergy = epsil*GammaEnergy;
    }
    
    //
    // scattered electron (positron) angles. ( Z - axis along the parent photon)
    //
    //  universal distribution suggested by L. Urban 
    // (Geant3 manual (1993) Phys211),
    //  derived from Tsai distribution (Rev Mod Phys 49,421(1977))
    
    G4double u;
    const G4double a1 = 0.625 , a2 = 3.*a1 , d = 27. ;
    
    if (9./(9.+d) >G4UniformRand()) u= - log(G4UniformRand()*G4UniformRand())/a1;
    else                            u= - log(G4UniformRand()*G4UniformRand())/a2;
    
    G4double TetEl = u*electron_mass_c2/ElectTotEnergy;
    G4double TetPo = u*electron_mass_c2/PositTotEnergy;
    G4double Phi  = twopi * G4UniformRand();
    G4double dxEl= sin(TetEl)*cos(Phi),dyEl= sin(TetEl)*sin(Phi),dzEl=cos(TetEl);
    G4double dxPo=-sin(TetPo)*cos(Phi),dyPo=-sin(TetPo)*sin(Phi),dzPo=cos(TetPo);
    
    //
    // kinematic of the created pair
    //
    // the electron and positron are assumed to have a symetric
    // angular distribution with respect to the Z axis along the parent photon.
    
    G4double ElectKineEnergy = std::max(0.,ElectTotEnergy - electron_mass_c2);
    
    G4ThreeVector ElectDirection (dxEl, dyEl, dzEl);
    ElectDirection.rotateUz(GammaDirection);   
    
    // create G4DynamicParticle object for the particle1  
    G4DynamicParticle* aParticle1= new G4DynamicParticle(
							 theElectron,ElectDirection,ElectKineEnergy);
    
    // the e+ is always created (even with Ekine=0) for further annihilation.
    
    G4double PositKineEnergy = std::max(0.,PositTotEnergy - electron_mass_c2);
    
    G4ThreeVector PositDirection (dxPo, dyPo, dzPo);
    PositDirection.rotateUz(GammaDirection);   
    
    // create G4DynamicParticle object for the particle2 
    G4DynamicParticle* aParticle2= new G4DynamicParticle(
							 thePositron,PositDirection,PositKineEnergy);
    
    // Fill output vector
    secondaries->push_back(aParticle1);
    secondaries->push_back(aParticle2);
    secondaryWeights.push_back(theWgt);
    secondaryWeights.push_back(theWgt);
  }

  // kill incident photon
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);   
}

 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
