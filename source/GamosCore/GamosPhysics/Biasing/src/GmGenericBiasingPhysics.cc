//---------------------------------------------------------------------------
//
// ClassName:   GmGenericBiasingPhysics
//
// Author:      M. Verderi (Sept.10.2013)
// Modified:
// 07/11/2014, M. Verderi : fix bug of PhysicsBias(...) which was not taking
//             into account the vector of processes passed, but biasing all.
//
//----------------------------------------------------------------------------
//

#include "GmGenericBiasingPhysics.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4BiasingHelper.hh"
#include "G4BiasingProcessInterface.hh"
#include "GmEWBSSplittingProcess.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(GmGenericBiasingPhysics);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmGenericBiasingPhysics::GmGenericBiasingPhysics(const G4String& name)
:  G4VPhysicsConstructor(name),
  fPhysBiasAllCharged(false),    fNonPhysBiasAllCharged(false),
  fPhysBiasAllChargedISL(false), fNonPhysBiasAllChargedISL(false),
  fPhysBiasAllNeutral(false),    fNonPhysBiasAllNeutral(false),
  fPhysBiasAllNeutralISL(false), fNonPhysBiasAllNeutralISL(false),
  fVerbose(false)
{
  theParamMgr = GmParameterMgr::GetInstance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmGenericBiasingPhysics::~GmGenericBiasingPhysics()
{;}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::PhysicsBias(const G4String& particleName)
{
  fBiasedParticles.push_back(particleName);
  std::vector< G4String > dummy;
  fBiasedProcesses.push_back(dummy);
  fBiasAllProcesses.push_back(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::PhysicsBias(const G4String& particleName, const std::vector< G4String >& processNames)
{
  // check if particle already exists
  size_t ii = 0;
  for( ; ii < fBiasedProcesses.size(); ii++ ) {
    if( particleName == fBiasedParticles[ii] ) {
      break;
    }
  }
  if( ii == fBiasedParticles.size() ) {
    fBiasedParticles.push_back(particleName);
    fBiasedProcesses.push_back(processNames);
    fBiasAllProcesses.push_back(false);
  } else {
    std::vector< G4String > procv = fBiasedProcesses[ii];
    for( size_t jj = 0; jj < processNames.size(); jj++ ) {
      procv.push_back(processNames[jj]);
    }
    std::vector< std::vector< G4String > >::iterator itev = fBiasedProcesses.begin(); itev += ii;
    fBiasedProcesses.erase(itev);
    fBiasedProcesses.insert(itev,procv);
  }
  /*  G4cout << " GmGenericBiasingPhysics::PhysicsBias " <<particleName << " ";
  for( size_t jj = 0; jj < processNames.size(); jj++ ) {
    G4cout << processNames[jj] << " ";
  }
  G4cout << " " << fBiasedParticles.size() << G4endl; //GDEB */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::NonPhysicsBias(const G4String& particleName)
{
  fNonPhysBiasedParticles.push_back(particleName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::Bias(const G4String& particleName)
{
  PhysicsBias(particleName);
  NonPhysicsBias(particleName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::Bias(const G4String& particleName, const std::vector< G4String >& processNames)
{
  PhysicsBias(particleName, processNames);
  NonPhysicsBias(particleName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGenericBiasingPhysics::PhysicsBiasAddPDGRange( G4int PDGlow, G4int PDGhigh, G4bool includeAntiParticle )
{
  if ( PDGlow > PDGhigh ) G4cout << " GmGenericBiasingPhysics::PhysicsBiasAddPDGRange(...) :  PDGlow > PDGhigh, call ignored." << G4endl;
  fPhysBiasByPDGRangeLow .push_back( PDGlow  );
  fPhysBiasByPDGRangeHigh.push_back( PDGhigh );
  if ( includeAntiParticle )
    {
      fPhysBiasByPDGRangeLow .push_back( -PDGhigh );
      fPhysBiasByPDGRangeHigh.push_back( -PDGlow  );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGenericBiasingPhysics::NonPhysicsBiasAddPDGRange( G4int PDGlow, G4int PDGhigh, G4bool includeAntiParticle )
{
  if ( PDGlow > PDGhigh ) G4cout << " GmGenericBiasingPhysics::NonPhysicsBiasAddPDGRange(...) :  PDGlow > PDGhigh, call ignored." << G4endl;
  fNonPhysBiasByPDGRangeLow .push_back( PDGlow  );
  fNonPhysBiasByPDGRangeHigh.push_back( PDGhigh );
  if ( includeAntiParticle )
    {
      fNonPhysBiasByPDGRangeLow .push_back( -PDGhigh );
      fNonPhysBiasByPDGRangeHigh.push_back( -PDGlow  );
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmGenericBiasingPhysics::BiasAddPDGRange( G4int PDGlow, G4int PDGhigh, G4bool includeAntiParticle )
{
  if ( PDGlow > PDGhigh ) G4cout << " GmGenericBiasingPhysics::BiasAddPDGRange(...) :  PDGlow > PDGhigh, call ignored." << G4endl;
  PhysicsBiasAddPDGRange   ( PDGlow, PDGhigh, includeAntiParticle );
  NonPhysicsBiasAddPDGRange( PDGlow, PDGhigh, includeAntiParticle );
}

void GmGenericBiasingPhysics::PhysicsBiasAllCharged( G4bool includeShortLived )
{
  fPhysBiasAllCharged    = true; 
  fPhysBiasAllChargedISL = includeShortLived;
} 
void GmGenericBiasingPhysics::NonPhysicsBiasAllCharged( G4bool includeShortLived )
{ 
  fNonPhysBiasAllCharged    = true; 
  fNonPhysBiasAllChargedISL = includeShortLived;
}
void GmGenericBiasingPhysics::BiasAllCharged( G4bool includeShortLived )
{  
  fPhysBiasAllCharged       = true;
  fNonPhysBiasAllCharged    = true;
  fPhysBiasAllChargedISL    = includeShortLived;
  fNonPhysBiasAllChargedISL = includeShortLived;
}
void GmGenericBiasingPhysics::PhysicsBiasAllNeutral( G4bool includeShortLived )
{ 
  fPhysBiasAllNeutral    = true;  
  fPhysBiasAllNeutralISL = includeShortLived;
} 
void GmGenericBiasingPhysics::NonPhysicsBiasAllNeutral( G4bool includeShortLived )
{
  fNonPhysBiasAllNeutral    = true;
  fNonPhysBiasAllNeutralISL = includeShortLived;
}
void GmGenericBiasingPhysics::BiasAllNeutral( G4bool includeShortLived )
{  
  fPhysBiasAllNeutral       = true;
  fNonPhysBiasAllNeutral    = true;
  fPhysBiasAllNeutralISL    = includeShortLived;
  fNonPhysBiasAllNeutralISL = includeShortLived;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::ConstructParticle()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmGenericBiasingPhysics::ConstructProcess()
{
  DeReferenceWords();
  
  // -- bias setup per individual particle name:
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  
  while( (*particleIterator)() )
    {
      G4ParticleDefinition*     particle = particleIterator->value();
      G4String              particleName = particle->GetParticleName();
      G4ProcessManager*         pmanager = particle->GetProcessManager();
      
      // -- include non physics process interface for biasing:
      if ( std::find(fNonPhysBiasedParticles.begin(),
		     fNonPhysBiasedParticles.end(),
		     particleName             )  != fNonPhysBiasedParticles.end() )
	{
	  G4BiasingHelper::ActivateNonPhysicsBiasing(pmanager);
	}
      
      // -- wrap biased physics processes, all processes or only user selected:
      std::vector< G4String >::const_iterator particleIt =
	std::find(fBiasedParticles.begin(),
		  fBiasedParticles.end(),
		  particleName             );
      if ( particleIt == fBiasedParticles.end() ) continue;
      
      std::vector < G4String >& biasedProcesses = fBiasedProcesses [ particleIt - fBiasedParticles.begin() ];
      G4bool biasAll                            = fBiasAllProcesses[ particleIt - fBiasedParticles.begin() ];
      
      if ( biasAll )
	{
	  G4ProcessVector*  vprocess = pmanager->GetProcessList();
	  for (size_t ip = 0 ; ip < vprocess->size() ; ip++)
	    {
	      G4VProcess* process = (*vprocess)[ip];
	      biasedProcesses.push_back( process->GetProcessName() );
	    }
	}
      
      G4bool restartLoop(true);
      while ( restartLoop )
	{
	  for (std::size_t ip = 0 ; ip < biasedProcesses.size() ; ip++)
	    {
	      G4bool activ = G4BiasingHelper::ActivatePhysicsBiasing(pmanager, biasedProcesses[ip], "" );
	      restartLoop = activ;
	      if ( restartLoop ) break;
	    }
	}
      
    }
  
  
  // -- bias setup per group:
  particleIterator->reset();

  while( (*particleIterator)() )
    {
      G4ParticleDefinition*     particle = particleIterator->value();
      G4String              particleName = particle->GetParticleName();
      G4ProcessManager*         pmanager = particle->GetProcessManager();
      
      // -- exclude particles invidually specified by name:
      if ( std::find( fNonPhysBiasedParticles.begin(),
		      fNonPhysBiasedParticles.end(),
		      particleName                    ) != fNonPhysBiasedParticles.end() ) continue;

      
      if ( std::find( fBiasedParticles.begin(),
		      fBiasedParticles.end(),
		      particleName                    ) != fBiasedParticles.end() ) continue;

      G4bool physBias(false), nonPhysBias(false);
      
      auto PDG = particle->GetPDGEncoding();

      // -- include particle if in right PDG range:
      for ( size_t i = 0 ; i < fPhysBiasByPDGRangeLow.size() ; i++ )
	if ( ( PDG >= fPhysBiasByPDGRangeLow[i] ) && ( PDG <= fPhysBiasByPDGRangeHigh[i] ) )
	  {
	    physBias = true;
	    break;
	  }
      for ( size_t i = 0 ; i < fNonPhysBiasByPDGRangeLow.size() ; i++ )
	if ( ( PDG >= fNonPhysBiasByPDGRangeLow[i] ) && ( PDG <= fNonPhysBiasByPDGRangeHigh[i] ) )
	  {
	    nonPhysBias = true;
	    break;
	  }
      
      // -- if particle has not yet any biasing, include it on charge criteria:
      if ( ( physBias == false ) && ( nonPhysBias == false ) )
	{
	  if ( std::abs( particle->GetPDGCharge() ) > DBL_MIN )
	    {
	      if ( fPhysBiasAllCharged    ) if (    fPhysBiasAllChargedISL || !particle->IsShortLived() )    physBias = true;
	      if ( fNonPhysBiasAllCharged ) if ( fNonPhysBiasAllChargedISL || !particle->IsShortLived() ) nonPhysBias = true;
	    }
	  else
	    {
	      if ( fPhysBiasAllNeutral    ) if (    fPhysBiasAllNeutralISL || !particle->IsShortLived() )    physBias = true;
	      if ( fNonPhysBiasAllNeutral ) if ( fNonPhysBiasAllNeutralISL || !particle->IsShortLived() ) nonPhysBias = true;
	    }
	}
      
      
      if ( nonPhysBias ) G4BiasingHelper::ActivateNonPhysicsBiasing(pmanager);
      
      if ( physBias )
	{
	  std::vector < G4String > biasedProcesses;
	  G4ProcessVector*  vprocess = pmanager->GetProcessList();
	  for (size_t ip = 0 ; ip < vprocess->size() ; ip++)
	    {
	      G4VProcess* process = (*vprocess)[ip];
	      biasedProcesses.push_back( process->GetProcessName() );
	    }
	  
	  G4bool restartLoop(true);
	  while ( restartLoop )
	    {
	      for (std::size_t ip = 0 ; ip < biasedProcesses.size() ; ip++)
		{
		  G4bool activ = G4BiasingHelper::ActivatePhysicsBiasing(pmanager, biasedProcesses[ip], "" );
		  restartLoop = activ;
		  if ( restartLoop ) break;
		}
	    }
	}
      
    }


  
  if ( fVerbose )
    {
      // -- print:
      particleIterator->reset();
      
      while( (*particleIterator)() )
	{
	  G4ParticleDefinition*     particle = particleIterator->value();
	  G4String              particleName = particle->GetParticleName();
	  G4ProcessManager*         pmanager = particle->GetProcessManager();
	  
	  G4bool isBiased(false);
	  G4String processNames;
	  G4int icount(0);
	  
	  G4ProcessVector*  vprocess = pmanager->GetProcessList();
	  for (size_t ip = 0 ; ip < vprocess->size() ; ip++)
	    {
	      G4VProcess* process = (*vprocess)[ip];
	      G4BiasingProcessInterface* pb = dynamic_cast< G4BiasingProcessInterface* >(process);
	      
	      if ( pb != nullptr )
		{
		  isBiased = true;
		  if ( icount < 3 )
		    {
		      processNames += pb->GetProcessName();
		      processNames += " ";
		    }
		  else
		    {
		      processNames += "\n                     ";
		      processNames += pb->GetProcessName();
		      processNames += " ";
		      icount = 0;
		    }
		  icount++;
		}
	    }
	  if ( isBiased )
	    {
	      if ( particle->IsShortLived() )
		G4cout << std::setw(14) << particleName << " **** : " << processNames << G4endl;
	      else
		G4cout << std::setw(18) << particleName << " : " << processNames << G4endl;
	    }
	}
    }

  if( theSplittingProcParams.size() != 0 ) {
    CreateSplittingProcess( theSplittingProcParams );
  }
}

// GAMOS
void GmGenericBiasingPhysics::DeReferenceWords()
{

  auto particleIterator=GetParticleIterator();
  for( size_t ii = 0; ii < fBiasedParticles.size(); ii++ ) {
    std::vector<G4String> derefBPa;
    particleIterator->reset();
    while( (*particleIterator)() )  {
      G4ParticleDefinition*     particle = particleIterator->value();
      G4String              particleName = particle->GetParticleName();
      if( GmGenUtils::AreWordsEquivalent( fBiasedParticles[ii], particleName ) ) {
	derefBPa.push_back( particleName );
      }
    }
    if( derefBPa.size() > 1 ) {
      fBiasedParticles[ii] = derefBPa[0];
      for( size_t jj = 1; jj < derefBPa.size(); jj++ ) {
	fBiasedParticles.push_back(derefBPa[jj]);
	fBiasedProcesses.push_back(fBiasedProcesses[ii]);
	fBiasAllProcesses.push_back(fBiasAllProcesses[ii]);
      }
    } else if (derefBPa.size() == 0 ) {
      G4Exception("GmGenericBiasingPhysics::DeReferenceWords",
		  "GBP001",
		  FatalErrorInArgument,
		  ("Particle type not found " + fBiasedParticles[ii]).c_str());
    }
  }

  for( size_t ii = 0; ii < fBiasedParticles.size(); ii++ ) {
    particleIterator->reset();
    while( (*particleIterator)() )  {
      G4ParticleDefinition*     particle = particleIterator->value();
      G4String              particleName = particle->GetParticleName();      
      if( fBiasedParticles[ii] == particleName ) {
	G4ProcessManager*    pmanager = particle->GetProcessManager();
	G4ProcessVector*  vprocess = pmanager->GetProcessList();
	std::vector<G4String> biasedProcessesOrig = fBiasedProcesses[ii];
	std::vector<G4String> biasedProcessesNew = fBiasedProcesses[ii];
	for( size_t jj = 0; jj < biasedProcessesOrig.size(); jj++ ) {
	  std::vector<G4String> derefBPr;
	  for (size_t ip = 0 ; ip < vprocess->size() ; ip++) {
	    G4VProcess* process = (*vprocess)[ip];
	    G4String processName = process->GetProcessName();
	    if( GmGenUtils::AreWordsEquivalent( biasedProcessesOrig[jj], processName ) ) {
	      derefBPr.push_back(processName);
	    }
	  }
	  if( derefBPr.size() > 1 ) {
	    biasedProcessesNew[jj] = derefBPr[0];
	    for( size_t kk = 1; kk < derefBPr.size(); kk++ ) {
	      biasedProcessesNew.push_back(derefBPr[jj]);
	    }
	    fBiasedProcesses[ii] = biasedProcessesNew;
	  } else if (derefBPr.size() == 0 ) {
	    G4Exception("GmGenericBiasingPhysics::DeReferenceWords",
			"GBP001",
			FatalErrorInArgument,
			("Process not found " + biasedProcessesOrig[jj] + " for particle " + particleName).c_str());
	  }

	}
	
      }
    }
  }
}


//-----------------------------------------------------------------------
void GmGenericBiasingPhysics::AddSplittingProcess( std::vector<G4String> params)
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

  theSplittingProcParams = params;

}

//-----------------------------------------------------------------------
void GmGenericBiasingPhysics::CreateSplittingProcess( std::vector<G4String> params)
{
  GmEWBSSplittingProcess* splitProc = new  GmEWBSSplittingProcess();

  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    G4ProcessManager*         pmanager = particle->GetProcessManager();
    
    for( size_t ii = 0; ii < params.size(); ii++ ){
      if( GmGenUtils::AreWordsEquivalent( params[ii], particleName ) ) {
	pmanager->AddDiscreteProcess( splitProc );
      }
    }
    
  }
  
}
