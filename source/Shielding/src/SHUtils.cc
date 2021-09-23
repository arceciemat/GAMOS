#include "SHUtils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4HadronicProcess.hh"

#include "G4tgrFileIn.hh"
#include "G4HadronicProcessStore.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"

#include <iomanip>
std::vector<G4HadronicProcess*> SHUtils::theHadronicProcesses;
std::map<G4String, ChannelData > SHUtils::theChannelParticles;
std::map<G4int,G4String> SHUtils::theElements;
std::map<G4double,G4int> SHUtils::theProductIDs;

//------------------------------------------------------------------
SHUtils::SHUtils() 
{
}

//------------------------------------------------------------------
void SHUtils::StoreHadronicProcesses(G4String primaryParticleName)
{
  //-- Get process manager of particle
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  G4ProcessManager* pmanager = 0;
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    //    G4cout << ":SHUtils  PARTICLE " << particle->GetParticleName() << " lepton " << particle->GetLeptonNumber() << G4endl;
    if( particle->GetParticleName() == primaryParticleName ){
      //      G4cout << "SHUtils:  PARTICLE is " << particle->GetParticleName() << G4endl;
      pmanager = particle->GetProcessManager();

      G4ProcessVector* pvect = pmanager->GetProcessList();
      int jj, sizproc = pvect->size();
      for( jj = 0; jj < sizproc; jj++ ) {
	G4VProcess* proc = (*pvect)[jj];
	if( proc->GetProcessType() != fHadronic ) continue;
	G4HadronicProcess* prochad = dynamic_cast<G4HadronicProcess*>(proc);
	//	G4cout << " STORING PROCESS " << proc << " -> " <<  prochad << " " << proc->GetProcessName() << G4endl;
	if( prochad ) {
	  theHadronicProcesses.push_back( prochad );
	  //	  G4cout << "SHUtils::StoreNeutronProcesses proc= " << prochad->GetProcessName() << G4endl;
	  
	  //	  G4cout << "%%%%% TABLE OF CROSS SECTIONS FOR PROCESS " << prochad->GetProcessName() << " PARTICLE " << particle->GetParticleName() << G4endl;
	  //	  prochad->DumpPhysicsTable(*particle);
	}
      }
    }
    
  }

}


//------------------------------------------------------------------
void SHUtils::BuildChannelList(const G4ParticleDefinition* particle, G4String UAName)
{
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue(UAName+":ChannelListFileName","channel.lis");
  fileName = GmGenUtils::FileInPath( fileName );

  G4tgrFileIn fin = G4tgrFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4int nProtonsProjectile = -particle->GetAtomicNumber();
  G4int nNeutronsProjectile = -(particle->GetAtomicMass() - nProtonsProjectile);
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    if( wl[0].substr(0,1) == "(" ){
      G4int nProtons = nProtonsProjectile;
      G4int nNeutrons = nNeutronsProjectile;
      if( wl[0] == "(n,n')" ) wl[0] =  "(n,n)"; //??
      G4String particles = wl[0].substr(3,wl[0].length()-4);
      if( particles == "anything" ) particles = "X";
      //      G4cout << " CHANNEL " << wl[1] << " " << particles << G4endl;

      //--- Change 3He by He3
      size_t fhe = particles.find("3He");
      if( fhe != std::string::npos ) particles = particles.substr(0,fhe)+"He3"+particles.substr(fhe+3,9999);
      //---- Loop to letters in channel name
      for( unsigned int ii = 0; ii < particles.length(); ii++ ){
	ChannelData channelData;

	G4int nPart = 1;
	G4String part = particles.substr(ii,1);
	if( GmGenUtils::IsNumber(part) ) {
	  nPart = G4int( GmGenUtils::GetValue(part) );
	  ii++;
	  part = particles.substr(ii,1);
	}
	if( part == "H" ) { // He3
	  ii++;
	  part += particles.substr(ii,1);
	  ii++;
	  part += particles.substr(ii,1);
	}

	// process particle name
	if( part == "p" ) {
	  nProtons += nPart;
	} else if( part == "n" ) {
	  nNeutrons += nPart; 	 
	} else if( part == "d" ) {
	  nProtons += nPart; 	 
	  nNeutrons += nPart; 	 
	} else if( part == "t" ) {
	  nProtons += nPart;
	  nNeutrons += 2*nPart;
	} else if( part == "He3" ) {
	  nProtons += 2*nPart; 
	  nNeutrons += nPart; 	 
	} else if( part == "a" ) {
	  nProtons += 2*nPart; 	 
	  nNeutrons += 2*nPart;
	} else if( part == "X" ) {
	} else {
	  G4Exception("SHUtils",
		      "Wrong particle name in file",
		      FatalErrorInArgument,
		      G4String("It can only be p / n / d / t / He3 /a , and it is " + part).c_str());
	}

	channelData.name = particles;
	channelData.nProtons = nProtons;
	channelData.nNeutrons = nNeutrons;
	theChannelParticles[wl[1]] = channelData;
	//	G4cout << " theChannelParticles " << wl[1] << " = " << channelData.name << " nn " << channelData.nNeutrons << " np " << channelData.nProtons << G4endl;
      }
    }
  }
}

//------------------------------------------------------------------
void SHUtils::BuildElementNames()
{
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue("SHUtils:ElementsFileName","elementsZ.lis");
  fileName = GmGenUtils::FileInPath( fileName );

  G4tgrFileIn fin = G4tgrFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    theElements[G4int(GmGenUtils::GetValue(wl[1]))] = wl[0];
  }    

}


//------------------------------------------------------------------
G4String SHUtils::GetChannelName(G4int targetZ, G4int targetA, G4String channelName, std::string projectile)
{
  std::map<G4String, ChannelData >::const_iterator ite = theChannelParticles.find(channelName);
  ChannelData CD = (*ite).second;
  G4String reactionName = GetElementName(targetZ)+GmGenUtils::itoa(targetA) 
    + "("+projectile+"," 
    + CD.name 
    + ")" 
    + GetElementName(targetZ-CD.nProtons)
    + GmGenUtils::itoa(targetA-CD.nProtons-CD.nNeutrons);

  //  G4cout << " GetChannelName CD used " << CD.name << " " << CD.nProtons << " " << CD.nNeutrons << " targetZ " << targetZ << " targetA " << targetA << G4endl;

  return reactionName;

}

//------------------------------------------------------------------
G4String SHUtils::GetElementName( G4int Z ) 
{
  return theElements[Z];
}

//------------------------------------------------------------------
G4String SHUtils::GetProductName( G4int massCode )
{
  G4String productName = "";
  switch(massCode) {
  case 0:
    productName = "g";
    break;
  case 1:
    productName = "n";
    break;
  case 1001:
    productName = "p";
    break;
  case 1002:
    productName = "d";
    break;
  case 1003:
    productName = "t";
    break;
  case 2003:
    productName = "He3";
    break;
  case 2004:
    productName = "a";
    break;
  default:
    G4int A = massCode % 1000;
    G4int Z = massCode / 1000;
    productName = GetElementName(Z)+GmGenUtils::itoa(A);    
    break;
  }

  return productName;

}

//------------------------------------------------------------------
G4int SHUtils::GetProductID( G4double massCode )
{
  G4int pID = -1;
  std::map<G4double,G4int>::const_iterator ite = theProductIDs.find(massCode);
  if( ite == theProductIDs.end() ) {
    G4int nprod = theProductIDs.size();
    theProductIDs[massCode] = nprod;
    pID = nprod;
  } else {
    pID = (*ite).second;
  }

  return pID;
}

//------------------------------------------------------------------
G4int SHUtils::GetProcessID( const G4HadronicProcess* hadProc )
{
 
  for( unsigned int ii = 0; ii != theHadronicProcesses.size(); ii++ ) {
    if( theHadronicProcesses[ii] == hadProc ) return ii;
  }

  G4Exception("SHUtils::GetProcessID",
	      "Error",
	      FatalException,
	      ("Process not found " + hadProc->GetProcessName()).c_str());
  return 0;
}
