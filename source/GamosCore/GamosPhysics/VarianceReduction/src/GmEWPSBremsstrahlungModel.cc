#include "GmEWPSBremsstrahlungModel.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"
#include "G4ProductionCutsTable.hh"
#include "G4DataVector.hh"
#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmEWPSBremsstrahlungModel::GmEWPSBremsstrahlungModel(const G4ParticleDefinition* part,
                                               const G4String& nam)
  : G4eBremsstrahlungModel(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSBremsstrahlungModel::~GmEWPSBremsstrahlungModel()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void GmEWPSBremsstrahlungModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries,
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* dp,
		    G4double tmin,
		    G4double maxEnergy,
		    const G4Track& track)
// The emitted gamma energy is sampled using a parametrized formula 
// from L. Urban.
// This parametrization is derived from :
//    cross-section values of Seltzer and Berger for electron energies
//    1 keV - 10 GeV,
//    screened Bethe Heilter differential cross section above 10 GeV,
//    Migdal corrections in both case.
//  Seltzer & Berger: Nim B 12:95 (1985)
//  Nelson, Hirayama & Rogers: Technical report 265 SLAC (1985)
//  Migdal: Phys Rev 103:1811 (1956); Messel & Crawford: Pergamon Press (1970)
//
// A modified version of the random number techniques of Butcher&Messel is used
//    (Nuc Phys 20(1960),15).
{
 //PS<
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " GmEWPSBremsstrahlungModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif
  G4bool bIsApplicable = IsApplicable(track.GetPosition().z());  
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmEWPSBremsstrahlungModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    //g494    SampleSecondaries(secondaries, currentCouple, dp, tmin, 100.*TeV );
    SampleSecondaries(secondaries, couple, dp, tmin, 100.*TeV );
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }

  theWgt = track.GetWeight();   //PS<
  theNewNSplit = theNSplit;
  if( theWgt > 0.99 ) {
    theWgt *= theNSplitInv;   
  } else {
    theNewNSplit = 1;
  }   //PS>
  G4ThreeVector position = track.GetPosition();

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "  GmEWPSBremsstrahlungModel::SampleSecondaries wgt " << theWgt << " theNSplit " << theNewNSplit << G4endl;
#endif

  G4double kineticEnergy = dp->GetKineticEnergy();
  G4double totalEnergy = kineticEnergy + electron_mass_c2;
  G4ThreeVector direction = dp->GetMomentumDirection();
  G4ThreeVector gammaDirection;
  G4double tmax = min(maxEnergy, kineticEnergy);
  if(tmin >= tmax) return;

//
// GEANT4 internal units.
//
  static const G4double
     ah10 = 4.67733E+00, ah11 =-6.19012E-01, ah12 = 2.02225E-02,
     ah20 =-7.34101E+00, ah21 = 1.00462E+00, ah22 =-3.20985E-02,
     ah30 = 2.93119E+00, ah31 =-4.03761E-01, ah32 = 1.25153E-02;

  static const G4double
     bh10 = 4.23071E+00, bh11 =-6.10995E-01, bh12 = 1.95531E-02,
     bh20 =-7.12527E+00, bh21 = 9.69160E-01, bh22 =-2.74255E-02,
     bh30 = 2.69925E+00, bh31 =-3.63283E-01, bh32 = 9.55316E-03;

  static const G4double
     al00 =-2.05398E+00, al01 = 2.38815E-02, al02 = 5.25483E-04,
     al10 =-7.69748E-02, al11 =-6.91499E-02, al12 = 2.22453E-03,
     al20 = 4.06463E-02, al21 =-1.01281E-02, al22 = 3.40919E-04;

  static const G4double
     bl00 = 1.04133E+00, bl01 =-9.43291E-03, bl02 =-4.54758E-04,
     bl10 = 1.19253E-01, bl11 = 4.07467E-02, bl12 =-1.30718E-03,
     bl20 =-1.59391E-02, bl21 = 7.27752E-03, bl22 =-1.94405E-04;

  static const G4double tlow = 1.*MeV;

  G4double gammaEnergy = 0.;
  G4bool LPMOK = false;
  const G4Material* material = couple->GetMaterial();

  //Sample gamma direction
  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS
    
    G4double u;
    const G4double a1 = 0.625 , a2 = 3.*a1 , d = 27. ;
    
    if (9./(9.+d) > G4UniformRand()) u = - log(G4UniformRand()*G4UniformRand())/a1;
    else                          u = - log(G4UniformRand()*G4UniformRand())/a2;
    
    G4double theta = u*electron_mass_c2/totalEnergy;
    
    G4double sint = sin(theta);
    
    G4double phi = twopi * G4UniformRand() ;
    
    gammaDirection = G4ThreeVector(sint*cos(phi),sint*sin(phi), cos(theta));
    gammaDirection.rotateUz(direction);

#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmEWPSLivermoreBremsstrahlungModel::SampleSecondariesPS  gammaDir " << gammaDirection << " elecDir " << dp->GetMomentumDirection() << G4endl;
#endif

    if( AcceptGamma(gammaDirection,position) ) {   //PS<
      theNewWgt = theWgt;
    } else {
        //- Try Russian Roulette
      if ( RussRoul() ) {
	theNewWgt = 1.;
      } else {
	continue;
      }
    }    //PS>
 
    // select randomly one element constituing the material
    const G4Element* anElement = SelectRandomAtom(couple);
    
    // Extract Z factors for this Element
    G4double lnZ = 3.*(anElement->GetIonisation()->GetlogZ3());
    G4double FZ = lnZ* (4.- 0.55*lnZ);
    G4double ZZ = anElement->GetIonisation()->GetZZ3();
    
    // limits of the energy sampling
    G4double xmin     = tmin/kineticEnergy;
    G4double xmax     = tmax/kineticEnergy;
    G4double kappa    = 0.0;
    if(xmax >= 1.) xmax = 1.;
    else     kappa    = log(xmax)/log(xmin);
    G4double epsilmin = tmin/totalEnergy;
    G4double epsilmax = tmax/totalEnergy;
    
    // Migdal factor
    G4double MigdalFactor = (material->GetElectronDensity())*MigdalConstant
      / (epsilmax*epsilmax);
    
    G4double x, epsil, greject, migdal, grejmax, q;
    G4double U  = log(kineticEnergy/electron_mass_c2);
    G4double U2 = U*U;
    
    // precalculated parameters
    G4double ah, bh;
    G4double screenfac = 0.0;
    
    if (kineticEnergy > tlow) {
      
      G4double ah1 = ah10 + ZZ* (ah11 + ZZ* ah12);
      G4double ah2 = ah20 + ZZ* (ah21 + ZZ* ah22);
      G4double ah3 = ah30 + ZZ* (ah31 + ZZ* ah32);
      
      G4double bh1 = bh10 + ZZ* (bh11 + ZZ* bh12);
      G4double bh2 = bh20 + ZZ* (bh21 + ZZ* bh22);
      G4double bh3 = bh30 + ZZ* (bh31 + ZZ* bh32);
      
      ah = 1.   + (ah1*U2 + ah2*U + ah3) / (U2*U);
      bh = 0.75 + (bh1*U2 + bh2*U + bh3) / (U2*U);
      
      // limit of the screening variable
      screenfac =
	136.*electron_mass_c2/((anElement->GetIonisation()->GetZ3())*totalEnergy);
      G4double screenmin = screenfac*epsilmin/(1.-epsilmin);
      
      // Compute the maximum of the rejection function
      G4double F1 = max(ScreenFunction1(screenmin) - FZ ,0.);
      G4double F2 = max(ScreenFunction2(screenmin) - FZ ,0.);
      grejmax = (F1 - epsilmin* (F1*ah - bh*epsilmin*F2))/(42.392 - FZ);
      
    } else {  
      
      G4double al0 = al00 + ZZ* (al01 + ZZ* al02);
      G4double al1 = al10 + ZZ* (al11 + ZZ* al12);
      G4double al2 = al20 + ZZ* (al21 + ZZ* al22);
      
      G4double bl0 = bl00 + ZZ* (bl01 + ZZ* bl02);
      G4double bl1 = bl10 + ZZ* (bl11 + ZZ* bl12);
      G4double bl2 = bl20 + ZZ* (bl21 + ZZ* bl22);
      
      ah = al0 + al1*U + al2*U2;
      bh = bl0 + bl1*U + bl2*U2;
      
      // Compute the maximum of the rejection function
      grejmax = max(1. + xmin* (ah + bh*xmin), 1.+ah+bh);
      G4double xm = -ah/(2.*bh);
      if ( xmin < xm && xm < xmax) grejmax = max(grejmax, 1.+ xm* (ah + bh*xm));
    }
    
    //
    //  sample the energy rate of the emitted gamma for e- kin energy > 1 MeV
    //
    
    do {
      if (kineticEnergy > tlow) {
	do {
	  q = G4UniformRand();
	  x = pow(xmin, q + kappa*(1.0 - q));
	  epsil = x*kineticEnergy/totalEnergy;
	  G4double screenvar = screenfac*epsil/(1.0-epsil);
	  G4double F1 = max(ScreenFunction1(screenvar) - FZ ,0.);
	  G4double F2 = max(ScreenFunction2(screenvar) - FZ ,0.);
	  migdal = (1. + MigdalFactor)/(1. + MigdalFactor/(x*x));
	  greject = migdal*(F1 - epsil* (ah*F1 - bh*epsil*F2))/(42.392 - FZ);
	  /*
	    if ( greject > grejmax ) {
            G4cout << "### GmEWPSBremsstrahlungModel Warning: Majoranta exceeded! "
	    << greject << " > " << grejmax
	    << " x= " << x 
	    << " e= " << kineticEnergy
	    << G4endl;
	    }
	  */	
	} while( greject < G4UniformRand()*grejmax );
	
      } else {  
	
	do {
	  q = G4UniformRand();
	  x = pow(xmin, q + kappa*(1.0 - q));
	  migdal = (1. + MigdalFactor)/(1. + MigdalFactor/(x*x));  
	  greject = migdal*(1. + x* (ah + bh*x));
	  /*
	    if ( greject > grejmax ) {
	    G4cout << "### GmEWPSBremsstrahlungModel Warning: Majoranta exceeded! " 
	    << greject << " > " << grejmax 
	    << " x= " << x 
	    << " e= " << kineticEnergy
	    << G4endl;
	    }
	  */
	} while( greject < G4UniformRand()*grejmax );
      }
      /*
	if(x > 0.999) {
	G4cout << "### GmEWPSBremsstrahlungModel Warning: e= " << kineticEnergy
	<< " tlow= " << tlow
	<< " x= " << x
	<< " greject= " << greject 
	<< " grejmax= " << grejmax
	<< " migdal= " << migdal
	<< G4endl; 
	//      if(x >= 1.0) G4Exception("X=1");
	}
      */
      gammaEnergy = x*kineticEnergy; 
      
      if (LPMFlag()) {
	// take into account the supression due to the LPM effect
	if (G4UniformRand() <= SupressionFunction(material,kineticEnergy,
						  gammaEnergy))
	  LPMOK = true;
      }
      else LPMOK = true;
      
    } while (!LPMOK);
    
    //
    //  angles of the emitted gamma. ( Z - axis along the parent particle)
    //
    //  universal distribution suggested by L. Urban 
    // (Geant3 manual (1993) Phys211),
    //  derived from Tsai distribution (Rev Mod Phys 49,421(1977))
    
    // create G4DynamicParticle object for the Gamma
    G4DynamicParticle* g = new G4DynamicParticle(theGamma,gammaDirection,
						 gammaEnergy);
    secondaries->push_back(g);
    secondaryWeights.push_back( theNewWgt );
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBremsstrahlungModel new particle of weight " << theNewWgt << G4endl;
#endif
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBremsstrahlungModel N SECOS = " << secondaries->size() << G4endl;
#endif
  if( secondaries->size() > 0 ) {
    G4double totMomentum = sqrt(kineticEnergy*(totalEnergy + electron_mass_c2));
    G4ThreeVector dir = totMomentum*direction - gammaEnergy*gammaDirection;
    direction = dir.unit();

    // energy of primary
    G4double finalE = kineticEnergy - gammaEnergy;
    
    // stop tracking and create new secondary instead of primary
    //-  It is = 1.797693134862316e+308   G4cout << " SecondaryThreshold " << SecondaryThreshold() << " particle " << particle->GetParticleName() << G4endl;
    
    /* PS    if(gammaEnergy > SecondaryThreshold()) {
      fParticleChange->ProposeTrackStatus(fStopAndKill);
      fParticleChange->SetProposedKineticEnergy(0.0);
      G4DynamicParticle* el = 
	new G4DynamicParticle(const_cast<G4ParticleDefinition*>(particle),
			      direction, finalE);
      secondaries->push_back(el);
      
      // continue tracking
      } else { PS*/
      fParticleChange->SetProposedMomentumDirection(direction);
      fParticleChange->SetProposedKineticEnergy(finalE);
      //PS    }
  }
}
