#include "GmBiasingMgr.hh"

#include "GmGenericBiasingPhysics.hh"
#include "G4BiasingHelper.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTable.hh"
#include "G4VModularPhysicsList.hh"
#include "G4RunManager.hh"

#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
#include "GmBiasingOperatorFactory.hh"
#endif

//----------------------------------------------------------------------
GmBiasingMgr* GmBiasingMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmBiasingMgr::GmBiasingMgr()
{	
  theBiasingPhysics = 0;
}

//----------------------------------------------------------------------
GmBiasingMgr* GmBiasingMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmBiasingMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmBiasingMgr::~GmBiasingMgr()
{
}

//-----------------------------------------------------------------------
void GmBiasingMgr::CreateOperator(std::vector<G4String> params )
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddOperator",
		"BM001",
		FatalErrorInArgument,
		G4String("At least three arguments must be supplied in command '/gamos/physics/biasing/createOperator' : NAME TYPE.  They are: "+parastr).c_str());
  }
  
  G4String operName = params[0];
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  if( ite != theOperators.end() ) {
    G4Exception("GmBiasingMgr::AddOperator",
		"BM002",
		JustWarning,
		G4String("Operator already exists " + operName).c_str());
  }
  
  G4String operType = params[1];

#ifdef ROOT5
  GmVBiasingOperator* theBOper = Reflex::PluginService::Create<GmVBiasingOperator*>("GmBOptr"+operType,operName);
#else
  GmVBiasingOperator* theBOper = GmBiasingOperatorFactory::get()->create("GmBOptr"+operType,operName);
  #endif
    if( theBOper == 0 ) {
      G4Exception("GmBiasingMgr::CreateOperator",
	 	  "BOper1",
		  FatalErrorInArgument,
		  ("Biasing operator  " + operType + " does not exist").c_str());
    }

   theOperators[operName] = theBOper;

  //prepare physics
  if( !theBiasingPhysics ) {
    theBiasingPhysics = new GmGenericBiasingPhysics();
    G4VUserPhysicsList* physicsList = const_cast<G4VUserPhysicsList*>(G4RunManager::GetRunManager()->GetUserPhysicsList());
    if( !physicsList ) {
      G4Exception("GmBiasingMgr::CreateOperator",
		  "BM004",
		  FatalErrorInArgument,
		  "No physics list created yet: '/gamos/physics/biasing/createOperator'");
    }
    G4VModularPhysicsList* modularPhysicsList = dynamic_cast<G4VModularPhysicsList*>(physicsList);
    if( !modularPhysicsList ) {
      G4Exception("GmBiasingMgr::CreateOperator",
		  "BM004",
		  FatalErrorInArgument,
		  "Physics list is not modular, try another physics list");
    }
    modularPhysicsList->RegisterPhysics(theBiasingPhysics);
  }
  
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AssocOper2LogVol(std::vector<G4String> params )
{
  if( params.size() < 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AssocOper2LogVol",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/associateProcess' : LOGICAL_VOLUE_1 LOGICAL_VOLUME_2 ... LOGICAL_VOLUME_N OPERATOR_NAME.  They are: "+parastr).c_str());
  }
  
  G4String operName = params[params.size()-1];
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  if( ite == theOperators.end() ) {
    G4Exception("GmBiasingMgr::AddOperator",
		"BM002",
		FatalException,
		G4String("Operator does not exist " + operName).c_str());
  }
  GmVBiasingOperator* biasOper = (*ite).second;
  
  std::vector<G4LogicalVolume*> logvols;
  for( unsigned int ii = 0; ii < params.size()-1; ii++ ){
    std::vector<G4LogicalVolume*> logvols1 = GmGeometryUtils::GetInstance()->GetLogicalVolumes( params[ii], FALSE );
    
    if( logvols1.size() == 0 ) {
      GmGeometryUtils::GetInstance()->DumpG4LVTree();
      G4Exception("GmBiasingMgr::AssocOper2LogVol",
		  " Cannot associate sensitive detector to a logical volume that does not exist ",
		  FatalErrorInArgument,
		  ("Command issued:  /gamos/biasing/assocOper2LogVol " +  params[ii]).c_str() );
    }
    for( size_t il = 0; il < logvols1.size(); il++ ) {
      logvols.push_back( logvols1[il] );
    }
  }
  
  for( unsigned int jj = 0; jj < logvols.size(); jj++ ){
    G4LogicalVolume* logvol = logvols[jj];
    biasOper->AttachTo(logvol);
  }
    
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddProcesses2Oper(std::vector<G4String> params)
{
  if( params.size() < 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddProcesses",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/addProcesses' : PROCESS_1 PROCESS_2 ... PROCESS_N.  They are: "+parastr).c_str());
  }


  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    G4ProcessManager*         pmanager = particle->GetProcessManager();
    
    G4ProcessVector* vprocess = pmanager->GetProcessList();
    std::vector< G4String > processToBiasNames;
    for (size_t ip = 0 ; ip < vprocess->size() ; ip++) {
      for( size_t ii = 0; ii < params.size()-1; ii++ ){
	if( GmGenUtils::AreWordsEquivalent( params[ii], (*vprocess)[ip]->GetProcessName() ) ) {
	  processToBiasNames.push_back( (*vprocess)[ip]->GetProcessName() );
	  break;
	}
      }
    }
    if( processToBiasNames.size() != 0 ) {
      theBiasingPhysics->PhysicsBias( particleName, processToBiasNames );
    }
    
  }
  
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddParticles2Oper(std::vector<G4String> params )
{
  if( params.size() < 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddParticles2Oper",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/addParticles2Oper' : PARTICLE_1 PARTICLE_2 ... PROCESS_N.  They are: "+parastr).c_str());
  }

  //--- find biasing operator
  G4String operName = params[params.size()-1];
  GmVBiasingOperator* biasOperator = 0;
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  if( ite == theOperators.end() ) {
    G4Exception("GmBiasingMgr::AssocPartProc2Oper",
		"BM005",
		JustWarning,
		G4String("Operator does not exist " + operName).c_str());
  } else {
    // check if NonPhys operator (for the moment all Phys)
    biasOperator = (*ite).second;
  }
  G4bool bOperNonPhys = biasOperator->IsNonPhysicsBiased();
 
  // check if NonPhys operator (for the moment all Phys)
  //  G4bool bOperPhys = true;

  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    for( size_t ii = 0; ii < params.size()-1; ii++ ){
      if( GmGenUtils::AreWordsEquivalent( params[ii], particleName ) ) {
	if( bOperNonPhys ) {
	  theBiasingPhysics->Bias( particleName ); // both, Bias / NonBias
	} else {
	  //	  theBiasingPhysics->NonPhysicsBias( particleName );
	  theBiasingPhysics->PhysicsBias( particleName );
	}
	biasOperator->AddParticle( params[ii] );
	break;
      }
    }
  }
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddParticleProcesses2Oper(std::vector<G4String> params )
{
  if( params.size() < 3 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddParticleProcesses2Oper",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/addParticleProcesses2Oper' : PARTICLE PROCESS_1 PROCESS_2 ... PROCESS_N OPERATOR_NAME.  They are: "+parastr).c_str());
  }

  //--- find biasing operator
  G4String operName = params[params.size()-1];
  GmVBiasingOperator* biasOperator = 0;
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  //  G4bool bOperPhys = false;
  if( ite == theOperators.end() ) {
    G4Exception("GmBiasingMgr::AssocPartProc2Oper",
		"BM005",
		JustWarning,
		G4String("Operator does not exist " + operName).c_str());
  } else {
    // check if NonPhys operator (for the moment all Phys)
    biasOperator = (*ite).second;
    //    bOperPhys = bOperator->IsPhysical();
  }
  
  std::vector< G4String > processToBiasNames;
  for( size_t ii = 1; ii < params.size()-1; ii++ ) {
    processToBiasNames.push_back(params[ii]);
    biasOperator->AddParticleProcess( params[0], params[ii] );
  }
  theBiasingPhysics->PhysicsBias( params[0], processToBiasNames );
  
  /*
  
  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    G4ProcessManager*         pmanager = particle->GetProcessManager();
    G4bool bPartOK = false;
    if( GmGenUtils::AreWordsEquivalent( params[0], particleName ) ) {
      bPartOK = true;
      break;
    }

    if( bPartOK ) {
      G4ProcessVector* vprocess = pmanager->GetProcessList();
      std::vector< G4String > processToBiasNames;
      for (size_t ip = 0 ; ip < vprocess->size() ; ip++) {
	if( GmGenUtils::AreWordsEquivalent( params[1], (*vprocess)[ip]->GetProcessName() ) ) {
	  processToBiasNames.push_back( (*vprocess)[ip]->GetProcessName() );
	}
      }   
      if( processToBiasNames.size() != 0 ) {
	theBiasingPhysics->PhysicsBias( particleName, processToBiasNames );
      }

    }
    
  }
  */
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddSplittingProcess( std::vector<G4String> params)
{
  theBiasingPhysics->AddSplittingProcess(params);
}
