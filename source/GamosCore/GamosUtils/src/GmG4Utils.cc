#include "GmG4Utils.hh"
#include "GmGenUtils.hh"

#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4VProcess.hh"
#include "G4ProcessVector.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4EventManager.hh"
#include "G4NavigationHistory.hh"
#include "G4AffineTransform.hh"
#include "G4tgrFileIn.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "G4GenericIon.hh"
#include "G4NistManager.hh"
#include "G4RadioactiveDecay.hh"
#include "G4Nucleus.hh"
#include "G4HadronicProcess.hh"
#include "G4DecayTable.hh"
#include "G4VTouchable.hh"
#include "G4GammaGeneralProcess.hh"
#include "G4GammaConversionToMuons.hh"

std::map<const G4ParticleDefinition*,G4DecayTable*> GmG4Utils::thePartDecayTable;

/*//------------------------------------------------------------------
G4VProcess* GetProcess(const G4String& particleName, const G4String& processName )
{
  G4VProcess* proc = 0;

  proc = G4ProcessTable::GetProcessTable()->FindProcess( processName, particleName);

  if( !proc ) {
    G4Exception("GmG4Utils::GetProcess","ERROR",FatalErrorInArgument,("process name not found " + processName + " for particle " + particleName ).c_str() );
  } 

  return proc;
  }*/

//------------------------------------------------------------------
  std::vector<G4Material*> GmG4Utils::GetG4MaterialList( const G4String& materialName, G4bool bMustExist )
{
  std::vector<G4Material*> theMaterials;

  const G4MaterialTable* mateTable = G4Material::GetMaterialTable();
  G4MaterialTable::const_iterator itemate;

  for( itemate = mateTable->begin(); itemate != mateTable->end(); itemate++ ) {
    G4Material* material = *itemate;
    if( GmGenUtils::AreWordsEquivalent(materialName,material->GetName()) ){
      theMaterials.push_back(material);
    }
  }

  if( theMaterials.size() == 0 && bMustExist ) {
    G4Exception("GmG4Utils::GetG4MaterialList",
		"ERROR",
		FatalErrorInArgument,
		("material name not found " + materialName ).c_str() );
  } 

  return theMaterials;

}


//------------------------------------------------------------------
// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Eta.hh"
#include "G4EtaPrime.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZero.hh"
#include "G4AntiKaonZero.hh"
#include "G4KaonZeroLong.hh"
#include "G4KaonZeroShort.hh"

#include "G4DMesonPlus.hh"
#include "G4DMesonMinus.hh"
#include "G4DMesonZero.hh"
#include "G4AntiDMesonZero.hh"
#include "G4DsMesonPlus.hh"
#include "G4DsMesonMinus.hh"
#include "G4JPsi.hh"

#include "G4BMesonPlus.hh"
#include "G4BMesonMinus.hh"
#include "G4BMesonZero.hh"
#include "G4AntiBMesonZero.hh"
#include "G4BsMesonZero.hh"
#include "G4AntiBsMesonZero.hh"

//------------------------------------------------------------------
G4bool GmG4Utils::IsLightMeson( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if (partDef == G4PionPlus::PionPlusDefinition() || 
      partDef == G4PionMinus::PionMinusDefinition() || 
      partDef == G4PionZero::PionZeroDefinition() || 
      partDef == G4Eta::EtaDefinition() || 
      partDef == G4EtaPrime::EtaPrimeDefinition() || 
      partDef == G4KaonPlus::KaonPlusDefinition() || 
      partDef == G4KaonMinus::KaonMinusDefinition() || 
      partDef == G4KaonZero::KaonZeroDefinition() || 
      partDef == G4AntiKaonZero::AntiKaonZeroDefinition() || 
      partDef == G4KaonZeroLong::KaonZeroLongDefinition() || 
      partDef == G4KaonZeroShort::KaonZeroShortDefinition() ||
      partDef->GetParticleName().substr(0,3) == G4String("a0(") ||
      partDef->GetParticleName().substr(0,3) == G4String("a1(") ||
      partDef->GetParticleName().substr(0,3) == G4String("a2(") ||
      partDef->GetParticleName().substr(0,2) == G4String("k(") ||
      partDef->GetParticleName().substr(0,3) == G4String("k1(") ||
      partDef->GetParticleName().substr(0,3) == G4String("k2(") ||
      partDef->GetParticleName().substr(0,7) == G4String("k_star(") ||
      partDef->GetParticleName().substr(0,8) == G4String("k0_star(") ||
      partDef->GetParticleName().substr(0,8) == G4String("k2_star(") ||
      partDef->GetParticleName().substr(0,8) == G4String("k3_star(") ||
      partDef->GetParticleName().substr(0,7) == G4String("anti_k(") ||
      partDef->GetParticleName().substr(0,8) == G4String("anti_k0(") ||
      partDef->GetParticleName().substr(0,8) == G4String("anti_k1(") ||
      partDef->GetParticleName().substr(0,8) == G4String("anti_k2(") ||
      partDef->GetParticleName().substr(0,12) == G4String("anti_k_star(") ||
      partDef->GetParticleName().substr(0,13) == G4String("anti_k2_star(") ||
      partDef->GetParticleName().substr(0,13) == G4String("anti_k3_star(") ||
      partDef->GetParticleName().substr(0,3) == G4String("b1(") ||
      partDef->GetParticleName().substr(0,3) == G4String("f0(") ||
      partDef->GetParticleName().substr(0,3) == G4String("f1(") ||
      partDef->GetParticleName().substr(0,3) == G4String("f2(") ||
      partDef->GetParticleName().substr(0,9) == G4String("f2_prime(") ||
      partDef->GetParticleName().substr(0,3) == G4String("h1(") ||
      partDef->GetParticleName().substr(0,4) == G4String("eta(") ||
      partDef->GetParticleName().substr(0,5) == G4String("eta2(")  ||
      partDef->GetParticleName().substr(0,4) == G4String("phi(")  ||
      partDef->GetParticleName().substr(0,5) == G4String("phi3(")  ||
      partDef->GetParticleName().substr(0,3) == G4String("pi(")  ||
      partDef->GetParticleName().substr(0,4) == G4String("pi2(")  ||
      partDef->GetParticleName().substr(0,4) == G4String("rho(")  ||
      partDef->GetParticleName().substr(0,5) == G4String("rho3(") ){
    isOK = TRUE;
  }
  return isOK;
}

//------------------------------------------------------------------
G4bool GmG4Utils::IsCharmMeson( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if( partDef == G4DMesonPlus::DMesonPlusDefinition() || 
      partDef == G4DMesonMinus::DMesonMinusDefinition() || 
      partDef == G4DMesonZero::DMesonZeroDefinition() || 
      partDef == G4AntiDMesonZero::AntiDMesonZeroDefinition() || 
      partDef == G4DsMesonPlus::DsMesonPlusDefinition() || 
      partDef == G4DsMesonMinus::DsMesonMinusDefinition() || 
      partDef == G4JPsi::JPsiDefinition()){
    isOK = TRUE;
  }
  return isOK; 
}

//------------------------------------------------------------------
G4bool GmG4Utils::IsBottomMeson( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if( partDef == G4BMesonPlus::BMesonPlusDefinition() || 
      partDef == G4BMesonMinus::BMesonMinusDefinition() || 
      partDef == G4BMesonZero::BMesonZeroDefinition() || 
      partDef == G4AntiBMesonZero::AntiBMesonZeroDefinition() || 
      partDef == G4BsMesonZero::BsMesonZeroDefinition() || 
      partDef == G4AntiBsMesonZero::AntiBsMesonZeroDefinition()){
    isOK = TRUE;
  }
  return isOK;
}

//------------------------------------------------------------------
// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

//------------------------------------------------------------------
G4bool GmG4Utils::IsLightBaryon( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if( partDef == G4Proton::ProtonDefinition() || 
      partDef == G4AntiProton::AntiProtonDefinition() || 
      partDef == G4Neutron::NeutronDefinition() || 
      partDef == G4AntiNeutron::AntiNeutronDefinition() || 
      partDef->GetParticleName().substr(0,2) == G4String("N(") ||
      partDef->GetParticleName().substr(0,7) == G4String("anti_N(") ||
      partDef->GetParticleName().substr(0,6) == G4String("delta(") ||
      partDef->GetParticleName().substr(0,11) == G4String("anti_delta(") ){
      isOK = TRUE;
  }
  return isOK;
}

//------------------------------------------------------------------
#include "G4Lambda.hh"
#include "G4SigmaPlus.hh"
#include "G4SigmaZero.hh"
#include "G4SigmaMinus.hh"
#include "G4XiMinus.hh"
#include "G4XiZero.hh"
#include "G4OmegaMinus.hh"

#include "G4AntiLambda.hh"
#include "G4AntiSigmaPlus.hh"
#include "G4AntiSigmaZero.hh"
#include "G4AntiSigmaMinus.hh"
#include "G4AntiXiMinus.hh"
#include "G4AntiXiZero.hh"
#include "G4AntiOmegaMinus.hh"

#include "G4LambdacPlus.hh"
#include "G4SigmacPlusPlus.hh"
#include "G4SigmacPlus.hh"
#include "G4SigmacZero.hh"
#include "G4XicPlus.hh"
#include "G4XicZero.hh"
#include "G4OmegacZero.hh"

#include "G4AntiLambdacPlus.hh"
#include "G4AntiSigmacPlusPlus.hh"
#include "G4AntiSigmacPlus.hh"
#include "G4AntiSigmacZero.hh"
#include "G4AntiXicPlus.hh"
#include "G4AntiXicZero.hh"
#include "G4AntiOmegacZero.hh"

//------------------------------------------------------------------
G4bool GmG4Utils::IsStrangeBaryon( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if( partDef == G4Lambda::LambdaDefinition() || 
      partDef == G4AntiLambda::AntiLambdaDefinition() || 
      partDef == G4SigmaZero::SigmaZeroDefinition() || 
      partDef == G4AntiSigmaZero::AntiSigmaZeroDefinition() || 
      partDef == G4SigmaPlus::SigmaPlusDefinition() || 
      partDef == G4AntiSigmaPlus::AntiSigmaPlusDefinition() || 
      partDef == G4SigmaMinus::SigmaMinusDefinition() || 
      partDef == G4AntiSigmaMinus::AntiSigmaMinusDefinition() || 
      partDef == G4XiZero::XiZeroDefinition() || 
      partDef == G4AntiXiZero::AntiXiZeroDefinition() || 
      partDef == G4XiMinus::XiMinusDefinition() || 
      partDef == G4AntiXiMinus::AntiXiMinusDefinition() || 
      partDef == G4OmegaMinus::OmegaMinusDefinition() || 
      partDef == G4AntiOmegaMinus::AntiOmegaMinusDefinition() ||
      partDef->GetParticleName().substr(0,7) == G4String("lambda(") ||
      partDef->GetParticleName().substr(0,12) == G4String("anti_lambda(") ||
      partDef->GetParticleName().substr(0,6) == G4String("sigma(") ||
      partDef->GetParticleName().substr(0,11) == G4String("anti_sigma(") || 
      partDef->GetParticleName().substr(0,3) == G4String("xi(") ||
      partDef->GetParticleName().substr(0,8) == G4String("anti_xi(") ||
      partDef->GetParticleName().substr(0,6) == G4String("omega(") ||
      partDef->GetParticleName().substr(0,7) == G4String("omega3(")) {
    isOK = TRUE;
  }
  return isOK;
}

//------------------------------------------------------------------
G4bool GmG4Utils::IsCharmBaryon( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if( partDef == G4LambdacPlus::LambdacPlusDefinition() || 
      partDef == G4SigmacPlusPlus::SigmacPlusPlusDefinition() || 
      partDef == G4SigmacPlus::SigmacPlusDefinition() || 
      partDef == G4SigmacZero::SigmacZeroDefinition() || 
      partDef == G4XicPlus::XicPlusDefinition() || 
      partDef == G4XicZero::XicZeroDefinition() || 
      partDef == G4OmegacZero::OmegacZeroDefinition() || 
      partDef == G4AntiLambdacPlus::AntiLambdacPlusDefinition() || 
      partDef == G4AntiSigmacPlusPlus::AntiSigmacPlusPlusDefinition() || 
      partDef == G4AntiSigmacPlus::AntiSigmacPlusDefinition() || 
      partDef == G4AntiSigmacZero::AntiSigmacZeroDefinition() || 
      partDef == G4AntiXicPlus::AntiXicPlusDefinition() || 
      partDef == G4AntiXicZero::AntiXicZeroDefinition() || 
      partDef == G4AntiOmegacZero::AntiOmegacZeroDefinition()){
    isOK = TRUE;
  }
  return isOK;
}

//------------------------------------------------------------------
#include "G4Alpha.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4GenericIon.hh"

//------------------------------------------------------------------
G4bool GmG4Utils::IsIon( G4ParticleDefinition* partDef )
{
  G4bool isOK = FALSE;
  if( partDef == G4Alpha::AlphaDefinition() || 
      partDef == G4Deuteron::DeuteronDefinition() || 
      partDef == G4Triton::TritonDefinition() || 
      partDef == G4He3::He3Definition() ||
      partDef == G4GenericIon::GenericIonDefinition() ){
    isOK = TRUE;
  }
  return isOK; 
}


//------------------------------------------------------------------
G4TrackVector GmG4Utils::GetSecondariesOfCurrentStep()
{
  G4SteppingManager* fpSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();

  return GmG4Utils::GetSecondariesOfCurrentStep( fpSteppingManager );
}


//---------------------------------------------------------------------
G4TrackVector GmG4Utils::GetSecondariesOfCurrentStep(G4SteppingManager* fpSteppingManager )
{
 // number of secondaries in current step
  G4int nSecoStep = fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
  // list of secondaries since the beginning of track 
  const G4TrackVector* secondaryTracks = fpSteppingManager->GetSecondary();
  G4int nSecoTrack = G4int(secondaryTracks->size());
  // extract the last stepNseco tracks from the list 
  G4int ii;  
  G4TrackVector secondaries;
  for( ii = nSecoTrack-nSecoStep; ii < nSecoTrack; ii++ ){
    //  for( ite = secondaryTracks->rbegin(); ii < numberOfSecondaries; ite++,ii++ ){
    //    secondaries.push_back(*ite);
    secondaries.push_back( (*secondaryTracks)[ii] );
  }

  return secondaries;
}

//----------------------------------------------------------------
G4ThreeVector GmG4Utils::GetLocalFromGlobalPos( const G4ThreeVector globalPos, const G4NavigationHistory* navHis )
{
  const G4AffineTransform transform = navHis->GetTopTransform();
  G4ThreeVector localPos = transform.TransformPoint(globalPos);

  //  G4cout << " GmG4Utils::GetLocalFromGlobalPos " << localPos << " FROM " << globalPos << G4endl; //GDEB
  // G4cout << " GmG4Utils::GetLocalFromGlobalPos  transform " << transform << G4endl; //GDEB
  return localPos;
  
}


//----------------------------------------------------------------
G4ThreeVector GmG4Utils::GetLocalNFromGlobalPos( const G4ThreeVector globalPos, const G4NavigationHistory* navHis, G4int ancestorLevel )
{
  G4int navLevel = navHis->GetDepth()-ancestorLevel;
  if( navLevel < 0 ) navLevel = 0;
  const G4AffineTransform transform = navHis->GetTransform(navLevel);
  G4ThreeVector localPos = transform.TransformPoint(globalPos);
  //  G4cout << " GmG4Utils::GetLocalFromGlobalPos " << localPos << " FROM " << globalPos << G4endl; //GDEB
  // G4cout << " GmG4Utils::GetLocalFromGlobalPos  transform " << transform << G4endl; //GDEB
  return localPos;
  
}


//----------------------------------------------------------------
G4ThreeVector GmG4Utils::GetLocalFromGlobalDir( const G4ThreeVector globalDir, const G4NavigationHistory* navHis )
{
  const G4AffineTransform transform = navHis->GetTopTransform();
  G4ThreeVector localDir = transform.TransformAxis(globalDir);
  
  return localDir;
  
}

std::map<G4int,G4String> GmG4Utils::theElements;
std::map<G4String,G4int> GmG4Utils::theElementsInv;
typedef std::pair<G4String,G4String> pps;
typedef std::vector<pps> vpps;
//----------------------------------------------------------------
G4String GmG4Utils::GetInelasticName( const G4Step* aStep ) 
{
  const G4VProcess* proc  = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if( proc->GetProcessType() != fHadronic ) return "";

  const G4HadronicProcess* hadProc = (const G4HadronicProcess*)(proc);
  const G4Nucleus* aNucleus = hadProc->GetTargetNucleus();
  //  G4cout << " GmG4Utils::GetInelasticName::aStep NSECOS " << aNucleus << G4endl; //GDEG

  std::vector<G4String> secos;
  std::vector<G4Track*>::const_iterator itet;
  std::vector<G4Track*> secoTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  //  G4cout << " GmG4Utils::GetInelasticName::aStep NSECOS " << secoTracks.size() << G4endl;
  for( itet = secoTracks.begin(); itet != secoTracks.end(); itet++ ){
    G4ParticleDefinition* part = (*itet)->GetDefinition();
    //    G4cout << " GmG4Utils::GetInelasticName::aStep " << part->GetParticleName() << " baryon= " << part->GetBaryonNumber() << G4endl; //GDEB
    if( !part->GetBaryonNumber() ) continue;
    secos.push_back( part->GetParticleName() );
    //    G4cout << " GmG4Utils::GetInelasticName::aStep nSecosInV " << secos.size() << G4endl;
  }
//  G4cout << " GmG4Utils::GetInelasticName::aStep volume " << aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() << G4endl;

  return GetInelasticName( secos, aStep->GetTrack()->GetDefinition(), aNucleus );

}

//----------------------------------------------------------------
G4String GmG4Utils::GetInelasticName( std::vector<G4String> secos, const G4ParticleDefinition* primaryParticle, const G4Nucleus* targetNucleus ) 
{
  std::map<G4String,G4int> secosN;
  std::vector<G4String>::const_iterator ite;
  for( ite = secos.begin(); ite != secos.end(); ite++ ){
    secosN[(*ite)]++;
  }
  return GetInelasticName( secosN, primaryParticle, targetNucleus );
}

//----------------------------------------------------------------
G4String GmG4Utils::GetInelasticName( std::vector<G4ParticleDefinition*> secos, const G4ParticleDefinition* primaryParticle, const G4Nucleus* targetNucleus ) 
{
  std::map<G4String,G4int> secosN;
  std::vector<G4ParticleDefinition*>::const_iterator ite;
  for( ite = secos.begin(); ite != secos.end(); ite++ ){
    secosN[(*ite)->GetParticleName()]++;
  }
  return GetInelasticName( secosN, primaryParticle, targetNucleus );
}


//------------------------------------------------------------------
G4String GmG4Utils::GetInelasticName( std::map<G4String,G4int> secosN, const G4ParticleDefinition* primaryParticle, const G4Nucleus* targetNucleus )
{
  G4String channelName = "";

  G4String targetNucName = "";
  if( targetNucleus != 0 ){
    if( targetNucleus->GetZ_asInt() != 0 ) {
      targetNucName = GmG4Utils::GetElementNameFromZ(targetNucleus->GetZ_asInt() ) + GmGenUtils::itoa(targetNucleus->GetA_asInt() );
    } else {
      targetNucName = "ZZZ";
    }
    //    G4cout << " GmG4Utils::GetInelasticName: targetNucName from targetNucleus " << targetNucName << G4endl; //GDEB
  }
  G4String primaryParticleName = primaryParticle->GetParticleName();
  
  // First add ions
  std::map<G4String,G4int> secosNShortName;
  std::map<G4String,G4int>::reverse_iterator itepi;
  G4String ionName = "";
  for(itepi = secosN.rbegin(); itepi != secosN.rend(); itepi++) { 
    G4String particleName = (*itepi).first;
    //    G4int nPart = (*itepi).second;
  /* OLD GEANT4, ion O15[0.0], now O15
    size_t ic = particleName.find("[");
    G4cout << " GmG4Utils::GetInelasticName: seco " << particleName << " N " << nPart << G4endl; //GDEB
    if( ic != G4String::npos ) {
      if( ic == 0 ) {
	G4String newTargetNucName = particleName.substr(1,particleName.size());
	if( targetNucleus != 0  && ( targetNucName != newTargetNucName) ){
	  G4Exception("GmG4Utils::GetInelasticName",
		      "Warning",
		      JustWarning,
		      G4String("Target nucleus has a name: " + targetNucName + " different than the one appearing in list of secondaries: " + targetNucName + " Please check ").c_str());
	}
	targetNucName = particleName.substr(1,particleName.size());
	//	G4cout << " GmG4Utils::GetInelasticName: targetNucName from seco loop " << targetNucName << G4endl;

      } else {
	if( ionName != "" ) {
	  ionName += "-";
	}
	if( nPart == 1 )  {
	  ionName += particleName.substr(0,ic);
	}else {
	  ionName +=  GmGenUtils::itoa(nPart) + particleName.substr(0,ic);
	}
	//	G4cout << " GmG4Utils::GetInelasticName: ionName " << ionName << G4endl; //GDEB

      }
    } else{
      G4String particleNameShort = GetParticleShortName( particleName );
      secosNShortName[particleNameShort] = (*itepi).second;
      //      G4cout << " GmG4Utils::GetInelasticName: secoPart add " << particleNameShort << " : " << particleName << G4endl; //GDEB

    } */
    if( particleName == "neutron" || particleName == "proton" || particleName == "deuteron"  || particleName == "triton" || particleName == "He3" || particleName == "alpha" ) {
      G4String particleNameShort = GetParticleShortName( particleName );
      secosNShortName[particleNameShort] = (*itepi).second;
      //      G4cout << " GmG4Utils::GetInelasticName: secoPart add " << particleNameShort << " : " << particleName << G4endl; //GDEB
    } else {
      if( ionName != "" ) ionName += "+";
      ionName += particleName;
    }
  }
  // Now non ions particles in order
  std::string order = "npdtha";
  G4String nonIonNames = "";
  for( int ii = 0; ii < 6; ii++ ){
    std::string part = order.substr(ii,1);
    std::map<G4String,G4int>::const_iterator itepni = secosNShortName.find(part);
    if( itepni != secosNShortName.end() ) {
      if( (*itepni).second == 1 )  {
	nonIonNames += (*itepni).first;
      }else {
	//	      channelName += GmGenUtils::itoa((*itepni).second) + (*itepni).first;
	nonIonNames += GmGenUtils::itoa((*itepni).second) + (*itepni).first;
      } 
      //      G4cout << " GmG4Utils::GetInelasticName: nonIonNames " << nonIonNames << G4endl;

    }
  }
  
  // if no target nucleus found, built it from A/Z 
  if( targetNucName == "" ) {
    targetNucName = GetNucleusNameFromSecos( secosNShortName, primaryParticle );
    //    G4cout << " GmG4Utils::GetInelasticName: targetNucName from secos!! " << targetNucName << G4endl;
  }
  
  channelName = targetNucName + "(" + GetParticleShortName(primaryParticle->GetParticleName()) + "," + nonIonNames + ")" + ionName;
  
  //  G4cout << " GmG4Utils::GetInelasticName: CHANNEL NAME " << channelName << G4endl;

  return channelName;
}
				      

//------------------------------------------------------------------
G4String GmG4Utils::GetNucleusNameFromSecos( std::map<G4String,G4int> secosN, const G4ParticleDefinition* primaryParticle )
{
  G4String targetNucleusName = "";

  if( theElements.size() == 0 ) BuildElementNames();

  std::map<G4String,G4int>::const_iterator ites;
  G4int Aplus = 0;
  G4int Zplus = 0;

  for( ites = secosN.begin(); ites != secosN.end(); ites++ ){
    if( (*ites).first == "neutron" ) {
      Aplus += (*ites).second;
    } else if( (*ites).first == "proton" ) {
      Zplus += (*ites).second;
      Aplus += (*ites).second;
    } else if( (*ites).first == "deuteron" ) {
      Zplus += (*ites).second;
      Aplus += (*ites).second*2;
    } else if( (*ites).first == "triton" ) {
      Zplus += (*ites).second;
      Aplus += (*ites).second*3;
    } else if( (*ites).first == "He3" ) {
      Zplus += (*ites).second*2;
      Aplus += (*ites).second*3;
    } else if( (*ites).first == "alpha" ) {
      Zplus += (*ites).second*2;
      Aplus += (*ites).second*4;
    } else if( (*ites).first.find("[") != std::string::npos ) {
      // Get Z and A of ion
      // first decompose ion name in symbol + A
      G4int firstNumber = -1;
      G4String newIonName = (*ites).first;
      size_t fp = newIonName.find("[");
      newIonName = newIonName.substr( 0,fp );
      for( unsigned int ii = 0; ii < (*ites).first.length(); ii++ ){
	G4String part = newIonName.substr(ii,1);
	if( GmGenUtils::IsNumber(part) ) {
	  firstNumber = ii; 
	  break;
	}
      } 
      G4String newIonSymbol = newIonName.substr(0,firstNumber);
      Zplus += theElementsInv[newIonSymbol];
      Aplus += G4int(GmGenUtils::GetValue((*ites).first.substr(firstNumber,999)));
    } else {      // is gamma or other
    }
  }

  G4String newIonName;
  G4String projectileName;
  G4String reactionName = "";
  
  G4String projectileNameShort = "";
  
  Zplus -= primaryParticle->GetAtomicNumber();
  Aplus -= primaryParticle->GetAtomicMass();
  
  targetNucleusName = "#"+ theElements[Zplus] + GmGenUtils::itoa(Aplus);
  
  return targetNucleusName;
} 
  
//------------------------------------------------------------------
void GmG4Utils::BuildElementNames()
{
  G4String fileName = "elementsZ.lis";
  fileName = GmGenUtils::FileInPath( fileName );

  G4tgrFileIn fin = G4tgrFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    theElements[G4int(GmGenUtils::GetValue(wl[1]))] = wl[0];
    theElementsInv[wl[0]]= G4int(GmGenUtils::GetValue(wl[1]));
  }

}

//------------------------------------------------------------------
G4String GmG4Utils::GetElementNameFromZ(G4int Z)
{
  if( theElements.size() == 0 ) BuildElementNames();
  std::map<G4int,G4String>::const_iterator ite = theElements.find(Z);
  if( ite == theElements.end() ) {
    for( ite = theElements.begin(); ite != theElements.end(); ite++ ){
      G4cerr << " ELEMENT Z " << (*ite).first << " = " << (*ite).second << G4endl;
    }
    G4Exception("GmG4Utils::GetElementNameFromZ",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Z value not found: "+GmGenUtils::itoa(Z)).c_str());
  }
  return (*ite).second;
}


//------------------------------------------------------------------
G4int GmG4Utils::GetElementZFromName(G4String name)
{
  if( theElementsInv.size() == 0 ) BuildElementNames();
  std::map<G4String, G4int>::const_iterator ite = theElementsInv.find(name);
  if( ite == theElementsInv.end() ) {
    for( ite = theElementsInv.begin(); ite != theElementsInv.end(); ite++ ){
      G4cerr << " ELEMENT " << (*ite).first << " Z= " << (*ite).second << G4endl;
    }
    G4Exception("GmG4Utils::GetElementZFromName",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Name not found: "+name).c_str());
  }
  return (*ite).second;
}


//------------------------------------------------------------------
G4bool GmG4Utils::CheckProcessExists( G4ProcessManager* pmanager, const G4String& procName, G4bool bWarning )
{
  G4bool bFound = false;
  G4ProcessVector* procList = pmanager->GetProcessList();
  for( size_t ii = 0; ii < procList->size(); ii++) {
    if( (*procList)[ii]->GetProcessName() == procName) {
      bFound = true;
      break;
    }
  } 
    
  if( bFound && bWarning ) {
    G4Exception("GmG4Utils::CheckProcessExists",
		"Warning",
		JustWarning,
		G4String("Adding a process to a particle that already has it: " + procName + " to " + pmanager->GetParticleType()->GetParticleName()).c_str());
  } 

  return bFound;
  
} 

//------------------------------------------------------------------
G4String GmG4Utils::GetParticleShortName( G4String name )
{

  if( name.find("[") != G4String::npos ) return name;

  if( name == "neutron" ) {
    return "n";
  } else if( name == "proton" ) {
    return "p";
  } else if( name == "deuteron" ) {
    return "d";
  } else if( name == "triton" ) {
    return "t";
  } else if( name == "He3" ) {
    return "h";
  } else if( name == "alpha" ) {
    return "a";
  } else if( name == "gamma" ) {
    return "g";
  } else {
    return "X";
  }
 
}

//---------------------------------------------------------------------------
G4DecayTable* GmG4Utils::FindOrBuildDecayTable( const G4ParticleDefinition* part )
{
  std::map<const G4ParticleDefinition*,G4DecayTable*>::const_iterator ite = thePartDecayTable.find(part);
  G4DecayTable* decayTable = 0;
  if( ite == thePartDecayTable.end() ) {
    G4ProcessManager* pmanager = part->GetProcessManager();
    G4ProcessVector* procList = pmanager->GetProcessList();
    for( size_t ii = 0; ii < procList->size(); ii++) {
      G4RadioactiveDecay* decayProc = dynamic_cast<G4RadioactiveDecay*>((*procList)[ii]);
      if( decayProc ) {
	decayTable = decayProc->GetDecayTable(part);
	thePartDecayTable[part] = decayTable;
	break;
      }
    }
  } else {
    return ite->second;
  }
  
  return decayTable;
}


G4ParticleDefinition* GmG4Utils::GetG4Particle( const G4String particleName, G4bool bMustExist )
{
  G4ParticleDefinition* part = 0;
  
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    if( particle->GetParticleName() == particleName ) {
      part = particle;
      break;
    }
  }
  
  if( part == 0 && bMustExist ){
    G4Exception("GmG4Utils::GetG4Particle",
		"ERROR",
		FatalErrorInArgument,
		("particle name not found " + particleName ).c_str() );
  }

  return part;
}

//----------------------------------------------------------------
G4String GmG4Utils::GetNameNumberFromTouchable( const G4VTouchable* touch )
{
  if( !touch ) return "OutOfWorld";
  if( !touch->GetVolume() ) return "OutOfWorld";
  
  G4String name = touch->GetVolume()->GetName();
  G4String number = GmGenUtils::itoa(touch->GetReplicaNumber());

  G4String nameNum = name+":"+number;

  return nameNum;
}

//----------------------------------------------------------------
G4ProcessVector* GmG4Utils::GetGammaProcessVector(G4ProcessVector* procVectorOrig )
{
  G4ProcessVector* procVectorNew = new G4ProcessVector();
  for( size_t ii = 0; ii < procVectorOrig->size(); ii++ ) {
    G4VEmProcess* process = (G4VEmProcess*)((*procVectorOrig)[ii]);
    G4GammaGeneralProcess* ggproc = dynamic_cast<G4GammaGeneralProcess*>((*procVectorOrig)[ii]);
    if( ! ggproc ) {
      procVectorNew->insert(process);
    } else {
      if( ggproc->GetPhotoElectric() != 0 ) {
	procVectorNew->insert( ggproc->GetPhotoElectric() );
      }
      if(ggproc->GetCompton() != 0 ) {
	procVectorNew->insert( ggproc->GetCompton() );
      }
      if( ggproc->GetConversionEE() != 0 ) {
	procVectorNew->insert( ggproc->GetConversionEE() );
      }
      if( ggproc->GetRayleigh() != 0 ) {
	procVectorNew->insert( ggproc->GetRayleigh() );
      }
      if( ggproc->GetConversionMM() != 0 ) {
	procVectorNew->insert( ggproc->GetConversionMM() );
      }
    }
  }

  return procVectorNew;
}
