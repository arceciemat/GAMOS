#include <math.h>

#include "GmPDSCreateAngleTablesUA.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Material.hh"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTable.hh"
#include "G4TransportationManager.hh"
#include <iomanip>

//------------------------------------------------------------------
GmPDSCreateAngleTablesUA::GmPDSCreateAngleTablesUA() : GmUserRunAction(), GmUserSteppingAction()
{
  theLastMaterial = 0;
  theLastEnergy = -1.;
  bInitialised = false;
}


//------------------------------------------------------------------
void GmPDSCreateAngleTablesUA::BeginOfRunAction(const G4Run*)
{
  G4String histoFileName = GmParameterMgr::GetInstance()->GetStringValue(theName+":HistoFileName","angleDeviation");
  theAnaMgr = GmAnalysisMgr::GetInstance(histoFileName) ;  
}


//------------------------------------------------------------------
void GmPDSCreateAngleTablesUA::BeginOfEventAction( const G4Event* evt )
{
  if( !bInitialised ) Initialise(evt);

  G4PrimaryParticle* primary = evt->GetPrimaryVertex(0)->GetPrimary(0);
  G4double energy = GmGenUtils::GetKineticEnergy( primary->GetMass(), primary->GetMomentum().mag() );
  G4LogicalVolume* topLV = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume();
  G4Material* mate = topLV->GetMaterial();
  if( mate != theLastMaterial ) {
    // check if this material has already been simulated
    std::map<G4Material*,G4int>::const_iterator item = theMaterialIDs.find(mate);
    if( item == theMaterialIDs.end() ) {
      theCurrentMateID = theMaterialIDs.size();
      theMaterialIDs[mate] = theCurrentMateID;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSCreateAngleTablesUA::BeginOfEventAction new material " << mate->GetName() << " N mates = " << theMaterialIDs.size() << " theCurrentMateID " << theCurrentMateID << G4endl;
#endif
    } else {
      theCurrentMateID = (*item).second;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSCreateAngleTablesUA::BeginOfEventAction old material theCurrentMateID " << theCurrentMateID << G4endl;
#endif
    }
  }

  G4String energyName ="";
  if( energy != theLastEnergy ) {
    // check if this energy has already been simulated

    //problems of precision    std::map<G4double,G4int>::const_iterator itee = theEnergyIDs.find(energy);
    std::map<G4double,G4int>::const_iterator itee;
    for( itee = theEnergyIDs.begin(); itee != theEnergyIDs.end(); itee++ ) {
      if( fabs((*itee).first - energy)/energy < 0.00001 ) break;
    }
    if( itee == theEnergyIDs.end() ) {
      theCurrentEnergyID = theEnergyIDs.size();
      theEnergyIDs[energy] = theCurrentEnergyID;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSCreateAngleTablesUA::BeginOfEventAction new energy " << energy << " N energies = " << theEnergyIDs.size() << " theCurrentEnergyID " << theCurrentEnergyID << G4endl;
#endif
    } else {
      theCurrentEnergyID = (*itee).second;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSCreateAngleTablesUA::BeginOfEventAction old energy " << energy << " theCurrentEnergyID " << theCurrentEnergyID << G4endl;
#endif
    }
    G4double val = energy*1.000001;
    G4int exp = G4int(log10(val));
    if( exp < 0 ) exp -= 1;
    //    G4cout << energy << "BOOK val " << val << " exp " << exp << G4endl;
    G4int vali = G4int( val*pow(10.,-exp+4) );
    val = vali/1E4;
    energyName = GmGenUtils::ftoa(val) + "E" + GmGenUtils::ftoa(exp);
    //    G4cout << " energyName " << energyName << " BOOK " << val << " " << vali << G4endl
  }
    
  //  G4cout << " BOOKHISTOS " << energyName << " " << mate->GetName() << G4endl;
  if( energyName != "" && theHistos.find(energyName+mate->GetName()) == theHistos.end() ) {
      //  if( mate != theLastMaterial || energy != theLastEnergy ) {
    BookHistos( energyName, mate->GetName() );
  }

  theLastEnergy = energy;
  theLastMaterial = mate;
}


//------------------------------------------------------------------
void GmPDSCreateAngleTablesUA::Initialise( const G4Event* evt )
{
  thePrimaryParticleName = evt->GetPrimaryVertex(0)->GetPrimary(0)->GetG4code()->GetParticleName();

  theSecondaryParticleName = GmParameterMgr::GetInstance()->GetStringValue("GmPDSCreateAngleTablesUA:SecondaryParticleName",thePrimaryParticleName);

  //-- Get process manager of particle
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  G4ProcessManager* pmanager = 0;
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    if( particle->GetParticleName() == thePrimaryParticleName ){
#ifndef GAMOS_NO_VERBOSE
     if( ScoringVerb(debugVerb) ) G4cout << "GmPDSCreateAngleTablesUA:  PARTICLE is " << particle->GetParticleName() << G4endl;
#endif
      pmanager = particle->GetProcessManager();
      break;
    }
  }

  theProclis = pmanager->GetProcessList();

  bInitialised = true;
}


//------------------------------------------------------------------
void GmPDSCreateAngleTablesUA::BookHistos(G4String energyName, G4String mateName )
{
  theHistos.insert(energyName+mateName);

#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPDSCreateAngleTablesUA::BookHistos " << energyName << " " << mateName << G4endl;
#endif
  std::string hnam,hnam0,hnam1;
  std::string procName;
  for( size_t nn = 0; nn < theProclis->entries(); nn++ ){
    procName = (*theProclis)[nn]->GetProcessName();
    theProcesses[procName] = nn;
    if( procName == "Transportation" || procName == "Decay" ) continue;    

    hnam0 = thePrimaryParticleName + " - " + theSecondaryParticleName + " : " + procName + std::string(" : ") + mateName + std::string(" : ");

    //	G4cout << " val " << val << " GmGenUtils::ftoa(val) " << GmGenUtils::ftoa(val) << " exp " << exp << " ostn " << ostn << G4endl;
    
    hnam1 = hnam0 + std::string(energyName) + std::string(" : ");
    hnam = hnam1 + std::string("cos Deviation Angle");
    theAnaMgr->CreateHisto1D(hnam,200,-1.0,1.0,5000000+nn*100000+theCurrentEnergyID*1000+theCurrentMateID*10+1);
    hnam = hnam1 + std::string("log10 Interaction Distance (cm)");
    theAnaMgr->CreateHisto1D(hnam,220,-5,6.,5000000+nn*100000+theCurrentEnergyID*1000+theCurrentMateID*10+2);	
  }

}

//------------------------------------------------------------------
void GmPDSCreateAngleTablesUA::UserSteppingAction(const G4Step* aStep)
{ 
  //----- Only for selected particle
  const G4Track* aTrack = aStep->GetTrack();
  G4Track* aTracknc = const_cast<G4Track*>(aTrack);
  //  if( aTrack->GetDefinition()->GetParticleName() != theParticleName ) return;

  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  //-- Check if process is not transportation
  if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL && 
     procName != "Transportation" ){
    //----- Get secondary if it is selected particle name, else get the current track 
    G4Track* seco = GetSecondaryTrackSameType();
    if( seco == 0 ) {
      //----- Get current track unless it is stopped (case of inelastic)
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout  << " GmPDSCreateAngleTablesUA::UserSteppingAction track status when no good seco " << aTrack->GetTrackStatus() << G4endl;
#endif
      //      G4cout << " old mom " << aTracknc->GetStep()->GetPreStepPoint()->GetMomentumDirection() << " new mom " << aTracknc->GetStep()->GetPostStepPoint()->GetMomentumDirection() << G4endl;

      //----- Check if it is non-interesting step:
      //- Inelastic with no neutron : fStopAndKill
      //- Capture : fStopAndKill
      //- Transportation
      //--- If process is inelastic, track will be put in state fStopButAlive (or fStopAndKill?)
      if( aTrack->GetTrackStatus() == fStopAndKill || aTrack->GetTrackStatus() == fStopButAlive || aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation" ) {   // non interesting step
	return;
      //- LElastic : fSuspend, hElastic: fAlive, but both cases momentum changes direction  
      } else if( aTrack->GetDefinition()->GetParticleName() == theSecondaryParticleName ) {
	if( aTracknc->GetStep()->GetPreStepPoint()->GetMomentumDirection() != aTracknc->GetStep()->GetPostStepPoint()->GetMomentumDirection() ) {
	  seco = aTracknc; // track has already changed direction (neutron: in LElastic this track is suspended and a new track with same direction is created, in hElastic, track continues, but in both cases it has changed direction 
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSCreateAngleTablesUA::UserSteppingAction  change of direction " << G4endl;
#endif
	} else {
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(warningVerb) ) G4cerr << " GmPDSCreateAngleTablesUA::UserSteppingAction no seco and no change direction!!?!?  PROCESS= " << aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << " momPre " << aTracknc->GetStep()->GetPreStepPoint()->GetMomentumDirection() << " momPost " << aTracknc->GetStep()->GetPostStepPoint()->GetMomentumDirection() << G4endl;
#endif
	  seco = aTracknc; 
	}
      }
    }

    if( seco != 0 ) {
#ifndef GAMOS_NO_VERBOSE
      //  if( procName != "LElastic" &&  procName != "hElastic" && procName != "inelastic" && procName != "NeutronInelastic" ) if( ScoringVerb(debugVerb) ) G4cout << " neutron from process " << procName << G4endl;
#endif
      //-      G4double cosAngle = (aStep->GetPreStepPoint()->GetMomentumDirection() * seco->GetMomentumDirection() );
      // for charged particles, msc is applied at AlongStepDoIt changing the direction and secondary emission at process PostStepDoIt, using the direction at post step
      //t and for others? 
      //t protonInelastic stop the particle and postStep direction does not change, and eBrems??
      G4double cosAngle;
      if( seco != aTracknc ) {
	cosAngle = (aStep->GetPostStepPoint()->GetMomentumDirection() * seco->GetMomentumDirection() );
      } else {
	cosAngle = (aStep->GetPreStepPoint()->GetMomentumDirection() * seco->GetMomentumDirection() );
      }

      G4double ener = aStep->GetPreStepPoint()->GetKineticEnergy();
      G4double dist = aStep->GetStepLength()/CLHEP::cm;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " direction change " << cosAngle << " distance " << dist << " in material " << aStep->GetTrack()->GetMaterial()->GetName() << " energy " << ener << G4endl;
#endif
      std::map<G4String,int>::iterator itep = theProcesses.find( procName );
      if( itep == theProcesses.end() ){
	G4Exception("GmPDSCreateAngleTablesUA::UserSteppingAction",
		    "Process not defined: particle is not the same as the primary one!",
		    JustWarning,
		    "Use /gamos/userAction GmKillAtStackingActionUA GmSecondaryFilter");
	return;
      }
      G4int nproc = (*itep).second;

      G4int nh = 5000000+nproc*100000+theCurrentEnergyID*1000+theCurrentMateID*10;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " filling histo " << nh << " " << theAnaMgr->GetHisto1(nh+1)->GetName() << G4endl;
#endif

      theAnaMgr->GetHisto1(nh+1)->Fill( cosAngle );
      theAnaMgr->GetHisto1(nh+2)->Fill( log10(dist) );
      
      aTracknc->SetTrackStatus( fStopAndKill);
      
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPDSCreateAngleTablesUA::UserSteppingAction no seco produced " << G4endl;
#endif
    }
  } else {
    return;
  }
   
  if(  aStep->GetPreStepPoint()->GetKineticEnergy() !=  aStep->GetPostStepPoint()->GetKineticEnergy() )  {
    aTracknc->SetTrackStatus( fStopAndKill);
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cerr << " No different energy and no Transportation " << aStep->GetPostStepPoint()->GetProcessDefinedStep() << G4endl;
#endif
  }      

}


//------------------------------------------------------------------
G4Track* GmPDSCreateAngleTablesUA::GetSecondaryTrackSameType()
{
  G4Track* seco = 0;
  const G4TrackVector* secoList = fpSteppingManager->GetSecondary(); // get all track secondaries, as tracks are killed after first step 
  G4TrackVector::const_iterator ite;
  for( ite = secoList->begin(); ite != secoList->end(); ite++ ){
    if( (*ite)->GetDefinition()->GetParticleName() == theSecondaryParticleName ){
      seco = *ite;
      break;
    }
  }

  return seco;

}


//------------------------------------------------------------------
void GmPDSCreateAngleTablesUA::EndOfRunAction(const G4Run*)
{
}
