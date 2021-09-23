#include <math.h>

#include "SHGetHadronicChannelXSUA.hh"
#include "SHUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorChangeEnergyAndMaterial.hh"
#include "G4Material.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4StateManager.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4ElementVector.hh"
#include "G4HadronicProcess.hh"
#include "G4UserRunAction.hh"
#include "G4IonTable.hh"

#include <iomanip>

//------------------------------------------------------------------
SHGetHadronicChannelXSUA::SHGetHadronicChannelXSUA() 
{
  bInitialised = FALSE;
}


//------------------------------------------------------------------
void SHGetHadronicChannelXSUA::BeginOfRunAction(const G4Run*)
{
  theCrossSections.clear();

  bFirstEventInRun = TRUE;

  thePrimaryParticle = 0;

  theNEventsInRun = 0;

  if( !bInitialised ) {
    bOnlyIons = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":OnlyIons",0));
    bNotGammas = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NotGammas",0));
    bParticleYields = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ParticleYields",0));
    
    bHistos = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":Histos",1));
    bLogE = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmGeneratorChangeEnergyAndMaterial:logE",0));
    
    if( bHistos ) {
      const G4VUserPrimaryGeneratorAction* generator = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
      GmGeneratorChangeEnergyAndMaterial* generatorCEM = (GmGeneratorChangeEnergyAndMaterial*)(generator);
      
      //--- for use in histogram 
      theMinE = generatorCEM->GetMinE();
      theMaxE = generatorCEM->GetMaxE();
      theNStepsE = generatorCEM->GetNStepsE();
      if( !bLogE ) {
	theMinE -= 0.5 * generatorCEM->GetStepE();
	theMaxE += 0.5 * generatorCEM->GetStepE();
      } else {
	G4double step = (log10(generatorCEM->GetMaxE())-log10(generatorCEM->GetMinE()))/(theNStepsE-1);
	theMinE = log10(generatorCEM->GetMinE()) - 0.5 * step;
	theMaxE = log10(generatorCEM->GetMaxE()) + 0.5 * step;
	//	theMinE = log10(generatorCEM->GetMinE()) - 0.5 * generatorCEM->GetStepE();
	//	theMaxE = log10(generatorCEM->GetMaxE()) + 0.5 * generatorCEM->GetStepE();
	G4cout << bLogE << " MINE " << theMinE << " " << generatorCEM->GetMinE() << " " << generatorCEM->GetStepE() << " " << log10(generatorCEM->GetMinE()) << " step= " << step <<  G4endl; //GDEB
      } 
      G4cout << " MAXE " << theMaxE << " " << generatorCEM->GetMaxE() << " " << generatorCEM->GetStepE() << G4endl; //GDEB
      G4cout << " NSTEPE " << theNStepsE << G4endl; //GDEB
      theMateIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaterialIndexMax",10));
      theChanIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ChannelIndexMax",200));
      theProdIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ProductIndexMax",100));
      theTargIdxMax = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":TargetNucleusIndexMax",50));
      
      SetHistoNameAndNumber(theName,theFilters,theClassifier);
      theAnaMgr->SetDefaultNormalize( false );

    }      

  }
  
}

//------------------------------------------------------------------
void SHGetHadronicChannelXSUA::BeginOfEventAction(const G4Event* evt)
{
  if( !bInitialised ){
    G4PrimaryParticle* primParticle = evt->GetPrimaryVertex(0)->GetPrimary(0);
    SHUtils::StoreHadronicProcesses(primParticle->GetParticleDefinition()->GetParticleName());
    if( bHistos ) {
      const G4VUserPrimaryGeneratorAction* generator = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
      GmGeneratorChangeEnergyAndMaterial* generatorCEM = (GmGeneratorChangeEnergyAndMaterial*)(generator);
      
      // create histograms for TOTAL_XS
      std::vector<G4Material*> mates = generatorCEM->GetMaterials();
      G4cout << "NMATES " <<  mates.size() << " NPROC " << SHUtils::GetNHadronicProcesses() << G4endl;
      for( unsigned int imat = 0; imat < mates.size(); imat++ ) {
	theMaterials[mates[imat]] = imat;
      }
      for( unsigned int imat = 0; imat < mates.size(); imat++ ) {
	for( G4int iproc = 0; iproc < SHUtils::GetNHadronicProcesses(); iproc++ ){
	  G4HadronicProcess* hadProc = SHUtils::GetHadronicProcess(iproc);
	  G4String hname = mates[imat]->GetName() + "_" + hadProc->GetProcessName();
	  //	  G4cout << " PROCNAME " << hadProc->GetProcessName() << G4endl;
	  theAnaMgr->CreateHisto1D((hname+"_XSmicro").c_str(), theNStepsE, theMinE, theMaxE, theHistoNumber+(imat)*100000+iproc*10000+1);
	  theAnaMgr->CreateHisto1D((hname+"_XSmacro").c_str(), theNStepsE, theMinE, theMaxE, theHistoNumber+(imat)*100000+iproc*10000+2);
	}
      }
      
    }

    bInitialised = TRUE;
  }
 
  const G4ParticleDefinition* primary =evt->GetPrimaryVertex(0)->GetPrimary(0)->GetParticleDefinition();
  if( thePrimaryParticle != 0 && primary != thePrimaryParticle ) {
    G4Exception("SHGetHadronicChannelXSUA::BeginOfEventAction",
		"Warning",
		JustWarning,
		G4String("First primary of current event different than previous event : " + primary->GetParticleName() + " <> " + thePrimaryParticle->GetParticleName()).c_str());
  }
  thePrimaryParticle = primary;
  thePrimaryParticleName = thePrimaryParticle->GetParticleName();

  if( bFirstEventInRun ) {

    InitialiseProcessSecoParticles();
    
    G4PrimaryParticle* particle  = evt->GetPrimaryVertex(0)->GetPrimary(0);
    thePrimaryKineticEnergy = GmGenUtils::GetKineticEnergy(particle->GetMass(), particle->GetMomentum().mag() );
    theCurrentMaterial = GmGeometryUtils::GetInstance()->GetTopLV()->GetMaterial();
    theCurrentElementVector = const_cast<G4ElementVector*>(theCurrentMaterial->GetElementVector());
    theCurrentAtomNumDensityVector = const_cast<G4double*>(theCurrentMaterial->GetVecNbOfAtomsPerVolume());
  }

  theNEventsInRun++;
}

//------------------------------------------------------------------
void SHGetHadronicChannelXSUA::InitialiseProcessSecoParticles()
{
  theProcessSecoParticles.clear();
  //-- Get process manager of particle
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  G4ProcessManager* pmanager = 0;
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    //    G4cout << ":SHGetHadronicChannelXSUA  PARTICLE " << particle->GetParticleName() << " lepton " << particle->GetLeptonNumber() << G4endl;
    if( particle->GetParticleName() == thePrimaryParticleName ){
      //      G4cout << "SHGetHadronicChannelXSUA:  PARTICLE is " << particle->GetParticleName() << G4endl;
      pmanager = particle->GetProcessManager();

      G4ProcessVector* pvect = pmanager->GetProcessList();
      int jj, sizproc = pvect->size();
      for( jj = 0; jj < sizproc; jj++ ) {
	G4VProcess* proc = (*pvect)[jj];
	if( proc->GetProcessType() != fHadronic ) continue;
	G4HadronicProcess* prochad = dynamic_cast<G4HadronicProcess*>(proc);
	if( prochad ) {
	  //	  G4cout << "SHGetHadronicChannelXSUA::InitialiseProcessSecoParticles proc= " << prochad->GetProcessName() << G4endl;
	  std::map< std::map<G4String,G4int>, G4int > pseco;
	  theProcessSecoParticles[prochad] = pseco;

	}
      }
    }
    
  }
  
}


//------------------------------------------------------------------
void SHGetHadronicChannelXSUA::UserSteppingAction(const G4Step* aStep)
{ 
  const G4VProcess* proc  = aStep->GetPostStepPoint()->GetProcessDefinedStep();

  //----- Only for selected particle
  const G4Track* aTrack = aStep->GetTrack();
  G4Track* aTracknc = const_cast<G4Track*>(aTrack);  
  aTracknc->SetTrackStatus( fStopAndKill);
  //  G4cout << " SHGetHadronicChannelXSUA::UserSteppingAction " << aTrack->GetDefinition()->GetParticleName() << " !=? " << theParticleName << G4endl;
 
  if( proc->GetProcessType() != fHadronic ) return;

  if( aTrack->GetDefinition()->GetParticleName() != thePrimaryParticleName ) return;
    
   //----- Loop to processes
  if( bFirstEventInRun ) { // only once per world
    bFirstEventInRun = FALSE;

    for( G4int iproc = 0; iproc < SHUtils::GetNHadronicProcesses(); iproc++ ){
      G4HadronicProcess* hadProc = SHUtils::GetHadronicProcess(iproc);

      //--- Get Mean free path
      aTracknc->SetKineticEnergy( aTracknc->GetVertexKineticEnergy() );
      //      G4ForceCondition* cond;
      //      G4double mfp = hadProc->GetMeanFreePath(*aTracknc,0.,cond );
      
      //--- Get microscopic cross section
      const G4DynamicParticle* aParticle = aTracknc->GetDynamicParticle();
      G4double cross = 0.;
      size_t nelm = theCurrentMaterial->GetNumberOfElements();
      //      G4double aTemp = theCurrentMaterial->GetTemperature();
      G4double nAtomsPermm3 = 0;
      for (size_t iele=0; iele<nelm; iele++) {
	const G4Element* elm = (*theCurrentElementVector)[iele];
	cross += theCurrentAtomNumDensityVector[iele]*  // atoms / mm3
	  //  hadProc->GetInelasticCrossSectionPerAtom(aParticle,ener,elm);
 	  hadProc->GetMicroscopicCrossSection(aParticle,elm,theCurrentMaterial);
	//	G4cout << " theCurrentAtomNumDensityVector " <<theCurrentAtomNumDensityVector[iele] << " GetMicroscopicCrossSection " << hadProc->GetMicroscopicCrossSection(aParticle,elm,aTemp) << G4endl; 
	nAtomsPermm3 += theCurrentAtomNumDensityVector[iele];
      }
      theCrossSections[hadProc] = cross/nAtomsPermm3/CLHEP::barn;
      G4cout << "@@SHGetHadronicChannelXSUA " << theCurrentMaterial->GetName() << " " 
	     << thePrimaryKineticEnergy << " MeV " 
	     << hadProc->GetProcessName()
	     << " TOTAL_XS(macro)= " << cross << " mm-1" 
	     << " TOTAL_XS(micro)= " << cross/nAtomsPermm3/CLHEP::barn << " barn" 
	     << G4endl;

      if( bHistos ) {
	G4int materialID = theMaterials[theCurrentMaterial];
	GmHisto1* hisXSmicro = theAnaMgr->GetHisto1( theHistoNumber+materialID*100000+iproc*10000+1);
	G4double enerH = thePrimaryKineticEnergy;
	if( bLogE ) enerH = log10(enerH);
	hisXSmicro->Fill(enerH,cross);
	//	G4cout << hisXSmicro->GetName() << " FILL XSmicro " << enerH << "= " <<cross << G4endl; //GDEB
	GmHisto1* hisXSmacro = theAnaMgr->GetHisto1( theHistoNumber+materialID*100000+iproc*10000+2);
	hisXSmacro->Fill(enerH,cross/nAtomsPermm3/CLHEP::barn);
      }	
    }
  }
  
  //---- Get secondary particles 
  const G4TrackVector* secoList = fpSteppingManager->GetSecondary();  //only one step is allowed
  G4TrackVector::const_iterator ite;
  std::map<G4String,G4int> secoParticles;
  // Add target nucleus as first particle in list of secondary particles
  const G4HadronicProcess* hadProc = (const G4HadronicProcess*)(proc);
  const G4Nucleus* aNucleus = hadProc->GetTargetNucleus();
  //  G4String nucName = "[" + GmG4Utils::GetElementNameFromZ(aNucleus->GetZ_asInt() ) + GmG4Utils::itoa(aNucleus->GetA_asInt() ) + " ";  // add [ for identification as ion
  G4String targetNucName = GmG4Utils::GetElementNameFromZ(aNucleus->GetZ_asInt() ) + GmGenUtils::itoa(aNucleus->GetA_asInt() );
  theNEvents[targetNucName] ++;
  targetNucName = "[" + targetNucName;  // add [ for identification as ion
  /*  if( aNucleus ) {
      G4cout << aNucleus << " NUCLEUS Z " << aNucleus->GetZ_asInt() 
      << " A Z " << aNucleus->GetA_asInt() << G4endl;
      }*/
  secoParticles[targetNucName] = 1;
  
  for( ite = secoList->begin(); ite != secoList->end(); ite++ ){
    //    G4cout << " SECOLIST " << (*ite)->GetDefinition()->GetParticleName() << G4endl;
    //--- build map of particles
    G4ParticleDefinition* particle = (*ite)->GetDefinition();
    if( bOnlyIons && particle != G4Gamma::Gamma() && particle->GetAtomicNumber() <= 4 ) continue;
    if( bNotGammas && particle == G4Gamma::Gamma() ) continue;
    secoParticles[particle->GetParticleName()] ++;
  }
  
  mhmmsi::iterator itemh = theProcessSecoParticles.find( hadProc );
  
  if( itemh == theProcessSecoParticles.end() ) {
    G4cerr << " PROCESS " << hadProc->GetProcessName();
    G4Exception("SHGetHadronicChannelXSUA::UserSteppingAction",
		"Error",
		FatalException,
		"BUG: process not defined in theProcessSecoParticles");
  }
  
  /*  std::map<G4String,G4int>::const_iterator itesp;
  G4cout << "SECO";
  for( itesp = secoParticles.begin(); itesp != secoParticles.end(); itesp++ ) {
    G4cout << " " << (*itesp).first << " " << (*itesp).second;
  }
  G4cout << G4endl;
  */
  
  //    G4cout << " add interaction " << ((*itemh).first)->GetProcessName() << " " << ((*itemh).second).size() <<  G4endl;
  ((*itemh).second)[ secoParticles ] ++;
  
}
  
//------------------------------------------------------------------
void SHGetHadronicChannelXSUA::EndOfRunAction(const G4Run*)
{
  mhmmsi::iterator itemh;
  mmsi::iterator itemp;
  std::map<G4String,G4int>::reverse_iterator itepi;
  std::map<const G4HadronicProcess*, G4int > countPerProcess;
  std::map< G4String, std::map<G4String,int>* > particleYields;  // nucleus - (particle - N) 

  //----- Loop to processes
  for( itemh = theProcessSecoParticles.begin(); itemh != theProcessSecoParticles.end(); itemh++ ) {
    G4int nEventsProc = 0;
    std::map<const G4HadronicProcess*,G4double>::const_iterator iteXS = theCrossSections.find( (*itemh).first );
    std::map<G4String, std::map<G4String, G4int>* >::const_iterator itepys;
    for( itepys = particleYields.begin(); itepys != particleYields.end(); itepys++ ){
      delete (*itepys).second;
    }
    particleYields.clear();

    //--- Get channel map
    mmsi secoChannelCount = (*itemh).second; // map of channel - N, where channel is list of particle-N
    // add channel by ion type 
    std::map<G4String, G4int > countPerChannelAll; // add all channels that are equal without taking into the gammas
    for( itemp = secoChannelCount.begin(); itemp != secoChannelCount.end(); itemp++ ){
      std::map<G4String,G4int> channelParticleList = (*itemp).first;
      G4cout << "@@SHGetHadronicChannelXSUA " << theCurrentMaterial->GetName() << " " 
	     << thePrimaryKineticEnergy << " MeV " 
	     << ((*itemh).first)->GetProcessName() << " CHANNEL ";
      //build channel name 
      G4String channelName = "";
      std::vector<G4String> secoParticles;
      G4String targetNucName = "";
      for(itepi = channelParticleList.rbegin(); itepi != channelParticleList.rend(); itepi++) {	
	G4String partName = (*itepi).first;
	G4cout << partName << " " << (*itepi).second << " ";
	if( partName.find("[") == 0 ) targetNucName = partName;
	if( (*itepi).first != "gamma" ) {
	  for( G4int ip = 0; ip < (*itepi).second; ip++ ) {
	    //	    if( channelName != "" ) channelName += "_";
	    //	    channelName += ((*itepi).first)->GetParticleName();
	    secoParticles.push_back( partName );
	    //	    G4cout << " secoParticles.insert " <<  (*itepi).first->GetParticleName() << " " << secoParticles.size() << G4endl;
	  } 
	}
      }
      
      if( !thePrimaryParticle ) return; // this run has 0 events (run/beamOn = N* nEventsPerRun)
      channelName = GmG4Utils::GetInelasticName( secoParticles, thePrimaryParticle );

      countPerChannelAll[channelName] += (*itemp).second;
      
      countPerProcess[((*itemh).first)] += (*itemp).second;
      G4cout << " = " << (*itemp).second << G4endl;
      nEventsProc += (*itemp).second;
      
      std::map<G4String, std::map<G4String,int>* >::iterator itepy = particleYields.find(targetNucName);
      std::map<G4String,int>* partYield;
      if( itepy == particleYields.end() ) {
	partYield = new std::map<G4String,int>();
	//	G4cout << partYield << " new " << targetNucName << G4endl;
	particleYields[targetNucName] = partYield;
      } else {
	partYield = (*itepy).second;
      }
      for(itepi = channelParticleList.rbegin(); itepi != channelParticleList.rend(); itepi++) {	
	G4String partName = (*itepi).first;
	if( partName !=	targetNucName ) {
	  (*partYield)[partName] += (*itepi).second * (*itemp).second;
	  //	  G4cout << targetNucName << " partYield " << partName << " " << (*itepi).second << " " << (*partYield)[partName] << G4endl;
	}
      }
      
    }
    
    std::map<G4String, G4int >::const_iterator iteci; //loop to channels found
    for( iteci = countPerChannelAll.begin(); iteci != countPerChannelAll.end(); iteci++ ){
      std::string channelName = (*iteci).first;
      size_t it = channelName.find("(");
      std::string targetNucName = channelName.substr(0,it);
      G4cout << "@@SHGetHadronicChannelXSUA " << theCurrentMaterial->GetName() 
	     << " " << thePrimaryKineticEnergy << " MeV"
	     << " " << ((*itemh).first)->GetProcessName() 
	     << " CHANNEL_ALL= " << channelName
	//      << " = " << (*iteci).second << "/" << nEventsProc 
	//      << "*XS(micro)= " <<  (*iteci).second*(*iteXS).second/nEventsProc << G4endl;
	//	     << " = " << (*iteci).second << "/" << theNEvents[targetNucName]
	//	     << "*XS(micro)= " <<  (*iteci).second*(*iteXS).second/ theNEvents[targetNucName] << G4endl;
	     << " = " << (*iteci).second << "/" << theNEventsInRun
	     << " = " << G4double((*iteci).second)/theNEventsInRun
	     << "*XS(micro)= " << (*iteci).second*(*iteXS).second/ theNEventsInRun << " barn " << G4endl;
      if( bHistos ) {
	G4int ichan; 
	std::map<G4String,G4int>::const_iterator itec = theChannels.find(channelName);
	if( itec == theChannels.end() ) {
	  ichan = theChannels.size();
	  theChannels[channelName] = ichan;
	} else {
	  ichan = (*itec).second;
	}
	G4int iproc = SHUtils::GetProcessID( (*itemh).first );
	std::map<G4Material*,G4int>::const_iterator item = theMaterials.find(theCurrentMaterial);
	G4int imat = (*item).second;

	G4int histoNChann = theHistoNumber 
	  + imat*theMateIdxMax*theChanIdxMax*10
	  + ichan*theChanIdxMax*10
	  + iproc*10;
	G4cout << histoNChann << " " << theHistoNumber << " LOOPING " << ((*itemh).first)->GetProcessName() << " : " << iproc << "  " << channelName << " : " << ichan << "  " << theCurrentMaterial->GetName() << " : " << imat << G4endl;
#ifndef GAMOS_NO_ROOT
	TH1F* hisNP = theAnaMgr->GetHisto1(histoNChann+3,0);
	TH1F* hisXSmic = 0;
#else
	GmHisto1* hisNP = theAnaMgr->GetHisto1(histoNChann+3,0);
	GmHisto1* hisXSmic = 0;
#endif
	if( !hisNP ) {
	  G4String hname =  ((*itemh).first)->GetProcessName() + "_" + theCurrentMaterial->GetName() + "_" + channelName;
	  theAnaMgr->CreateHisto1D((hname+"_NPART").c_str(), theNStepsE, theMinE, theMaxE, histoNChann+3 );
	  theAnaMgr->CreateHisto1D((hname+"_XSmicro").c_str(), theNStepsE, theMinE, theMaxE, histoNChann+4 );
	  hisNP = theAnaMgr->GetHisto1(histoNChann+3,0);
	}
	hisXSmic = theAnaMgr->GetHisto1(histoNChann+4);
	G4double enerH = thePrimaryKineticEnergy;
	if( bLogE ) enerH = log10(enerH);
	hisNP->Fill( enerH, G4double((*iteci).second)/theNEventsInRun);
	//	G4cout << hisXSmic->GetName() << " FILL XSmich " << enerH << "= " << (*iteci).second*(*iteXS).second/ theNEventsInRun << " " << (*iteci).second*(*iteXS).second << G4endl; //GDEB
	hisXSmic->Fill( enerH, (*iteci).second*(*iteXS).second/ theNEventsInRun );
      }

    }

    if( bParticleYields ) {
       std::map<G4String,G4int> totalProductYields; // sum for all target nuclei

      for( itepys = particleYields.begin(); itepys != particleYields.end(); itepys++ ){
	G4String targetNucName = (*itepys).first;
	targetNucName = targetNucName.substr(1,targetNucName.length());
	std::map<G4String,int>* partYield = (*itepys).second;	
	std::map<G4String,int>::const_iterator itepy1;
	for( itepy1 = partYield->begin(); itepy1 != partYield->end(); itepy1++ ) {
	  G4String productName = (*itepy1).first;
	  G4cout << "@@SHGetHadronicChannelXSUA " << theCurrentMaterial->GetName() 
		 << " " << thePrimaryKineticEnergy << " MeV"
		 << " " << ((*itemh).first)->GetProcessName()  
		 << " " << targetNucName 
		 << " PARTICLE_YIELD " << productName
		 << " = " << (*itepy1).second << "/" << theNEventsInRun
 		 << " = " << G4double((*itepy1).second)/theNEventsInRun 
		 << " *XS(micro)= " << (*itepy1).second*(*iteXS).second/ theNEventsInRun 
		 << G4endl;

	  totalProductYields[productName] += (*itepy1).second;

	  if( bHistos ) {
	    G4int itarg; 
	    std::map<G4String,G4int>::const_iterator itec = theTargetNuclei.find(targetNucName);
	    if( itec == theTargetNuclei.end() ) {
	      itarg = theTargetNuclei.size();
	      theTargetNuclei[targetNucName] = itarg;
	    } else {
	      itarg = (*itec).second;
	    }
	    G4int iproc = SHUtils::GetProcessID( (*itemh).first );
	    std::map<G4Material*,G4int>::const_iterator item = theMaterials.find(theCurrentMaterial);
	    G4int imat = (*item).second;
	    G4int iprod; 
	    itec = theProducts.find(productName);
	    if( itec == theProducts.end() ) {
	      iprod = theProducts.size();
	      theProducts[productName] = iprod;
	    } else {
	      iprod = (*itec).second;
	    }

	    G4int histoNYield = theHistoNumber 
	      + imat*theMateIdxMax*theChanIdxMax*theProdIdxMax*10
	      + (itarg+1)*theTargIdxMax*theProdIdxMax*10
	      + iprod*theProdIdxMax*10
	      + iproc*10;
#ifndef GAMOS_NO_ROOT
	    TH1F* hisY = theAnaMgr->GetHisto1(histoNYield+5,0);
	    TH1F* hisXSmic = 0;
#else
	    GmHisto1* hisY = theAnaMgr->GetHisto1(histoNYield+5,0);
	    GmHisto1* hisXSmic = 0;
#endif
	    if( !hisY ) {
	      G4String hname =  ((*itemh).first)->GetProcessName() + "_" + theCurrentMaterial->GetName() + "_" + targetNucName + "_" + productName;
	      theAnaMgr->CreateHisto1D((hname+"_YIELD").c_str(), theNStepsE, theMinE, theMaxE, histoNYield+5 );
	      theAnaMgr->CreateHisto1D((hname+"_YIELD_XSmicro").c_str(), theNStepsE, theMinE, theMaxE, histoNYield+6 );
	      hisY = theAnaMgr->GetHisto1(histoNYield+5,0);
	    }
	    hisXSmic = theAnaMgr->GetHisto1(histoNYield+6);
	    G4double enerH = thePrimaryKineticEnergy;
	    if( bLogE ) enerH = log10(enerH);
 	    hisY->Fill( enerH, G4double((*itepy1).second)/theNEventsInRun);
	    hisXSmic->Fill( enerH, (*itepy1).second*(*iteXS).second/ theNEventsInRun );
	  }

	}
      }

      std::map<G4String,int>::const_iterator itepy1;
      for( itepy1 = totalProductYields.begin(); itepy1 != totalProductYields.end(); itepy1++ ) {
	G4String productName = (*itepy1).first;
	G4cout << "@@SHGetHadronicChannelXSUA " << theCurrentMaterial->GetName() 
	       << " " << thePrimaryKineticEnergy << " MeV"
	       << " " << ((*itemh).first)->GetProcessName()  
	       << " TOTAL_PARTICLE_YIELD " << (*itepy1).first 
	       << " = " << (*itepy1).second << "/" << theNEventsInRun
	       << " = " << G4double((*itepy1).second)/theNEventsInRun 
	       << " *XS(micro)= " << (*itepy1).second*(*iteXS).second/ theNEventsInRun 
	       << G4endl;
	
	if( bHistos ) {
	  G4int iproc = SHUtils::GetProcessID( (*itemh).first );
	  std::map<G4Material*,G4int>::const_iterator item = theMaterials.find(theCurrentMaterial);
	  G4int imat = (*item).second;
	    std::map<G4String,G4int>::const_iterator itec = theProducts.find(productName);
	  G4int iprod = (*itec).second;
	  
	  G4int histoNYield = theHistoNumber 
	    + imat*theMateIdxMax*theChanIdxMax*theProdIdxMax*10
	    + iprod*theProdIdxMax*10
	    + iproc*10;
#ifndef GAMOS_NO_ROOT
	  TH1F* hisY = theAnaMgr->GetHisto1(histoNYield+5,0);
	  TH1F* hisXSmic = 0;
#else
	  GmHisto1* hisY = theAnaMgr->GetHisto1(histoNYield+5,0);
	  GmHisto1* hisXSmic = 0;
#endif
	  if( !hisY ) {
	    G4String hname =  ((*itemh).first)->GetProcessName() + "_" + theCurrentMaterial->GetName() + "_" + productName;
	    theAnaMgr->CreateHisto1D((hname+"_TOTAL_YIELD").c_str(), theNStepsE, theMinE, theMaxE, histoNYield+5 );
	    theAnaMgr->CreateHisto1D((hname+"_TOTAL_YIELD_XSmicro").c_str(), theNStepsE, theMinE, theMaxE, histoNYield+6 );
	    hisY = theAnaMgr->GetHisto1(histoNYield+5,0);
	  }
	  hisXSmic = theAnaMgr->GetHisto1(histoNYield+6);
	  G4double enerH = thePrimaryKineticEnergy;
	  if( bLogE ) enerH = log10(enerH);
	  G4cout << hisXSmic->GetName() << " FILL TYXSmich " << enerH << "= " <<  (*itepy1).second*(*iteXS).second/ theNEventsInRun << " " <<  (*itepy1).second << " * " <<  (*iteXS).second << " /" << theNEventsInRun << G4endl; //GDEB
	  hisY->Fill( enerH, G4double((*itepy1).second)/theNEventsInRun);
	  hisXSmic->Fill( enerH, (*itepy1).second*(*iteXS).second/ theNEventsInRun );
	}            
	
      }

    }

  }
  
  /*  std::map<const G4HadronicProcess*, G4int >::const_iterator itecp;
  for( itecp = countPerProcess.begin(); itecp != countPerProcess.end(); itecp++ ){
    std::map<const G4HadronicProcess*,G4double>::const_iterator iteXS = theCrossSections.find( (*itecp).first );
    G4cout << "@@SHGetHadronicChannelXSUA " << theCurrentMaterial->GetName() << " " << thePrimaryKineticEnergy << " PROCESS ALL " << ((*itecp).first)->GetProcessName() << " = " << (*itecp).second << " XS= " << (*iteXS).second << G4endl;
  } */

  theNEvents.clear();
}


