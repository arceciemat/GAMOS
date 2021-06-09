#include "GmGetParticleMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"
#include "G4ParticleTable.hh"
#include "G4VProcess.hh"
#include "G4RadioactiveDecay.hh"
#include "G4GenericIon.hh"
#include "G4NistManager.hh"
#include "G4ProcessManager.hh"

GmGetParticleMgr* GmGetParticleMgr::theInstance = 0;

//----------------------------------------------------------------------
GmGetParticleMgr::GmGetParticleMgr()
{
}

//----------------------------------------------------------------------
GmGetParticleMgr* GmGetParticleMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmGetParticleMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmGetParticleMgr::~GmGetParticleMgr()
{
}

//------------------------------------------------------------------
std::vector<G4ParticleDefinition*> GmGetParticleMgr::GetG4ParticleList( const G4String& particleName, G4bool bMustExist ) 
{
  std::vector<G4ParticleDefinition*> theParticles;

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    if( GmGenUtils::AreWordsEquivalent(particleName,particle->GetParticleName())
	//----- If particle is not found look if it is a group of particles
	|| (particleName == "lightMeson" && GmG4Utils::IsLightMeson(particle) )
	|| ( particleName == "charmMeson" && GmG4Utils::IsCharmMeson(particle) )
	|| ( particleName == "bottomMeson" && GmG4Utils::IsBottomMeson(particle) )
	|| ( particleName == "meson" && ( GmG4Utils::IsCharmMeson(particle) || GmG4Utils::IsBottomMeson(particle) ) )
	|| ( particleName == "lightBaryon" && GmG4Utils::IsLightBaryon(particle) )
	|| ( particleName == "strangeBaryon" && GmG4Utils::IsStrangeBaryon(particle) )
	|| ( particleName == "charmBaryon" && GmG4Utils::IsCharmBaryon(particle) )
	|| ( particleName == "baryon" && ( (GmG4Utils::IsStrangeBaryon(particle) || GmG4Utils::IsCharmBaryon(particle) ) ) )
	|| ( particleName == "ion" && (GmG4Utils::IsIon(particle) ) )
	|| particleName == "ALL" ) {
      theParticles.push_back( particle );
    }
    //    G4cout << " GetG4ParticleList " << particleName << " =? " << particle->GetParticleName() << G4endl;
  }

  if( theParticles.size() == 0  ) {
    G4Ions* ion = CreateIon(particleName);
    if( ion == 0  && bMustExist ) {
      G4Exception("GmG4Utils::GetG4Particle",
		"ERROR",
		FatalErrorInArgument,
		G4String("particle name not found " + particleName).c_str() );
    } else {
      theParticles.push_back( ion );
    }
  }


  return theParticles;

}

//------------------------------------------------------------------
G4ParticleDefinition* GmGetParticleMgr::GetG4Particle(const G4String& particleName, G4bool bMustExist )
{
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  if( !particle ) {
    particle = CreateIon(particleName);
    if( particle == 0  && bMustExist ) {
      G4Exception("GmG4Utils::GetG4Particle",
		  "ERROR",
		  FatalErrorInArgument,
		  ("particle name not found " + particleName).c_str() );
    } 
  }

  return particle;

}


//------------------------------------------------------------------
G4ParticleDefinition* GmGetParticleMgr::GetG4Particle(const G4VProcess* proc, G4bool bMustExist  )
{
  G4ParticleDefinition* particle = 0;

  //Look for the particle that has this process
  G4VProcess* procNC = const_cast<G4VProcess*>(proc);
  const G4ProcessManager* procMgr = procNC->GetProcessManager();
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  
  while( (*itepar)() ){
    if( itepar->value()->GetProcessManager() == procMgr ) {
      particle = itepar->value();
    }
  }

  if( !particle & bMustExist ) {
    G4Exception("GmG4Utils::GetG4Particle",
		"ERROR",
		FatalErrorInArgument,
		("process not found " + proc->GetProcessName() ).c_str() );
  } 

  return particle;
}

//----------------------------------------------------------------------
G4Ions* GmGetParticleMgr::CreateIon( const G4String& newValues )
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString(newValues);
  if( wl.size() == 1 ) {
    G4Ions* ion = 0;
    G4String partName = wl[0];
    // check if it is an ion 
    std::string::size_type istart = partName.find("[");
    std::string::size_type iend = partName.find("]");
    if( istart  != std::string::npos
	&& iend != std::string::npos ) {
      G4String isot = partName.substr(0,istart);
      unsigned int ii;
      for( ii = 0; ii < isot.length(); ii++ ){
	if( GmGenUtils::IsNumber( isot[ii] ) ) break;
      }
      if( ii != isot.length() ) {
	G4String symb = isot.substr(0,ii);
	G4Element* elem = G4NistManager::Instance()->FindOrBuildElement(symb, false);
	if( elem ) {
	  ion = CreateIon( G4int(elem->GetZ()), atoi( G4String(isot.substr( ii, isot.length() )) ), atof( G4String(partName.substr( istart+1, iend-istart-1 ) ))*CLHEP::keV, 0 );
	}
      }
    } else {
      G4Exception("GmGetParticleMgr::CreateIon",
		  "Trying to create an ion from a wrong particle name",
		  FatalErrorInArgument,
		  G4String("PARTICLE: "+ newValues).c_str());
    }
    return ion;
  }


  if( wl.size() < 2 || wl.size() > 4 ) {
    G4Exception("GmGeneratorMgr::CreateIon","ERROR there must be between 2 and 4: ATOMIC_NUMBER ATOMIC_MASS (ENERGY) (EXCITATION_NUMBER) ",FatalErrorInArgument,newValues);
  }

  G4int atomicNumber;
  G4int atomicMass;
  G4double energy;
  G4int excitNumber;

  atomicNumber = GmGenUtils::GetInteger( wl[0] );
  atomicMass = GmGenUtils::GetInteger( wl[1] );
  if( wl.size() == 2 ) {
    energy = 0.;
    excitNumber = 0;
  } else if( wl.size() == 3 ) {
    energy = GmGenUtils::GetValue( wl[2] );
    excitNumber = 0;
  } else if( wl.size() == 4 ) {
    energy = GmGenUtils::GetValue( wl[2] ); 
    excitNumber = GmGenUtils::GetInteger( wl[3] ); 
  }  

  return CreateIon( atomicNumber, atomicMass, energy, excitNumber );
  
}

//----------------------------------------------------------------------
G4Ions* GmGetParticleMgr::CreateIon( G4int atomicNumber,
				     G4int atomicMass,
				     G4double energy,
				     G4int excitNumber)
{

  //  G4cout << " CreateIon " << atomicNumber << " " << atomicMass << " " << energy << " " <<  excitNumber << G4endl;
  
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("Physics:RadioactiveDecay",1)) ){
    G4RadioactiveDecay* theRadioactiveDecay = new G4RadioactiveDecay();
    
    G4ProcessManager* pmanager = G4GenericIon::GenericIon()->GetProcessManager();
    if( !pmanager ) {
      G4Exception("GmG4Utils::CreateIon",
		  "Error",
		  FatalException,
		  "Ions cannot be created without an adequate physics list, use for example GmEMExtendedPhysics");
    }
    if( !GmG4Utils::CheckProcessExists( pmanager, "RadioactiveDecay", 0 )) {
      pmanager->AddProcess(theRadioactiveDecay,0,-1,1);
    }  
  }

#ifdef GAMOS_NO_VERBOSE
  // look if G4RadioactiveProcess exists 
  if( BaseVerb(debugVerb) ) G4cout << " add RadioactiveDecay " << G4GenericIon::GenericIon()->GetParticleName() << " pmanager " << pmanager <<  G4endl;
#endif
  
 
  //  G4IonTable* iontab = new G4IonTable();
  G4IonTable * iontab=
    (G4IonTable *)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4ParticleDefinition* partnew = 0;
  partnew = iontab->GetIon(atomicNumber,atomicMass,energy,excitNumber);
  
  G4Ions* ion = dynamic_cast<G4Ions*>(partnew);
  //  G4cout << " CreateIon " << ion->GetParticleName() << " atomic number= " << ion->GetAtomicNumber() << " atomic mass= " << ion->GetAtomicMass() << " excitation energy= " << ion->GetExcitationEnergy()/keV << " spin= " << ion->GetPDGiSpin() << G4endl;
  
  return ion;
    
}

//------------------------------------------------------------------
std::vector<G4VProcess*> GmGetParticleMgr::GetG4ProcessList(const G4String& particleName, const G4String& processName, G4bool bMustExist  )
{
  std::vector<G4VProcess*> procV;
  std::vector<G4ParticleDefinition*> particles = GetG4ParticleList( particleName );
  for( unsigned int ii = 0; ii < particles.size(); ii++ ){
    G4ProcessManager* procMgr = particles[ii]->GetProcessManager();
    G4ProcessVector* procList = procMgr->GetProcessList();
    for( int jj = 0; jj < procList->size(); jj++ ){
      if( GmGenUtils::AreWordsEquivalent(processName,(*procList)[jj]->GetProcessName()) ) {
	procV.push_back((*procList)[jj]);
      }
    }
  }

  if( procV.size() == 0 && bMustExist ) {
    G4Exception("GmG4Utils::GetG4ProcessList",
		"ERROR",
		FatalErrorInArgument,
		("process name not found " + processName + " for particle " + particleName ).c_str() );
  } 

  return procV;
}


//------------------------------------------------------------------
std::vector<G4VProcess*> GmGetParticleMgr::GetG4ProcessList(const G4String& processName, G4bool bMustExist )
{
  std::vector<G4VProcess*> procV;

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    G4ProcessManager* procMgr = particle->GetProcessManager();
    G4ProcessVector* procList = procMgr->GetProcessList();
    for( int jj = 0; jj < procList->size(); jj++ ){
      if( GmGenUtils::AreWordsEquivalent(processName,(*procList)[jj]->GetProcessName()) ) {
	G4cout << particle->GetParticleName() << " GetG4Process " << processName << " " << (*procList)[jj]->GetProcessName() << " " << (*procList)[jj] << G4endl;
	procV.push_back((*procList)[jj]);
      }
    }
  }

  if( procV.size() == 0 && bMustExist ) {
    G4Exception("GmG4Utils::GetG4ProcessList",
		"ERROR",
		FatalErrorInArgument,
		("process name not found " + processName + " for any particle ").c_str() );
  } 

  return procV;
}


//------------------------------------------------------------------
G4String GmGetParticleMgr::GetG4ProcessName(const G4String& particleName, G4String processName )
{
  G4ProcessManager* pmanager = GetG4Particle(particleName)->GetProcessManager();
  G4ProcessVector* pvect = pmanager->GetProcessList();
  int jj, sizproc = pvect->size();

  /*  if (processName=="annih") {
	for( jj = 0; jj < sizproc; jj++ ) {
	  G4String char * cProcessName = const_cast<char*>( (*pvect)[jj]->GetProcessName().c_str() );
	  //G4cout << " looking for process " << processName << " in " << cProcessName << G4endl;
	  if( G4String(cProcessName).find("annih") != G4String::npos ){
		//G4cout << " ProcessName: " << (*pvect)[jj]->GetProcessName() << G4endl;
		return (*pvect)[jj]->GetProcessName();
	  }
	}
	}*/

  for( jj = 0; jj < sizproc; jj++ ) {
    G4String processName2 = (*pvect)[jj]->GetProcessName();
    std::string::iterator ite= processName2.begin();
    *ite = toupper(*ite); 
    ite = processName.begin();
    *ite = toupper(*ite); 
    //    G4cout << " looking for process " << processName << " in " << processName << G4endl;
    if( processName2.find(processName) != G4String::npos ){
      //      G4cout << " ProcessName: " << (*pvect)[jj]->GetProcessName() << G4endl;
      return (*pvect)[jj]->GetProcessName();
    }
  }

  //PenCompton compt LowEnCompton
  //PenPhotoElec phot LowEnPhotoElec
  //PenRayleigh   --- LowEnRayleigh

  //PenelopeIoni LowEnergyIoni eIoni
  //PenelopeBrem LowEnBrem eBrem

  //PenAnnih annihil annihil

  //  G4Exception("GmG4Utils::GetG4ProcessName","WARNING",JustWarning,("process name not found " + processName + " for particle " + particleName ).c_str() );

  return "";
}
