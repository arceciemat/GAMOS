#include "GmUserLimitsMgr.hh"
#include "GmUserSpecialCuts.hh"
#include "GmStepLimiter.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

GmUserLimitsMgr* GmUserLimitsMgr::theInstance = 0;

GmUserLimitsMgr::GmUserLimitsMgr()
{
  theR2EGamma = 0;
  theR2EElectron = 0;
  theR2EPositron = 0;
}

//----------------------------------------------------------------------
GmUserLimitsMgr* GmUserLimitsMgr::GetInstance()
{
  if( !theInstance ){
    theInstance = new GmUserLimitsMgr();
  } 

  return theInstance;

}

//----------------------------------------------------------------------
GmUserLimitsMgr::~GmUserLimitsMgr()
{
}

//----------------------------------------------------------------------
void GmUserLimitsMgr::InitialiseR2E()
{
  theR2EGamma = new G4RToEConvForGamma();
  theR2EGamma->SetVerboseLevel(0);
  theR2EElectron = new G4RToEConvForElectron();
  theR2EElectron->SetVerboseLevel(0);
  theR2EPositron = new G4RToEConvForPositron();
  theR2EPositron->SetVerboseLevel(0);
}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsAll( std::vector<G4String>& wl )
{
  CreateUserLimits( wl, TRUE, TRUE );

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsMaxStep( std::vector<G4String>& wl )
{
  wl.push_back("DBL_MAX");
  wl.push_back("DBL_MAX");
  wl.push_back("0.");
  wl.push_back("0.");

  CreateUserLimits( wl, TRUE, FALSE );

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsMaxTrkLen( std::vector<G4String>& wl )
{
  G4String wlul = wl[3];
  wl[3] = "DBL_MAX";
  wl.push_back(wlul);
  wl.push_back("DBL_MAX");
  wl.push_back("0.");
  wl.push_back("0.");

  CreateUserLimits( wl, FALSE, TRUE );

}


//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsMaxTOF( std::vector<G4String>& wl )
{
  G4String wlul = wl[3];
  wl[3] = "DBL_MAX";
  wl.push_back("DBL_MAX");
  wl.push_back(wlul);
  wl.push_back("0.");
  wl.push_back("0.");

  CreateUserLimits( wl, FALSE, TRUE );

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsMinEKin( std::vector<G4String>& wl )
{
  G4String wlul = wl[3];
  wl[3] = "DBL_MAX";
  wl.push_back("DBL_MAX");
  wl.push_back("DBL_MAX");
  wl.push_back(wlul);
  wl.push_back("0.");

  CreateUserLimits( wl, FALSE, TRUE );

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsMinEKinByRange( std::vector<G4String>& wl )
{
  if( !theR2EGamma ) InitialiseR2E();

  //----- convert range to energy
  G4String wlul = wl[3];
  wl[3] = "DBL_MAX";
  wl.push_back("DBL_MAX");
  wl.push_back("DBL_MAX");
  wl.push_back(wlul);
  wl.push_back("0.");

  G4double rcut = GmGenUtils::GetValue(wlul);
  G4double ecut;

  G4String ulNameOrig = wl[0];
  std::vector<G4LogicalVolume*> volumeList = GmGeometryUtils::GetInstance()->GetLogicalVolumes( wl[1] );
  std::vector<G4ParticleDefinition*> particleList = GmGetParticleMgr::GetInstance()->GetG4ParticleList( wl[2] );
  for( unsigned int iv = 0; iv < volumeList.size(); iv++ ){
    G4LogicalVolume* lv = volumeList[iv];
    G4Material* mate = lv->GetMaterial();
    for( unsigned int ip = 0; ip < particleList.size(); ip++ ){
      G4ParticleDefinition* part = particleList[ip];
      if( part == G4Gamma::Gamma() ){
	ecut = theR2EGamma->Convert( rcut, mate); 
      } else if( part == G4Electron::Electron() ){ 
	ecut = theR2EElectron->Convert( rcut, mate); 
      } else if( part == G4Positron::Positron() ){
	ecut = theR2EPositron->Convert( rcut, mate); 
      } else {
	continue;
      }
      if( ecut > 1.E9 ) ecut = 1.E9;
      wl[6] = GmGenUtils::ftoa( ecut );
      wl[1] = lv->GetName(); 
      wl[2] = part->GetParticleName();
      wl[0] = ulNameOrig + "_" + wl[1] + "_" + wl[2];
      CreateUserLimits( wl, FALSE, TRUE );
    }
  }

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimitsMinRange( std::vector<G4String>& wl )
{
  G4String wlul = wl[3];
  wl[3] = "DBL_MAX";
  wl.push_back("DBL_MAX");
  wl.push_back("DBL_MAX");
  wl.push_back("0.");
  wl.push_back(wlul);

  CreateUserLimits( wl, FALSE, TRUE );

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::AddLVAndParticle( std::vector<G4String>& wl )
{
  std::map<G4String,G4UserLimits*>::const_iterator ite = theUserLimits.find( wl[0] );
  if( ite == theUserLimits.end() ) {
    G4cerr << "!! GmUserLimitsMgr::AddLV: user limit not found " << wl[0] << G4endl;
    G4cerr << "%%% List of defined user limits " << G4endl;
    for( ite = theUserLimits.begin(); ite != theUserLimits.end(); ite++ ){
      G4cerr << (*ite).first << G4endl;
    }
    G4Exception("GmUserLimitsMgr::AddLVAndParticle",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }

  G4UserLimits* ul = (*ite).second;
  AssociateLVAndParticle( wl[1], wl[2], ul, FALSE);

  //----- Check if G4UserLimits has stepLimiter or specialCuts
  G4bool bStepLimiter = FALSE;
  G4bool bSpecialCuts = FALSE;
  if( ul->GetMaxAllowedStep(G4Track()) != DBL_MAX ) {
    bStepLimiter = TRUE;
  }
  if( ul->GetUserMaxTrackLength(G4Track()) != DBL_MAX || ul->GetUserMaxTime(G4Track()) != DBL_MAX || ul->GetUserMinEkine(G4Track()) != 0. || ul->GetUserMinRange(G4Track()) != 0 ) {
    bSpecialCuts = TRUE;
  }
  AssociateProcesses2Particle( wl[2], bStepLimiter, bSpecialCuts );

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::CreateUserLimits( std::vector<G4String>& wl, G4bool bStepLimiter, G4bool bSpecialCuts )
{
  if( theUserLimits.find( wl[0] ) != theUserLimits.end() ){
    G4Exception("GmUserLimitsMgr::CreateUserLimits","User limits name already used", FatalErrorInArgument,wl[0]);
  }

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << " GmUserLimitsMgr::CreateUserLimits: ";
#endif
  for( unsigned int ii = 0; ii < wl.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(infoVerb) ) G4cout << " " << wl[ii];
#endif
  } 
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << G4endl;
#endif

  //----- Create G4UserLimits
  G4double maxStep = GmGenUtils::GetValue( wl[3] );
  G4double maxTrkLen = GmGenUtils::GetValue( wl[4] );
  G4double maxTOF = GmGenUtils::GetValue( wl[5] );
  G4double minEKin = GmGenUtils::GetValue( wl[6] );
  G4double minRange = GmGenUtils::GetValue( wl[7] );
  G4UserLimits* ul = new G4UserLimits( maxStep, maxTrkLen, maxTOF, minEKin, minRange );
  theUserLimits[ wl[0] ] = ul;

  //----- Associate G4UserLimits to G4LogicalVolume
  AssociateLVAndParticle( wl[1], wl[2], ul, TRUE );

  //----- Associate GmUserSpecialCuts and GmStepLimiter to particles
  AssociateProcesses2Particle( wl[2], bStepLimiter, bSpecialCuts );
}

//----------------------------------------------------------------------
void GmUserLimitsMgr::AssociateLVAndParticle( G4String& lvName, G4String& partName, G4UserLimits* ul, G4bool bCheckRepeat )
{
  std::vector<G4LogicalVolume*> lvs = GmGeometryUtils::GetInstance()->GetLogicalVolumes( lvName );
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "  GmUserLimitsMgr::AssociateLVAndParticle LVname " << lvName << " particleName " << partName << G4endl;
#endif

  for( unsigned int ii = 0; ii < lvs.size(); ii++ ){
    std::map<G4ParticleDefinition*,G4UserLimits*>* partUL;
    mlvpul::iterator ite = theUserLimitsMap.find( lvs[ii] );
    if( ite == theUserLimitsMap.end() ){
      partUL = new std::map<G4ParticleDefinition*,G4UserLimits*>();
      theUserLimitsMap[ lvs[ii] ] = partUL;
    } else {
      partUL = (*ite).second;
    }
   
    std::vector<G4ParticleDefinition*> particleList = GmGetParticleMgr::GetInstance()->GetG4ParticleList( partName );
    for( unsigned int jj = 0; jj < particleList.size(); jj++ ){
      if( bCheckRepeat && partUL->find( particleList[jj] ) != partUL->end() ){
	G4Exception("GmUserLimitsMgr::AssociateLVAndParticle",G4String("Repeated User Limit for volume= " + lvName + " and particle= " + partName).c_str(),JustWarning," Previous User Limit will be overwritten");
      }
      (*partUL)[ particleList[jj] ] = ul;
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "  GmUserLimitsMgr::AssociateLVAndParticle LV " << lvs[ii]->GetName() << " particle " << particleList[jj]->GetParticleName() << G4endl;
#endif
    }

  }
}


//----------------------------------------------------------------------
void GmUserLimitsMgr::AssociateProcesses2Particle( G4String& partName, G4bool bStepLimiter, G4bool bSpecialCuts )
{
  std::vector<G4ParticleDefinition*> partList = GmGetParticleMgr::GetInstance()->GetG4ParticleList( partName );
  if( partList.size() == 0 ) {
    G4Exception("GmUserLimitsMgr::AssociateProcesses2Particle","",FatalErrorInArgument,G4String(" particle does not exist: " + partName).c_str() );
  }

  std::vector<G4ParticleDefinition*>::const_iterator ite;
  for( ite = partList.begin(); ite != partList.end(); ite++ ){
    G4ParticleDefinition* particle = *ite;
    AssociateProcesses2Particle( particle, bStepLimiter, bSpecialCuts );
  }
}

//----------------------------------------------------------------------
void GmUserLimitsMgr::AssociateProcesses2Particle( G4ParticleDefinition* particle, G4bool bStepLimiter, G4bool bSpecialCuts )
{
  G4ProcessManager* pmanager = particle->GetProcessManager();
  G4ProcessVector* pvect = pmanager->GetProcessList();

  if( bStepLimiter ) { 
    G4bool bStepLimiterFound = FALSE;
    for( G4int ii = 0; ii < pvect->size(); ii++ ){
      if( (*pvect)[ii]->GetProcessName() == "GmStepLimiter" ){
	bStepLimiterFound = TRUE;
      }
    }
    if( !bStepLimiterFound ) {
      pmanager->AddProcess(new GmStepLimiter(),-1,-1,1);
    }
  }

  if( bSpecialCuts ) {
    G4bool bSpecialCutsFound = FALSE;
    for( G4int ii = 0; ii < pvect->size(); ii++ ){
      if( (*pvect)[ii]->GetProcessName() == "GmUserSpecialCut" ){
	bSpecialCutsFound = TRUE;
      }
    }
    
    if( !bSpecialCutsFound ){
      pmanager->AddProcess(new GmUserSpecialCuts(),-1,-1,1);
    }
    
  }

}

//----------------------------------------------------------------------
G4UserLimits* GmUserLimitsMgr::GetUserLimits( G4LogicalVolume* lv, G4ParticleDefinition* particle )
{

  std::map<G4ParticleDefinition*,G4UserLimits*>* partUL;
  mlvpul::iterator ite1 = theUserLimitsMap.find( lv );
  if( ite1 == theUserLimitsMap.end() ){
    return 0;
  } else {
    partUL = (*ite1).second;
  }
   
  std::map<G4ParticleDefinition*,G4UserLimits*>::iterator ite2 = partUL->find( particle );
  if( ite2 == partUL->end() ){
    return 0;
  } else {
    return (*ite2).second;
  }

}

//----------------------------------------------------------------------
void GmUserLimitsMgr::PrintUserLimits(std::ostream& out )
{
  std::map<G4ParticleDefinition*,G4UserLimits*>* partUL;
  mlvpul::iterator ite1;
  std::map<G4ParticleDefinition*,G4UserLimits*>::iterator ite2;
  out << "%%%%% USER LIMITS : (for each volume) particle MaxStep MaxTrkLength MaxTOF MinEKin MinRange" << G4endl;
  for( ite1 = theUserLimitsMap.begin(); ite1 != theUserLimitsMap.end(); ite1++ ){
    out << "%%%UL:  VOLUME= " << (*ite1).first->GetName() << G4endl;

    partUL = (*ite1).second;
    for( ite2 = partUL->begin(); ite2 != partUL->end(); ite2++ ){
      G4UserLimits* ul = (*ite2).second;
      out << "%UL: PARTICLE= " << (*ite2).first->GetParticleName() << " " 
	  << ul->GetMaxAllowedStep(G4Track()) << " " 
	  << ul->GetUserMaxTrackLength(G4Track()) << " " 
	  << ul->GetUserMaxTime(G4Track()) << " " 
	  << ul->GetUserMinEkine(G4Track()) << " " 
	  << ul->GetUserMinRange(G4Track()) << G4endl;
    }
  }
      
}
