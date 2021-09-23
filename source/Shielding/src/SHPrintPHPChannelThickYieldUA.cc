#include <math.h>
#include "G4ParticleHPFinalState.hh"

#include "SHPrintPHPChannelThickYieldUA.hh"
#include "SHUtils.hh"
#include "ShieldingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Material.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4ElementVector.hh"
#include "G4HadronicProcess.hh"
#include "G4UserRunAction.hh"

#include "G4ParticleHPThermalBoost.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ParticleHPFission.hh"
#include "G4ParticleHPChannelList.hh"
#include "G4ParticleHPChannel.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPChannelList.hh"
#include "G4ParticleHPChannel.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ParticleHPFission.hh"
#include "G4ParticleHPInelasticBaseFS.hh"
#include "G4ParticleHPInelasticCompFS.hh"

#include "G4EmCalculator.hh"
#include "G4HadronicProcessStore.hh"
#include "G4MaterialCutsCouple.hh"

#include <iomanip>

//------------------------------------------------------------------
SHPrintPHPChannelThickYieldUA::SHPrintPHPChannelThickYieldUA() 
{
  //  theParticleName  = GmParameterMgr::GetInstance()->GetStringValue("SHPrintPHPChannelThickYieldUA:ParticleName","neutron");
  GetEnergyParameters("SHPrintPHPChannelThickYieldUA");
  
}


//------------------------------------------------------------------
void SHPrintPHPChannelThickYieldUA::BeginOfRunAction(const G4Run*)
{
  bPrintXS0 = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintXS0",0));

  SHUtils::BuildElementNames();

  theEmCalculator = new G4EmCalculator;

}

//------------------------------------------------------------------
void SHPrintPHPChannelThickYieldUA::BeginOfEventAction(const G4Event* evt)
{
  G4PrimaryParticle* primParticle  = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4ParticleDefinition* particleDef = primParticle->GetG4code();
  if( particleDef->GetPDGCharge() == 0 ){
    G4Exception("SHPrintPHPChannelThickYieldUA::BeginOfEventAction",
		"Error",
		FatalException,
		("This user actions serves only for charged particles, it is " + particleDef->GetParticleName()).c_str());
  }

  G4ThreeVector direction(1., 0., 0. ); // dummy direction
  
  G4String primaryParticleName = primParticle->GetParticleDefinition()->GetParticleName();
  SHUtils::StoreHadronicProcesses(primaryParticleName);

  //--- Loop to materials 
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  G4ProductionCutsTable* cutsTable = G4ProductionCutsTable::GetProductionCutsTable();
  std::vector<G4MaterialCutsCouple*> coupleTable = cutsTable->coupleTable;
  for( unsigned int imat = 0; imat < matTable->size(); imat++ ){
    G4Material* material = (*matTable)[imat];
     std::vector<G4MaterialCutsCouple*>::const_iterator itect;
    for( itect = coupleTable.begin(); itect != coupleTable.end(); itect++ ) {
      if( (*itect)->GetMaterial() == material ) break;  
    }
    if( itect == coupleTable.end() )  continue; // material created only as component of another material

    std::map<G4double,G4double> totalProductYields; // sum for all channels, for all elements
    std::map<G4String,G4double>* thickYields = new std::map<G4String,G4double>;
    theThickYields[material] = thickYields;

    size_t numberOfElem = material->GetNumberOfElements();
    //    G4double aTemp = material->GetTemperature(); 
    G4ElementVector* elementVector = const_cast<G4ElementVector*>(material->GetElementVector());
    G4double* elementFractionVector = const_cast<G4double*>(material->GetFractionVector());

    const G4double* atomNumDensityVector = material->GetVecNbOfAtomsPerVolume();

    //--- Loop to hadronic processes 
    for( G4int iproc = 0; iproc < SHUtils::GetNHadronicProcesses(); iproc++ ){
      G4HadronicProcess* hadProc = SHUtils::GetHadronicProcess(iproc);
      G4String hname = material->GetName() + "_" + hadProc->GetProcessName();

      if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(primParticle->GetParticleDefinition(), theName);

      //-- Inelastic
      // check that model is a ParticleHP one for smallest up to biggest energy
      G4DynamicParticle* aParticle = new G4DynamicParticle(particleDef, direction, theMinE );
      G4HadProjectile hadProj(*aParticle);
      G4Nucleus targetNucleus = *( hadProc->GetTargetNucleusPointer());
      G4HadronicInteraction* hadModel = hadProc->ChooseHadronicInteraction(hadProj, targetNucleus, material, (*elementVector)[0]); // same model is assumed for all elements !!
      G4DynamicParticle* aParticle2 = new G4DynamicParticle(particleDef, direction, theMaxE );
      G4HadProjectile hadProj2(*aParticle2);
      G4HadronicInteraction* hadModel2 = hadProc->ChooseHadronicInteraction(hadProj2, targetNucleus, material, (*elementVector)[0]); // same model is assumed for all elements !!
      if( hadModel != hadModel2 ) {
	G4Exception("SHPrintPHPChannelThickYieldUA::BeginOfEventAction",
		    "Error",
		    FatalException,
		    ("Model should be the same for all energies " + hadModel->GetModelName() + " <> " + hadModel2->GetModelName()).c_str());
      }

      G4String modelName = hadModel->GetModelName();
      if( modelName == "ParticleHPInelastic" ){
	G4ParticleHPInelastic* HPinelas = (G4ParticleHPInelastic*)(hadModel);

	if( SHUtils::GetNChannelParticles() == 0 ) SHUtils::BuildChannelList(primParticle->GetParticleDefinition(), theName);
	
	//--- loop to elements 
	for (size_t iele=0; iele<numberOfElem; iele++) {
	  G4Element* elem = (*elementVector)[iele];
	  G4double nAtomsPermm3 = atomNumDensityVector[iele];
	  G4int index = elem->GetIndex();
	  G4double elemFraction = elementFractionVector[iele];

	  std::vector<G4ParticleHPChannelList*>* HPinelasvCL = HPinelas->GetInelastic();
	  G4ParticleHPChannel ** HPChannels = (*HPinelasvCL)[index]->GetChannels();
	  
	  G4int nChannels = (*HPinelasvCL)[index]->GetNChannels();
	  G4int numberOfIsos = HPChannels[0]->GetNiso(); // all channels have the same number of isotopes??
	  for( G4int ii = 0; ii < numberOfIsos; ii++ ) {
	    G4double* abunVector = elem->GetRelativeAbundanceVector();
	    G4double isotFraction = abunVector[ii];
         
	    G4String isotName = elem->GetIsotope(ii)->GetName();
	    
	    for(G4int ic=0; ic<nChannels; ic++) {
	      G4ParticleHPChannel* HPChannel = HPChannels[ic];
	      //		G4cout << ii << " LOOPING TO CHANNEL " << HPChannel->theFSType << G4endl;
	      if( HPChannel->HasFSData(ii) ) {// one channel per isotope , one final state per isotope
		G4ParticleHPFinalState * FS = HPChannel->theFinalStates[ii];
		
		//--- Print XS
		G4String FSType = HPChannel->theFSType;
		G4String channelName = SHUtils::GetChannelName(int(FS->theBaseZ), int(FS->theBaseA), FSType.substr(0,3), GmG4Utils::GetParticleShortName(primaryParticleName) );
		
		//--- Print yields of each product
		G4ParticleHPEnAngCorrelation* energyAngData;
		G4ReactionProduct * aTarget = new G4ReactionProduct();
		
		aTarget->SetMass(FS->theBaseA); //t
		
		if( FSType == "F01/" || FSType == "F23/" || FSType == "F24/" || FSType == "F25/" || FSType == "F26/" || FSType == "F27/" ){
		  G4ParticleHPInelasticCompFS* FSC = (G4ParticleHPInelasticCompFS*)(FS);
		  G4int it = FSC->SelectExitChannel(theMinE);
		  energyAngData = FSC->theEnergyAngData[it];
		  if( !energyAngData ) continue;
		  energyAngData->SetTarget(*aTarget);
		  //		  G4cout << FSC << " Use CompFS theEnergyAngData " << it << " " << FSC->theEnergyAngData[it] << G4endl;
		  
		} else {
		  G4ParticleHPInelasticBaseFS* FSB = (G4ParticleHPInelasticBaseFS*)(FS);
		  energyAngData = FSB->theEnergyAngData;
		  energyAngData->SetTarget(*aTarget);
		  if( !energyAngData ) continue;
		  //   G4cout << FSB << " Use BaseFS theEnergyAngData " << FSB->theEnergyAngData << G4endl;
		}
		
		G4int nProducts = energyAngData->nProducts;
		G4ParticleHPProduct * products = energyAngData->theProducts;
		
		for(G4int ip=0; ip<nProducts; ip++) {
		  G4ParticleHPProduct* product = &(products[ip]);
		  
		  //--- Loop to energies
		  G4double thickYield = 0.;
		  for( size_t iener = 0; iener != theNStepsE-1; iener++ ) {
		    G4double ener = theEnergies[iener];
		    if( ener == 0. ) continue;
		    G4double enerUp = theEnergies[iener+1];
		    G4double dEdX = theEmCalculator->GetDEDX(ener, particleDef, material);
		    G4ParticleHPProduct* product2 = &(products[ip]);
		    G4double yield = product2->GetMeanYield(ener);
		    // Micro XS
		    G4double FSXS = HPChannel->GetFSCrossSection(ener, ii);
		    // Macro XS
		    //		    G4double cross = FSXS*(nAtomsPermm3*isotFraction)*barn;
		    G4double cross = FSXS*(nAtomsPermm3)*CLHEP::barn; // FSXS has inito account the isotope fraction

		    thickYield += yield*cross/dEdX*(enerUp-ener);
		    G4String productName = SHUtils::GetProductName( G4int(product2->theMassCode) );
		    //		    PLOT CHANNEL XS
		      G4cout << material->GetName() 
			     << " " << hadProc->GetProcessName() 
			     << " CHANNEL= " << FSType << " " << channelName
			     << " PRODUCT= " << productName 
			     << " ENERGY= " << ener 
			     << " XS_YIELD= " << yield*FSXS
			     << G4endl; 
		    
#ifndef GAMOS_NO_VERBOSE
		      if( SHVerb(debugVerb) && yield != 0 ) 
			G4cout << material->GetName() 
			       << " " << hadProc->GetProcessName() 
			       << " CHANNEL= " << FSType << " " << channelName
			       << " PRODUCT= " << productName 
			       << " THICK_YIELD= " << thickYield
			       << " YIELD= " << yield
			       << " XS micro " << FSXS 
			       << " XS macro " << cross 
			       << " dEdX " << dEdX 
			       << " Ener " << ener 
			       << " cross " << cross
			       << " nAtomsPermm3 " << nAtomsPermm3
			       << " THICK_YIELD/isotFraction= " << thickYield/isotFraction
			       << G4endl;
		  }
#endif
		  //		  thickYield *= nAtomsPermm3 * isotFraction;
		  G4String productName = SHUtils::GetProductName( G4int(product->theMassCode) );
		  (*thickYields)[modelName+"_"+channelName+"_"+productName] = thickYield;
		  if( thickYield != 0 || bPrintXS0 ) {
		    totalProductYields[product->theMassCode] += thickYield;
		    G4cout << "@@SHPrintPHPChannelThickYieldUA " << material->GetName() 
			   << " " << hadProc->GetProcessName() 
			   << " CHANNEL= " << FSType << " " << channelName
			   << " PRODUCT= " << productName 
			   << " YIELD= " << thickYield
			   << " elemF " << elemFraction << " isotF " << isotFraction
			   << G4endl;
		  }
		}
	      } // if HasFSData
	    } // numberOfChannels
	  } // numberOfIsos
	} // numberOfElem
  
      } else if( modelName == "ParticleHPnCapture" ){
	
      }
      
    }

    //--- Print total yield for each product
    std::map<G4double,G4double>::const_iterator itepy;
    for( itepy = totalProductYields.begin(); itepy != totalProductYields.end(); itepy++ ){	
      G4String productName = SHUtils::GetProductName( G4int((*itepy).first) );
      (*thickYields)["ALL_"+productName] = (*itepy).second;
      G4cout << "@@SHPrintPHPChannelThickYieldUA " << material->GetName() 
	     << " PRODUCT= " << productName 
	     << " TOTAL_YIELD= " << (*itepy).second  
	     << G4endl; 
    } 
  }

}


//------------------------------------------------------------------
void SHPrintPHPChannelThickYieldUA::EndOfRunAction(const G4Run*)
{

}
