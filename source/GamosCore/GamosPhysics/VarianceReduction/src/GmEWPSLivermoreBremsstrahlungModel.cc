#include "GmEWPSLivermoreBremsstrahlungModel.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"

#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4SemiLogInterpolation.hh"
//
#include "G4VBremAngularDistribution.hh"
#include "G4ModifiedTsai.hh"
#include "G4Generator2BS.hh"
#include "G4Generator2BN.hh"
//
#include "G4BremsstrahlungCrossSectionHandler.hh"
//
#include "G4VEnergySpectrum.hh"
#include "G4eBremsstrahlungSpectrum.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmEWPSLivermoreBremsstrahlungModel::GmEWPSLivermoreBremsstrahlungModel(const G4ParticleDefinition* part,
						     const G4String& nam)
  :G4LivermoreBremsstrahlungModel(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmEWPSLivermoreBremsstrahlungModel::~GmEWPSLivermoreBremsstrahlungModel()
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void GmEWPSLivermoreBremsstrahlungModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries, 
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicParticle,
		    G4double tcut,
		    G4double ,
		    const G4Track& track)
{
 //PS<
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " GmEWPSLivermoreBremsstrahlungModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif

  G4ThreeVector position = track.GetPosition();
  G4bool bIsApplicable = IsApplicable(position.z());  
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "  GmEWPSLivermoreBremsstrahlungModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    //g494    SampleSecondaries(secondaries, currentCouple, aDynamicParticle, tcut, 100.*TeV );
    SampleSecondaries(secondaries, couple, aDynamicParticle, tcut, 100.*TeV );
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }
  //PS>
  
  //----- Store some variables
  G4double kineticEnergy = aDynamicParticle->GetKineticEnergy();
  
  // this is needed for pathological cases of no ionisation
  if (kineticEnergy <= fIntrinsicLowEnergyLimit)
    {
      fParticleChange->SetProposedKineticEnergy(0.);
      fParticleChange->ProposeLocalEnergyDeposit(kineticEnergy);
      return;
    }
    
  theWgt = track.GetWeight();   //PS<
  theNewNSplit = theNSplit;
  if( theWgt > 0.99 ) {
    theWgt *= theNSplitInv;   
  } else {
    theNewNSplit = 1;
  }
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "  GmEWPSLivermoreBremsstrahlungModel::SampleSecondariesPS wgt " << theWgt << " NSplit " << theNewNSplit << G4endl;
#endif
  //PS>


  //----- Loop 
  G4double totalEnergy = kineticEnergy + electron_mass_c2;
  G4ThreeVector electronDirection = aDynamicParticle->GetMomentumDirection();
  G4double tGamma = 0.;
  G4ThreeVector gammaDirection;
  G4double finalEnergy = 0.; // electron final energy 

  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS
    
    //Sample material
    G4int Z = crossSectionHandler->SelectRandomAtom(couple, kineticEnergy);
    
    //Sample gamma energy
    tGamma = energySpectrum->SampleEnergy(Z, tcut, kineticEnergy, kineticEnergy);
    //nothing happens                                                      
    if (tGamma == 0.) { return; }

    G4double totalEnergy = kineticEnergy + electron_mass_c2;
    finalEnergy = kineticEnergy - tGamma; // electron final energy  

    //Sample gamma direction                                               
    G4ThreeVector gammaDirection =
    GetAngularDistribution()->SampleDirection(aDynamicParticle,
                                              totalEnergy-tGamma,
                                              Z,
                                              couple->GetMaterial());

    G4ThreeVector electronDirection = aDynamicParticle->GetMomentumDirection();

#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmEWPSLivermoreBremsstrahlungModel::SampleSecondariesPS  gammaDir " << gammaDirection << " elecDir " << aDynamicParticle->GetMomentumDirection() << G4endl;
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
    
    /*PS    if (finalEnergy < 0.) 
      {
      // Kinematic problem
      tGamma = kineticEnergy;
      fParticleChange->SetProposedKineticEnergy(0.);
      }
      else 
      {
      //do it only once  //PS
      	G4double momentum = std::sqrt((totalEnergy + electron_mass_c2)*kineticEnergy);
      G4double finalX = momentum*electronDirection.x() - tGamma*gammaDirection.x();
      G4double finalY = momentum*electronDirection.y() - tGamma*gammaDirection.y();
      G4double finalZ = momentum*electronDirection.z() - tGamma*gammaDirection.z();
      G4double norm = 1./std::sqrt(finalX*finalX + finalY*finalY + finalZ*finalZ);
      
      fParticleChange->ProposeMomentumDirection(finalX*norm, finalY*norm, finalZ*norm);
      fParticleChange->SetProposedKineticEnergy(finalEnergy);
      }   PS*/
    
    //Generate the bremsstrahlung gamma
    G4DynamicParticle* aGamma= new G4DynamicParticle (G4Gamma::Gamma(),
						      gammaDirection, tGamma);
    secondaries->push_back(aGamma);
    secondaryWeights.push_back( theNewWgt );
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreBremsstrahlungModel new particle of weight " << theNewWgt << G4endl;
#endif
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSLivermoreBremsstrahlungModel N SECOS = " << secondaries->size() << G4endl;
#endif

  if( secondaries->size() > 0 ) {
    G4double momentum = std::sqrt((totalEnergy + electron_mass_c2)*kineticEnergy);
    G4double finalX = momentum*electronDirection.x() - tGamma*gammaDirection.x();
    G4double finalY = momentum*electronDirection.y() - tGamma*gammaDirection.y();
    G4double finalZ = momentum*electronDirection.z() - tGamma*gammaDirection.z();
    G4double norm = 1./std::sqrt(finalX*finalX + finalY*finalY + finalZ*finalZ);
    
    fParticleChange->ProposeMomentumDirection(finalX*norm, finalY*norm, finalZ*norm);
    fParticleChange->SetProposedKineticEnergy(finalEnergy);
  }

#ifndef GAMOS_NO_VERBOSE
  if (verboseLevel > 1)
    {
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Energy balance from G4LivermoreBremsstrahlung" << G4endl;
      G4cout << "Incoming primary energy: " << kineticEnergy/keV << " keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Outgoing primary energy: " << finalEnergy/keV << " keV" << G4endl;
      G4cout << "Gamma ray " << tGamma/keV << " keV" << G4endl;
      G4cout << "Total final state: " << (finalEnergy+tGamma)/keV << " keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
    }
  if (verboseLevel > 0)
    {
      G4double energyDiff = std::fabs(finalEnergy+tGamma-kineticEnergy);
      if (energyDiff > 0.05*keV)
	G4cout << "G4LivermoreBremsstrahlung WARNING: problem with energy conservation: " 
	       << (finalEnergy+tGamma)/keV << " keV (final) vs. " 
	       << kineticEnergy/keV << " keV (initial)" << G4endl;
    }
#endif
  
  return;
}
