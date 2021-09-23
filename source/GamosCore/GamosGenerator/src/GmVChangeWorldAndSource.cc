#include "GmVChangeWorldAndSource.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
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
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4StateManager.hh"
#include "G4ParticleTable.hh"

#include <iomanip>

//------------------------------------------------------------------
GmVChangeWorldAndSource::GmVChangeWorldAndSource() : GmUserEventAction(), GmUserStackingAction()
{
  bInitialised = FALSE;
  thePrimaryParticleName  = GmParameterMgr::GetInstance()->GetStringValue("GmVChangeWorldAndSource:PrimaryParticleName","");
  if( thePrimaryParticleName == "" ) {
    G4Exception("GmVChangeWorldAndSource::GmVChangeWorldAndSource",
		"Wrong argument",
		FatalErrorInArgument,
		"No primary particle name is defined, use command: /gamos/setParam GmVChangeWorldAndSource:PrimaryParticleName PARTICLE_NAME");
  }
  theSecondaryParticleName  = GmParameterMgr::GetInstance()->GetStringValue("GmVChangeWorldAndSource:SecondaryParticleName","");
  if( theSecondaryParticleName == "" ) {
    G4Exception("GmVChangeWorldAndSource::GmVChangeWorldAndSource",
		"Wrong argument",
		FatalErrorInArgument,
		"No primary particle name is defined, use command: /gamos/setParam GmVChangeWorldAndSource:SecondaryParticleName PARTICLE_NAME");
  }
  theNEventsPerRun = GmParameterMgr::GetInstance()->GetNumericValue("GmVChangeWorldAndSource:nEventsPerRun",100);
  theMinE = GmParameterMgr::GetInstance()->GetNumericValue("GmVChangeWorldAndSource:minE",1.E-12*CLHEP::MeV);
  theMaxE = GmParameterMgr::GetInstance()->GetNumericValue("GmVChangeWorldAndSource:maxE",1.E+2*CLHEP::MeV);
  theNstepsE = (unsigned int)(GmParameterMgr::GetInstance()->GetNumericValue("GmVChangeWorldAndSource:nStepsE",15));
  bLogE = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmVChangeWorldAndSource:logE",1));
  theCurrentWorld = -1;
  theCurrentSource = -1;
  bTooManyEvents = FALSE;
}


//------------------------------------------------------------------
void GmVChangeWorldAndSource::InitialiseRun()
{
    BuildWorlds();
    BuildSources();

    ChangeWorld();
    ChangeSource();

}

//------------------------------------------------------------------
void GmVChangeWorldAndSource::BeginOfEventAction(const G4Event* )
{
  if( bTooManyEvents ) {
    G4double nevtmax = theNEventsPerRun * theWorlds.size() * theSourceCommands.size();
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(warningVerb) ) G4cerr << "GmVChangeWorldAndSource::ChangeWorld()  maximum number of worlds reached and still some events not produced, please diminish the number of events to be at most " << nevtmax << G4endl;
#endif
    G4RunManager::GetRunManager()->AbortRun();
  }
}

//------------------------------------------------------------------
void GmVChangeWorldAndSource::EndOfEventAction(const G4Event* evt)
{
  //---- Check if new detector or source
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmVChangeWorldAndSource::EndOfEventAction evt " << evt->GetEventID() << " theNEventsPerRun " << theNEventsPerRun << G4endl;
#endif
  if( evt->GetEventID() % G4int(theNEventsPerRun) == theNEventsPerRun-1 ) {
    ChangeWorldOrSource();
  }
}

//------------------------------------------------------------------
void GmVChangeWorldAndSource::BuildWorlds()
{
  //----- Create n geometries
  std::vector<G4Material*> mates = GmGeometryUtils::GetInstance()->GetMaterialsUsed();

  std::vector<G4Material*>::const_iterator ite;
  for( ite = mates.begin(); ite != mates.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << "  GmVChangeWorldAndSource::BuildWorlds "
	   << " material " << (*ite)->GetName() 
	   << " radlen " << (*ite)->GetRadlen()
	   << " nucintlen " << (*ite)->GetNuclearInterLength() << G4endl;
#endif
    theWorlds[(*ite)] = BuildWorld( *ite );
    //-   G4cout << "  PV " << BuildWorld( *ite ) << G4endl;
  }

}

//------------------------------------------------------------------
G4PVPlacement* GmVChangeWorldAndSource::BuildWorld(G4Material* mate)
{

  G4double expHall = 1.E9*CLHEP::cm;

  G4Box* experimentalHall_box = new G4Box("expHall",expHall,expHall,expHall);
  G4LogicalVolume* experimentalHall_log = 
    new G4LogicalVolume(experimentalHall_box,
			mate ,"world",0,0,0);

  return  new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,
			    "world",0,false,0);

}


//------------------------------------------------------------------
void GmVChangeWorldAndSource::BuildSources()
{
  
 if( bLogE ) {
    theMinE = log10( theMinE );
    theMaxE = log10( theMaxE );
  }

  G4double stepsE = (theMaxE-theMinE)/(theNstepsE-1);
  for( unsigned int ii = 0; ii < theNstepsE; ii++ ) {
    G4double ener;
    if( bLogE ){
      ener = pow( 10., theMinE + ii*stepsE );
    } else {
      ener = theMinE + ii*stepsE;
    }
    if ( ener <= 0. ) continue;
    G4String stext = thePrimaryParticleName+"_"+GmGenUtils::itoa(ii)+" " + thePrimaryParticleName + " "+GmGenUtils::ftoa(ener)+ "*MeV";
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << ii << "  GmVChangeWorldAndSource::BuildSources  " << stext << G4endl;
#endif
    theSourceCommands[ener] = stext;
  }

}


//------------------------------------------------------------------
void GmVChangeWorldAndSource::ChangeWorldOrSource()
{
  if( theCurrentSource == G4int(theSourceCommands.size()-1) ){ 
    ChangeWorld();
  }

  ChangeSource();
  
}

//------------------------------------------------------------------
void GmVChangeWorldAndSource::ChangeWorld()
{
  theCurrentWorld++;
  if( theCurrentWorld == G4int(theWorlds.size()) ){
    bTooManyEvents = TRUE;
    theCurrentSource = -1;
    return;
  }

  G4RunManager* runMgr = G4RunManager::GetRunManager();
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
  geomMgr->OpenGeometry();

  std::map<G4Material*,G4PVPlacement*>::const_iterator ite = theWorlds.begin();
  for( G4int ii = 0; ii < theCurrentWorld; ii++) ite++;

  G4StateManager::GetStateManager()->SetNewState(G4State_Idle);
  runMgr->DefineWorldVolume( (*ite).second );
  geomMgr->CloseGeometry(true,1,(*ite).second);
  G4RunManagerKernel::GetRunManagerKernel()->UpdateRegion();
  G4StateManager::GetStateManager()->SetNewState(G4State_GeomClosed);

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmVChangeWorldAndSource::ChangeWorld  to " << theCurrentWorld << " = " << (*ite).first << G4endl ;
#endif

  theCurrentSource = -1;
}

//------------------------------------------------------------------
void GmVChangeWorldAndSource::ChangeSource()
{
  theCurrentSource++;

  const G4VUserPrimaryGeneratorAction* gener = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
  const GmGenerator* gmGenerc = dynamic_cast<const GmGenerator*>(gener);
  if( !gmGenerc ) {
    G4Exception("GmVChangeWorldAndSource::ChangeSource",
		"Wrong argument",
		FatalErrorInArgument,
		"Generator is not GmGenerator ");
  }

  GmGenerator* gmGener = const_cast<GmGenerator*>(gmGenerc);

  gmGener->DeleteSources();

  std::map<G4double, G4String>::const_iterator ite = theSourceCommands.begin();
  for( G4int ii = 0; ii < theCurrentSource; ii++) ite++;
  gmGener->AddSingleParticleSource( (*ite).second );

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmVChangeWorldAndSource::ChangeSource  to " << theCurrentSource << " = " << (*ite).second << G4endl ;
#endif
 
}

//------------------------------------------------------------------
G4ClassificationOfNewTrack GmVChangeWorldAndSource::ClassifyNewTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmVChangeWorldAndSource::ClassifyNewTrack new Track " << aTrack->GetDefinition()->GetParticleName() << " from " << aTrack->GetParentID() << " ener " << aTrack->GetKineticEnergy() << " dir " << aTrack->GetMomentumDirection() << G4endl;
#endif
  if( aTrack->GetParentID() == 0 ){
    //  if(aTrack->GetDefinition()->GetParticleName() == thePrimaryParticleName 
    //     && aTrack->GetParentID() == 0 ){
    return fUrgent;
  } else {
    return fKill;
  }
}
