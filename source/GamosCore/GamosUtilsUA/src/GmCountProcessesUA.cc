#include "GmCountProcessesUA.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

#include "G4Track.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4EmCalculator.hh"
#include "G4Proton.hh"
#include "G4NistManager.hh"
//#define DEBUG

//-----------------------------------------------------------------
GmCountProcessesUA::GmCountProcessesUA()
{ 
}


//-----------------------------------------------------------------
GmCountProcessesUA::~GmCountProcessesUA() 
{
}

//-----------------------------------------------------------------
void GmCountProcessesUA::BeginOfRunAction(const G4Run* )
{
  int ii, siz;
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  siz = partTable->size();
  for( ii= 0; ii < siz; ii++ ){
    G4ParticleDefinition* particle = partTable->GetParticle(ii);
    G4String particleName = particle->GetParticleName();
#ifdef DEBUG
    G4cout << ii << " PCA " << particleName<< " " << particle->GetPDGStable() << " " << particle->IsShortLived() << G4endl;
#endif
    //    theParticleCount[particleName] = 0;

    //--- All processes of this particle 
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if( pmanager ) {
      G4ProcessVector* pvect = pmanager->GetProcessList();
      int jj, sizproc = pvect->size();
      for( jj = 0; jj < sizproc; jj++ ) {
	G4String processName = (*pvect)[jj]->GetProcessName();
#ifdef DEBUG
	G4cout << jj << " Process Name " << processName<< G4endl;
#endif
	theProcessList.insert( mss::value_type(particleName,processName) );
	if( processName.find("Ioni") != std::string::npos ){
	  theProcessList.insert( mss::value_type(particleName,processName+"_NoSeco") );
	}
	if( processName.find("Brem") != std::string::npos ){
	  theProcessList.insert( mss::value_type(particleName,processName+"_NoSeco") );
	}
      }
    } else {
      G4Exception("GmCountProcessesUA::BeginOfRunAction",
		  "No manager found for particle",
		  JustWarning,
		  G4String("Particle= " + particle->GetParticleName()).c_str());
    }
  }

  for( ii = 0; ii < G4int(theFilters.size()); ii++ ){
    //    if( ii != 0 )
    theNameSuffix += "_";
    theNameSuffix += theFilters[ii]->GetName();
  }
  if( theClassifier ) {
    //    if( ii != 0 )
    theNameSuffix += "_";
    theNameSuffix += theClassifier->GetName();
  }

  //  if( !theClassifier ) G4cout << "@@@@@ GmCountProcessesUA " << theName << " SUFF " << theNameSuffix << G4endl;
  G4bool bPrintPROC_LIST = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintProcList",0));
  if( bPrintPROC_LIST ) DumpProcessList();
 
  bCreatorModel = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName + ":PrintCreatorModel",0));

}


//-----------------------------------------------------------------
void GmCountProcessesUA::PreUserTrackingAction(const G4Track* aTrack)
{
  long int index = 0;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromTrack(aTrack);
  }

  //----- Fill counter of particles
  G4String particleName = aTrack->GetDefinition()->GetParticleName();
  if( aTrack->GetCurrentStepNumber() == 0 ) { // not when Elastic creates a new track of the same particle
    mlmsi::iterator itep = theParticleCount.find(index);
    msi* particleCount = 0;
    if( itep == theParticleCount.end() ) {
      particleCount = new msi;
      theParticleCount[index] = particleCount;
    } else {
      particleCount = (*itep).second;
    }
    (*particleCount)[particleName]++;
    
    //----- Fill counter of Creator Processes
    const G4VProcess* proc = aTrack->GetCreatorProcess();
    G4String processName;
    if( proc != 0 ) {
      processName = proc->GetProcessName();
    } else {
      processName = "Primary";
    }
    G4String modelName = aTrack->GetCreatorModelName();
    if( bCreatorModel ) processName += "_" + aTrack->GetCreatorModelName();
    mlmpssi::iterator itecp = theCreatorProcessCount.find(index);
    mpssi* creatorProcessCount = 0;
    if( itecp == theCreatorProcessCount.end() ) {
      creatorProcessCount = new mpssi;
      theCreatorProcessCount[index] = creatorProcessCount;
    } else {
      creatorProcessCount = (*itecp).second;
    }
    pss parproc(particleName,processName);
    mpssi::iterator ite = creatorProcessCount->find( parproc );
    if( ite == creatorProcessCount->end() ) {
      (*creatorProcessCount)[ parproc ] = 1;
    } else {
      (*ite).second = (*ite).second +1; 
    }
#ifdef DEBUG
    G4cout << " creator " << particleName << " " << processName << " " << creatorProcessCount->size() << G4endl;
#endif
    
  }
}


//-----------------------------------------------------------------
void GmCountProcessesUA::UserSteppingAction(const G4Step* aStep )
{
  long int index = 0;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  }

  G4String processName;
  if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL){
     processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
     if( processName.find("Ioni") != std::string::npos || 
	 processName.find("Brem") != std::string::npos ) {
       if( fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt() == 0){
	 processName += "_NoSeco";
       }
       //       G4cout << " EIONI " << processName << G4endl;
     }
  } else {

    processName = "User Limit";
  }

  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

  mlmpssi::iterator itecp = theProcessCount.find(index);
  mpssi* processCount = 0;
  if( itecp == theProcessCount.end() ) {
    processCount = new mpssi;
    theProcessCount[index] = processCount;
  } else {
    processCount = (*itecp).second;
  }
  (*processCount)[pss(particleName,processName)] = (*processCount)[pss(particleName,processName)] + 1;
#ifdef DEBUG
  G4cout << " process list " << particleName << " " << processName << " " << (*processCount)[pss(particleName,processName)] << G4endl;
#endif

}


//-----------------------------------------------------------------
void GmCountProcessesUA::EndOfRunAction(const G4Run* )
{
  G4cout << "@@@@ " << theName << G4endl;
  DumpParticleCount();
  DumpProcessCount();
  DumpCreatorProcessCount();
}


//-----------------------------------------------------------------
void GmCountProcessesUA::DumpProcessList( std::ostream& out)
{    
  mss::iterator ite;
  G4String part = "";
  for( ite = theProcessList.begin(); ite != theProcessList.end(); ite++ ) {
    out << "PROC_LIST " << (*ite).first << " : " <<(*ite).second << G4endl; 
  }
  
}

//-----------------------------------------------------------------
void GmCountProcessesUA::DumpProcessCount( std::ostream& out)
{ 
  mlmpssi::iterator itemp;
  for( itemp = theProcessCount.begin(); itemp != theProcessCount.end(); itemp++ ) {
    if( theClassifier ) {
      G4cout << "@@@@@ " << theName << " : " << theClassifier->GetIndexName((*itemp).first) << G4endl;
    } 
    mpssi* processCount = (*itemp).second;
    mpssi::iterator ite;
    std::map<G4String,G4double> totalProcCount;
    G4String part = "";
    for( ite = processCount->begin(); ite != processCount->end(); ite++ ) {
      if( (*ite).second != 0 ) {
	totalProcCount[(*ite).first.first] += (*ite).second;
	if( part != (*ite).first.first && part != "") {
	  if( theClassifier ) out << theClassifier->GetIndexName((*itemp).first)  << " ";
	  out << " PROC_COUNT " << part << " : ALL = " << totalProcCount[part] << G4endl;
	}
	part = (*ite).first.first;
	if( theClassifier ) out << theClassifier->GetIndexName((*itemp).first)  << " ";
	out << " PROC_COUNT " << (*ite).first.first << " : " <<(*ite) .first.second << " = " << (*ite).second << G4endl;
      }
    }
    
    if( theClassifier ) out << theClassifier->GetIndexName((*itemp).first)  << " ";
    out << "PROC_COUNT " << part << " : ALL = " << totalProcCount[part] << G4endl;
  }

}

//-----------------------------------------------------------------
void GmCountProcessesUA::DumpCreatorProcessCount( std::ostream& out)
{    
  mlmpssi::iterator itemp;
  for( itemp = theCreatorProcessCount.begin(); itemp != theCreatorProcessCount.end(); itemp++ ) {
    if( theClassifier ) {
      G4cout << "@@@@@ " << theName << " : " << theClassifier->GetIndexName((*itemp).first) << G4endl;
    } 
    mpssi* creatorProcessCount = (*itemp).second;
    mpssi::iterator ite;
    for( ite = creatorProcessCount->begin(); ite != creatorProcessCount->end(); ite++ ) {
      if( (*ite).second != 0 ) {
	if( theClassifier ) out << theClassifier->GetIndexName((*itemp).first)  << " ";
	out << "PROC_CREATOR_COUNT " << (*ite).first.first << " : " <<(*ite) .first.second << " = " << (*ite).second << G4endl; 
      }
    }
  }

}

//-----------------------------------------------------------------
void GmCountProcessesUA::DumpParticleCount( std::ostream& out)
{    
  mlmsi::iterator itemp;
  for( itemp = theParticleCount.begin(); itemp != theParticleCount.end(); itemp++ ) {
    if( theClassifier ) {
      G4cout << "@@@@@ " << theName << " : " << theClassifier->GetIndexName((*itemp).first) << G4endl;
    } 
    msi* particleCount = (*itemp).second;
    msi::iterator itep;
    for( itep = particleCount->begin(); itep != particleCount->end(); itep++ ) {
      if( (*itep).second != 0 ) {
	if( theClassifier ) out << theClassifier->GetIndexName((*itemp).first)  << " ";
	out << " PART_COUNT: " << (*itep).first << " = " << (*itep).second << G4endl; 
      }
    }
  }

}
