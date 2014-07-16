#include "GmGenerVerbosity.hh"
#include "GmGeneratorChangeEnergyAndMaterial.hh"
#include "GmParticleSource.hh"
#include "GmGeneratorMessenger.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Material.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4StateManager.hh"
#include "G4ParticleTable.hh"
#include "G4UserRunAction.hh"
#include "G4NistManager.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4TransportationManager.hh"
#include "G4tgrFileIn.hh"
#include "G4ProductionCutsTable.hh"

#include <iomanip>

//------------------------------------------------------------------
GmGeneratorChangeEnergyAndMaterial::GmGeneratorChangeEnergyAndMaterial() : GmGenerator()
{
  bInitialised = false;

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();

  theNEventsPerRun = G4int(parMgr->GetNumericValue("GmGeneratorChangeEnergyAndMaterial:nEventsPerRun",100));

  GetEnergyParameters("GmGeneratorChangeEnergyAndMaterial");

  theCurrentMaterial = -1;
  theCurrentSource = -1;

}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::InitialiseRun()
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();

  if( theSources.size() == 0 ) {
    G4Exception("GmGeneratorChangeEnergyAndMaterial::InitialiseRun",
		"Error",
		FatalException,
		"No active source : select it with command '/gamos/generator/addSingleParticleSource  or  /gamos/generator/addIsotopeSource'");
  }
  if( theSources.size() > 1 ) {
    G4Exception("GmGeneratorChangeEnergyAndMaterial::InitialiseRun",
		"Error",
		FatalException,
		"More than one active source");
  }

  //---- Select logical volumes 
  theLogicalVolumeNames.push_back("*");
  theLogicalVolumeNames = parMgr->GetVStringValue("GmGeneratorChangeEnergyAndMaterial:VolumesToChange",theLogicalVolumeNames);

  //  bBuildSimpleGeometry = G4bool(parMgr->GetNumericValue("GmGeneratorChangeEnergyAndMaterial:BuildSimpleGeometry",false));
  bBuildSimpleGeometry = false; //BuildSimpleGeometry MAKES IT CRASH!

  if( bBuildSimpleGeometry )  { 
    BuildSimpleGeometry();
  } else {
    BuildLogicalVolumeList();
  } 

  //----- Select materials that are going to be tried
  //  bMaterialsFromGeom = G4bool(parMgr->GetNumericValue("GmGeneratorChangeEnergyAndMaterial:UseMaterialFromGeom",true));
  bMaterialsFromGeom = true;
  if( bMaterialsFromGeom ) {
    BuildMaterialsFromGeom();
  } else {
    BuildMaterialsFromNameList();
  }
  std::vector<G4Material*>::const_iterator ite;
  for( ite = theMaterials.begin(); ite != theMaterials.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "  GmGeneratorChangeEnergyAndMaterial BuildMaterials "
	   << " material " << (*ite)->GetName() 
	   << " radlen " << (*ite)->GetRadlen()
	   << " nucintlen " << (*ite)->GetNuclearInterLength() << G4endl;
#endif
  }

  ChangeMaterial(false);
}


//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::BuildMaterialsFromGeom()
{
  //----- Create n geometries
  theMaterials = GmGeometryUtils::GetInstance()->GetMaterialsUsed();
  G4cout << this << " GmGeneratorChangeEnergyAndMaterial::BuildMaterialsFromGe " << theMaterials.size() << G4endl; //GDEB
}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::BuildMaterialsFromNameList()
{
  std::vector<G4String>::const_iterator ite;
  G4Material* mate = 0;
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  theMaterialNames = parMgr->GetVStringValue("GmGeneratorChangeEnergyAndMaterial:Materials",theMaterialNames);

  for( ite = theMaterialNames.begin(); ite != theMaterialNames.end(); ite++) {
    //----- Search in GEANT4 materials built in geometry
    mate = GmGeometryUtils::GetInstance()->GetMaterial( *ite, false );
 
    //----- Search in GEANT4 materials in G4NistMaterial
    if( !mate ) mate = G4NistManager::Instance()->FindOrBuildMaterial( *ite );

    //----- Search in G4tgrMaterial's (read from text file, but not used, so not transformed to GEANT4 materials)
    //-  G4tgrMaterial* FindMaterial(const G4String& name) const;
    if( !mate ) mate = G4tgbMaterialMgr::GetInstance()->FindOrBuildG4Material(*ite, false);

    if( !mate ) G4Exception(" GmGeneratorChangeEnergyAndMaterial::BuildMaterialsFromNameList",
			    "Wrong argument",
			    FatalErrorInArgument,
			    G4String("Material not found " + *ite ).c_str());

    theMaterials.push_back(mate);
  }

   G4cout << this << " GmGeneratorChangeEnergyAndMaterial::BuildMaterialsFromGe " << theMaterials.size() << G4endl; //GDEB

}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::BuildLogicalVolumeList()
{

  if( !bBuildSimpleGeometry ) {
    const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
    std::vector<G4LogicalVolume*>::const_iterator lvcite;
    std::vector<G4String>::const_iterator lvncite;
    for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
      for( lvncite = theLogicalVolumeNames.begin(); lvncite != theLogicalVolumeNames.end(); lvncite++ ) {
	if( GmGenUtils::AreWordsEquivalent(*lvncite, (*lvcite)->GetName() ) ) {
	  theLogicalVolumes.push_back(*lvcite);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorChangeEnergyAndMaterial::BuildLogicalVolumeList add logical volume to change " << (*lvcite)->GetName() << " of material " << (*lvcite)->GetMaterial()->GetName() << G4endl;

#endif
	}
      }
    }
  }
}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::BuildSimpleGeometry()
{
  // GIVES CRASH AT RUNNING !!
  //----- Build simple geometry
  G4double worldDim = 1.E99*CLHEP::cm;
  
  G4Box* world_box = new G4Box("world",worldDim,worldDim,worldDim);
  G4Material* mate = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* world_log = new G4LogicalVolume(world_box, mate ,"world",0,0,0);
  theLogicalVolumes.push_back( world_log );    
  
  G4PVPlacement* worldPhys =  new G4PVPlacement(0,G4ThreeVector(),world_log,
						"1world",0,false,0);
  
  G4RunManager* runMgr = G4RunManager::GetRunManager();
  runMgr->DefineWorldVolume( worldPhys );
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->SetWorldVolume(worldPhys);

}


//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::BuildSources()
{

  for( unsigned int ii = 0; ii < theEnergies.size(); ii++ ) {
    G4double ener = theEnergies[ii];
    G4String stext = thePrimaryParticleName+"_"+GmGenUtils::itoa(ii)+" " + thePrimaryParticleName + " "+GmGenUtils::ftoa(ener)+ "*MeV";
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << ii << "  GmGeneratorChangeEnergyAndMaterial::BuildSources  " << stext << G4endl;
#endif
    theSourceCommands[ener] = stext;
  }

}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::BeginOfEventAction( const G4Event* )
{
  if( !bInitialised ) {
    InitialiseRun();
    bInitialised = true;
  }
}

//------------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::GeneratePrimaries(G4Event* evt)
{
  if( !bInitialised ) {
    GmParticleSource* source = (*(theSources.begin()));
    thePrimaryParticleName = source->GetParticleName();
    
    BuildSources();
  
    ChangeSource();
  } // initialised will end at BeginOfEventAction

  GmParticleSource* source = (*(theSources.begin()));

  std::vector<GmParticleSource*>::const_iterator ite;
  std::set<GmParticleSource*> currentSources;
  std::set<GmParticleSource*>::const_iterator ite2;
  //----- Select smaller time
  std::multimap<G4double, GmParticleSource*> times;

  //---- For the current source set last time to 0. and for the other sources add the time to the last time active 
  source->LastTimeActive(0.);
  G4double tim = source->GenerateTime();
  theLastEventTime += tim;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerator::GeneratePrimaries  event time= " << theLastEventTime << " increase= " << tim << G4endl;
#endif
  
  evt->AddPrimaryVertex( source->GenerateVertex( theLastEventTime ) );
  
  //-  G4cout << " evt " << evt << " " << theLastEventTime << G4endl;

}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::EndOfEventAction( const G4Event* evt )
{
  //---- Check if new detector or source
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorChangeEnergyAndMaterial::EndOfEventAction evt " << evt->GetEventID() << " theNEventsPerRun " << theNEventsPerRun << G4endl;
#endif
  if( evt->GetEventID() % theNEventsPerRun == theNEventsPerRun-1 ) {
    ChangeMaterialOrSource();
  }

}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::ChangeMaterialOrSource()
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGeneratorChangeEnergyAndMaterial::ChangeMaterialOrSource() " << G4endl;
#endif
  // stop run
  G4RunManager* runMgr = G4RunManager::GetRunManager();
  G4UserRunAction* runAction = const_cast<G4UserRunAction*>(runMgr->GetUserRunAction()); 
  const G4Run* run = runMgr->GetCurrentRun(); 

  if( theCurrentSource == G4int(theSourceCommands.size()-1) &&
      theCurrentMaterial+1 == G4int(theMaterials.size()) ){
    theCurrentSource = -1;
    G4int nevtmax = theNEventsPerRun * theMaterials.size() * theSourceCommands.size();
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(warningVerb) ) G4cerr << "GmGeneratorChangeEnergyAndMaterial::ChangeMaterialOrSource()  maximum number of worlds reached and still some events not produced, please diminish the number of events to be at most " << nevtmax << G4endl;
#endif
    runMgr->AbortRun();

    return;
  }
  
  runAction->EndOfRunAction( run );
  runAction->BeginOfRunAction( run );

  if( theCurrentSource == G4int(theSourceCommands.size()-1) ){ 
    ChangeMaterial();
  }

  ChangeSource();
  
}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::ChangeMaterial(G4bool bChangeSource)
{
  theCurrentMaterial++;

  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
  geomMgr->OpenGeometry();

  std::vector<G4Material*>::const_iterator item = theMaterials.begin();
  for( G4int ii = 0; ii < theCurrentMaterial; ii++) item++;

  G4StateManager::GetStateManager()->SetNewState(G4State_Idle);
  for( std::vector<G4LogicalVolume*>::const_iterator itelv = theLogicalVolumes.begin(); itelv != theLogicalVolumes.end(); itelv++ ) {
    (*itelv)->UpdateMaterial( *item );
  }
  //-  runMgr->DefineWorldVolume( (*ite).second );
  //G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume()
  //  geomMgr->CloseGeometry(true,1,world);
  geomMgr->CloseGeometry(true,1);
  G4RunManagerKernel::GetRunManagerKernel()->RunInitialization();
  //  G4RunManagerKernel::GetRunManagerKernel()->UpdateRegion();
  G4StateManager::GetStateManager()->SetNewState(G4State_GeomClosed);
  G4ProductionCutsTable::GetProductionCutsTable()->DumpCouples();

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGeneratorChangeEnergyAndMaterial::ChangeMaterial to " << theCurrentMaterial << " = " << (*item)->GetName() << G4endl ;
#endif

  if(bChangeSource) theCurrentSource = -1;
}

//------------------------------------------------------------------
void GmGeneratorChangeEnergyAndMaterial::ChangeSource()
{
  theCurrentSource++;

  /*  const G4VUserPrimaryGeneratorAction* gener = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
  const GmGenerator* gmGenerc = dynamic_cast<const GmGenerator*>(gener);
  if( !gmGenerc ) {
    G4Exception("GmGeneratorChangeEnergyAndMaterial::ChangeSource  generator is not GmGenerator ");
  }

  GmGenerator* gmGener = const_cast<GmGenerator*>(gmGenerc);
  gmGener->DeleteSources();

  std::map<G4double, G4String>::const_iterator ite = theSourceCommands.begin();
  for( G4int ii = 0; ii < theCurrentSource; ii++) ite++;
  gmGener->AddSingleParticleSource( (*ite).second );
  */

  GmParticleSource* source = (*(theSources.begin()));
  GmVGenerDistTime* distTime = source->GetTimeDistribution();
  GmVGenerDistPosition* distPos = source->GetPositionDistribution();
  GmVGenerDistDirection* distDir = source->GetDirectionDistribution();

  DeleteSources();
  std::map<G4double, G4String>::const_iterator ite = theSourceCommands.begin();
  for( G4int ii = 0; ii < theCurrentSource; ii++) ite++;
  AddSingleParticleSource( (*ite).second );

  source = (*(theSources.begin()));
  source->SetDistributionTime( distTime );
  source->SetDistributionPosition( distPos );
  source->SetDistributionDirection( distDir );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGeneratorChangeEnergyAndMaterial::ChangeSource  to " << theCurrentSource << " = " << (*ite).second << G4endl ;
#endif
 
}

