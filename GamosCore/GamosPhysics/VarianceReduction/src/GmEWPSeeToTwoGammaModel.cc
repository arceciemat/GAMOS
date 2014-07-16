#include "GmEWPSeeToTwoGammaModel.hh"
#include "G4TrackStatus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4ParticleChangeForGamma.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmEWPSeeToTwoGammaModel::GmEWPSeeToTwoGammaModel(const G4ParticleDefinition* part,
                                         const G4String& nam)
  : G4eeToTwoGammaModel(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSeeToTwoGammaModel::~GmEWPSeeToTwoGammaModel()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
void GmEWPSeeToTwoGammaModel::SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries,
				   std::vector<G4double>& secondaryWeights,
				   const G4MaterialCutsCouple* couple,
				   const G4DynamicParticle* dp,
				   G4double tmin,
				   G4double ,
				   const G4Track& track)
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << "GmEWPSeeToTwoGammaModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif

  G4bool bIsApplicable = IsApplicable(track.GetPosition().z());   //PS<
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmEWPSeeToTwoGammaModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    //g494    SampleSecondaries(secondaries, currentCouple, dp, tmin, 100.*TeV );
    SampleSecondaries(secondaries, couple, dp, tmin, 100.*TeV );
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }

  theNewNSplit  = theNSplit;
  // Do splitting only for FAT positrons
  theWgt = track.GetWeight();   //PS<
  if( theWgt > 0.99 ) {
    theWgt *= theNSplitInv;   
  } else {
    theNewNSplit = 1;
  }   //PS>
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "  GmEWPSeeToTwoGammaModel::SampleSecondariesPS wgt " << theWgt << " NSplit " << theNewNSplit << G4endl;
#endif
  //PS>
  G4double PositKinEnergy = dp->GetKineticEnergy();

 // Loop over PostStepDoIt method to generate multiple secondaries
  for (G4int ii=0; ii<theNewNSplit; ii++) {
    
    // Case at rest
    if(PositKinEnergy == 0.0) {
      G4double cost = 2.*G4UniformRand()-1.;
      G4double sint = sqrt((1. - cost)*(1. + cost));
      G4double phi  = twopi * G4UniformRand();
      G4ThreeVector dir (sint*cos(phi), sint*sin(phi), cost);
      // check if gamma1 points towards the field of interest
      if ( AcceptGamma(dir,track.GetPosition()) ) {
        G4DynamicParticle* aGamma1 = new G4DynamicParticle(theGamma, dir, electron_mass_c2); // why electron_mass?
        secondaries->push_back(aGamma1);
        secondaryWeights.push_back(theWgt);
#ifndef GAMOS_NO_VERBOSE
        if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon1 accepted by direction, dir " << dir << G4endl;
#endif
      } else {
        //- Try Russian Roulette
        if ( RussRoul() ) {
          G4DynamicParticle* aGamma1 = new G4DynamicParticle(theGamma, dir, electron_mass_c2); // why electron_mass?
          secondaries->push_back(aGamma1);
          secondaryWeights.push_back(1.0);
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon1 accepted by Russian Roul. and made fat, dir " << dir << G4endl;
#endif
        } else {
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon1 rejected by Russian Roul., dir " << dir << G4endl;
#endif
        }
      }
      // check if gamma2 points towards the field of interest
      if ( AcceptGamma(-dir,track.GetPosition()) ) {
        G4DynamicParticle* aGamma2 = new G4DynamicParticle(theGamma, -dir, electron_mass_c2);
        secondaries->push_back(aGamma2);
        secondaryWeights.push_back(theWgt);
#ifndef GAMOS_NO_VERBOSE
        if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon2 accepted by direction, dir " << -dir << G4endl;
#endif
      } else {
        //- Try Russian Roulette
        if ( CLHEP::RandFlat::shoot() < theNSplitInv ) {
          G4DynamicParticle* aGamma2 = new G4DynamicParticle(theGamma, -dir, electron_mass_c2);
          secondaries->push_back(aGamma2);
          secondaryWeights.push_back(1.0);
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon2 accepted by Russian Roul. and made fat, dir " << -dir << G4endl;
#endif
        } else {
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon2 rejected by Russian Roul., dir " << -dir << G4endl;
#endif
        }
      }
      
    } else {

      G4ThreeVector PositDirection = dp->GetMomentumDirection();
      
      G4double tau     = PositKinEnergy/electron_mass_c2;
      G4double gam     = tau + 1.0;
      G4double tau2    = tau + 2.0;
      G4double sqgrate = sqrt(tau/tau2)*0.5;
      G4double sqg2m1  = sqrt(tau*tau2);
      
      // limits of the energy sampling
      G4double epsilmin = 0.5 - sqgrate;
      G4double epsilmax = 0.5 + sqgrate;
      G4double epsilqot = epsilmax/epsilmin;
      
      //
      // sample the energy rate of the created gammas
      //
      G4double epsil, greject;
      
      do {
	epsil = epsilmin*pow(epsilqot,G4UniformRand());
	greject = 1. - epsil + (2.*gam*epsil-1.)/(epsil*tau2*tau2);
      } while( greject < G4UniformRand() );
      
      //
      // scattered Gamma angles. ( Z - axis along the parent positron)
      //
      
      G4double cost = (epsil*tau2-1.)/(epsil*sqg2m1);
      if(std::abs(cost) > 1.0) {
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(warningVerb) ) G4cout << "### GmEWPSeeToTwoGammaModel WARNING cost= " << cost
	       << " positron Ekin(MeV)= " << PositKinEnergy
	       << " gamma epsil= " << epsil
	       << G4endl;
#endif
	if(cost > 1.0) cost = 1.0;
	else cost = -1.0; 
      }
      G4double sint = sqrt((1.+cost)*(1.-cost));
      G4double phi  = twopi * G4UniformRand();
      
      G4double dirx = sint*cos(phi) , diry = sint*sin(phi) , dirz = cost;
      
      //
      // kinematic of the created pair
      //
      
      G4double TotalAvailableEnergy = PositKinEnergy + 2.0*electron_mass_c2;
      G4double Phot1Energy = epsil*TotalAvailableEnergy;

      G4ThreeVector Phot1Direction (dirx, diry, dirz);
      Phot1Direction.rotateUz(PositDirection);
      // check if gamma1 points towards the field of interest
      if ( AcceptGamma(Phot1Direction,track.GetPosition()) ) {
        G4DynamicParticle* aGamma1 = new G4DynamicParticle(theGamma, Phot1Direction, Phot1Energy);
        secondaries->push_back(aGamma1);
        secondaryWeights.push_back(theWgt);
#ifndef GAMOS_NO_VERBOSE
        if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon1 accepted by direction, dir " << Phot1Direction << G4endl;
#endif
      } else {
        //- Try Russian Roulette
        if ( CLHEP::RandFlat::shoot() < theNSplitInv ) {
          G4DynamicParticle* aGamma1 = new G4DynamicParticle(theGamma, Phot1Direction, Phot1Energy);
          secondaries->push_back(aGamma1);
          secondaryWeights.push_back(1.0);
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon1 accepted by Russian Roul. and made fat, dir " << Phot1Direction << G4endl;
#endif
        } else {
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon1 rejected by Russian Roul., dir " << Phot1Direction << G4endl;
#endif
        }
      }
      // check if gamma2 points towards the field of interest
      G4double Phot2Energy =(1.-epsil)*TotalAvailableEnergy;
      G4double PositP= sqrt(PositKinEnergy*(PositKinEnergy+2.*electron_mass_c2));
      G4ThreeVector dir = PositDirection*PositP - Phot1Direction*Phot1Energy;
      G4ThreeVector Phot2Direction = dir.unit();
      
      if ( AcceptGamma(Phot2Direction,track.GetPosition()) ) {
        G4DynamicParticle* aGamma2 = new G4DynamicParticle(theGamma, Phot2Direction, Phot2Energy);
        secondaries->push_back(aGamma2);
        secondaryWeights.push_back(theWgt);
#ifndef GAMOS_NO_VERBOSE
        if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon2 accepted by direction, dir " << Phot2Direction << G4endl;
#endif
      } else {
        //- Try Russian Roulette
        if ( CLHEP::RandFlat::shoot() < theNSplitInv ) {
          G4DynamicParticle* aGamma2 = new G4DynamicParticle(theGamma, Phot2Direction, Phot2Energy);
          secondaries->push_back(aGamma2);
          secondaryWeights.push_back(1.0);
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon2 accepted by Russian Roul. and made fat, dir " << Phot2Direction << G4endl;
#endif
        } else {
#ifndef GAMOS_NO_VERBOSE
          if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSeeToTwoGammaModel: Photon2 rejected by Russian Roul., dir " << Phot2Direction << G4endl;
#endif
        }
      }
     /*
	G4cout << "Annihilation in fly: e0= " << PositKinEnergy
	<< " m= " << electron_mass_c2
	<< " e1= " << Phot1Energy 
	<< " e2= " << Phot2Energy << " dir= " <<  dir 
	<< " -> " << Phot1Direction << " " 
	<< Phot2Direction << G4endl;
      */
    }
  }

  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
