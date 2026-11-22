#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AutoLock.hh"
#include "GamosCore/GamosUtils/include/GmMTMergeMgr.hh"
#include "GmPDSUA.hh"
#include "GmPDSNeutronProcess.hh"
#include "GmPDSGammaProcess.hh"
#ifdef PDS_OP
#include "GmPDSOpticalPhotonProcess.hh"
#endif
#include "GmPDSGeantinoProcess.hh"
#include "GmPDSScore.hh"
#ifndef WIN32
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#endif
#include "GmPDSProcessHelper.hh"
#include "GmPDSDetector.hh"
#include "GmPDSUtils.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
#include "G4Geantino.hh"
#include "G4ProcessManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"

//------------------------------------------------------------------
GmPDSUA::GmPDSUA()  : GmUserRunAction(), GmUserEventAction()
{ 
}


//------------------------------------------------------------------
void GmPDSUA::BeginOfRunAction(const G4Run*)
{
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  
  G4String histosFileName = paramMgr->GetStringValue("GmPDS:HistosFileName","pds");
  GmAnalysisMgr* anaMgr = GmAnalysisMgr::GetInstance(histosFileName);
  
  bScoreNeutrons = G4bool(paramMgr->GetNumericValue("GmPDS:ScoreNeutrons",0));
  bScoreGammas = G4bool(paramMgr->GetNumericValue("GmPDS:ScoreGammas",1));
#ifdef PDS_OP
  bScoreOpticalPhotons = G4bool(paramMgr->GetNumericValue("GmPDS:ScoreOpticalPhotons",0));
#endif
  
  if( bScoreNeutrons ) AddNeutronScoringProcess();
  
  if( bScoreGammas ) AddGammaScoringProcess();

#ifdef PDS_OP
  if( bScoreOpticalPhotons ) AddOpticalPhotonScoringProcess();
#endif

  std::map<G4int,GmPDSDetector*> detectors = BuildDetectors();
  
  //------ Neutrons
  if( bScoreNeutrons ) {
    theNeutronHelper = new GmPDSProcessHelper( PDSNeutron ); // isFromNeutron = TRUE
    theNeutronHelper->SetClassifier( theClassifier );
    theNeutronHelper->SetFilters( theFilters );
    theNeutronScoringProcess->AddHelper( theNeutronHelper );
    theNeutronGeantinoScoringProcess->AddHelper( theNeutronHelper );
    
    theNeutronHelper->SetDetectors(detectors);
    
    theScoresN = new std::map<G4String,GmPDSScore*>;
    theNeutronHelper->SetScores( theScoresN );
    theNeutronHelper->InitScores(-1); // Init GmVPDSProcess
    theNeutronHelper->SetAnaMgr(anaMgr);
    
    theNeutronHelper->BookHistos(-1);
  }
  
  //------ Gammas
  if( bScoreGammas ) {
    theGammaHelper = new GmPDSProcessHelper( PDSGamma ); // isFromNeutron = FALSE
    theGammaHelper->SetClassifier( theClassifier );
    theGammaHelper->SetFilters( theFilters );
    theGammaScoringProcess->AddHelper( theGammaHelper );
    theGammaGeantinoScoringProcess->AddHelper( theGammaHelper );
    
    theGammaHelper->SetDetectors(detectors);
    
    theScoresG = new std::map<G4String,GmPDSScore*>;
    theGammaHelper->SetScores( theScoresG );
    theGammaHelper->InitScores(-1); // Init GmVPDSProcess
    
    theGammaHelper->SetAnaMgr(anaMgr);
    
    theGammaHelper->BookHistos(-1);
  }

#ifdef PDS_OP
  //------ OpticalPhotons
  if( bScoreOpticalPhotons ) {
    theOpticalPhotonHelper = new GmPDSProcessHelper( PDSOpticalPhoton ); // isFromOpticalPhoton = TRUE
    theOpticalPhotonHelper->SetClassifier( theClassifier );
    theOpticalPhotonHelper->SetFilters( theFilters );
    theOpticalPhotonScoringProcess->AddHelper( theOpticalPhotonHelper );
    theOpticalPhotonGeantinoScoringProcess->AddHelper( theOpticalPhotonHelper );
    
    theOpticalPhotonHelper->SetDetectors(detectors);
    
    theScoresN = new std::map<G4String,GmPDSScore*>;
    theOpticalPhotonHelper->SetScores( theScoresN );
    theOpticalPhotonHelper->InitScores(-1); // Init GmVPDSProcess
    theOpticalPhotonHelper->SetAnaMgr(anaMgr);
    
    theOpticalPhotonHelper->BookHistos(-1);
  }
#endif
  
  GmPDSScore::SetbHstar( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHstar",1)) );
  GmPDSScore::SetbHp0( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHp0",0)) );
  GmPDSScore::SetbHp15( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHp15",0)) );
  GmPDSScore::SetbHp30( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHp30",0)) );
  GmPDSScore::SetbHp45( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHp45",0)) );
  GmPDSScore::SetbHp60( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHp60",0)) );
  GmPDSScore::SetbHp75( G4bool(paramMgr->GetNumericValue("GmPDS:PrintHp75",0)) );
  
}


//------------------------------------------------------------------
std::map<G4int,GmPDSDetector*> GmPDSUA::BuildDetectors()
{
  std::map<G4int,GmPDSDetector*> detectors;
  
  //-- Set detector from touchable name
  G4String detName = GmParameterMgr::GetInstance()->GetStringValue("GmPDS:DetectorName","");
  
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPDSUA::BuildDetectors = " << detName << G4endl;
#endif
#endif

  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables(detName, 0);
  if(touchs.size() == 0 ){
    GmGeometryUtils::GetInstance()->DumpG4PVLVTree( G4cerr );
    G4Exception("GmPDSUA::BuildDetectors",
                "Wrong argument",
                FatalErrorInArgument,
                G4String("No touchable with name "+detName).c_str());
  }
  
  //--- Check that 
  std::set<G4int> pvCopyNos;
  std::vector<GmTouchable*>::const_iterator ite;
  for( ite = touchs.begin(); ite != touchs.end(); ite++ ){
    GmTouchable* touch = *ite;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSUA::BuildDetectors building detector " << touch->GetLongName() << G4endl;
#endif
#endif
    // PD identification is based on PV copyNo> it cannot be repeated
    if( pvCopyNos.find( touch->GetCopyNo() ) != pvCopyNos.end() ) {
      G4Exception("GmPDSUA::BuildDetectors ",
		  "Error",
		  FatalErrorInArgument,
		  G4String("Repeated physical volume copy number. Copy number = "+GmGenUtils::itoa(touch->GetCopyNo() )).c_str());
    }
    pvCopyNos.insert( touch->GetCopyNo() );
    
    detectors[ touch->GetCopyNo()] = new GmPDSDetector( touch );
    //    G4cout << " DUMP DETECTOR " << *touch << G4endl; //GDEB
    
    delete *ite;
  }
  
  return detectors;
  
}

//------------------------------------------------------------------
void GmPDSUA::AddNeutronScoringProcess()
{
  G4SteppingManager* fpSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
  theNeutronScoringProcess = new GmPDSNeutronProcess("GmPDSNeutronProcess", fpSteppingManager);
  theNeutronScoringProcess->SetFilters( theFilters );

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    if( particle == G4Geantino::Geantino() ) continue;
    G4ProcessManager* pmanagerN = particle->GetProcessManager();
    pmanagerN->AddProcess( theNeutronScoringProcess, 20, -1, 20 );
    //    pmanagerN->SetProcessOrderingToLast(theNeutronScoringProcess,idxPostStep);
    //  pmanagerN->SetProcessOrderingToLast(theNeutronScoringProcess,idxAtRest);
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmPDSUA::AddScoringProcess added NeutronScoringProcess to particle " << particle->GetParticleName() << G4endl;
#endif
#endif
  }

  theNeutronGeantinoScoringProcess = new GmPDSGeantinoProcess("GmPDSGeantinoProcess", fpSteppingManager, G4Neutron::Neutron() );
  theNeutronGeantinoScoringProcess->SetFilters( theFilters );

  G4Geantino::GeantinoDefinition();
  G4ProcessManager* pmanagerG = G4Geantino::Geantino()->GetProcessManager();
  pmanagerG->AddDiscreteProcess( theNeutronGeantinoScoringProcess );

}


//------------------------------------------------------------------
void GmPDSUA::AddGammaScoringProcess()
{
  G4SteppingManager* fpSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
  theGammaScoringProcess = new GmPDSGammaProcess("GmPDSGammaProcess", fpSteppingManager);
  theGammaScoringProcess->SetFilters( theFilters );

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    if( particle == G4Geantino::Geantino() ) continue;
    G4ProcessManager* pmanagerN = particle->GetProcessManager();
    pmanagerN->AddProcess( theGammaScoringProcess, 2, -1, 2 );
    //  pmanagerN->SetProcessOrderingToLast(theGammaScoringProcess,idxPostStep);
    //  pmanagerN->SetProcessOrderingToLast(theGammaScoringProcess,idxAtRest);
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmPDSUA::AddScoringProcess added GammaScoringProcess to particle " << particle->GetParticleName() << G4endl;
#endif
#endif
  }

  theGammaGeantinoScoringProcess = new GmPDSGeantinoProcess("GmPDSGeantinoProcess", fpSteppingManager, G4Gamma::Gamma() );
  theGammaGeantinoScoringProcess->SetFilters( theFilters );
  G4Geantino::GeantinoDefinition();
  G4ProcessManager* pmanagerG = G4Geantino::Geantino()->GetProcessManager();
  pmanagerG->AddDiscreteProcess( theGammaGeantinoScoringProcess );
  /*  G4ProcessVector* pvect = pmanagerG->GetProcessList();
  int jj, sizproc = pvect->size();
  for( jj = 0; jj < sizproc; jj++ ) {
    G4String processName = (*pvect)[jj]->GetProcessName();
    G4cout << "GEANTINO " << processName << " " <<  (*pvect)[jj] << G4endl;
    } */

}

#ifdef PDS_OP
//------------------------------------------------------------------
void GmPDSUA::AddOpticalPhotonScoringProcess()
{
  G4SteppingManager* fpSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
  theOpticalPhotonScoringProcess = new GmPDSOpticalPhotonProcess("GmPDSOpticalPhotonProcess", fpSteppingManager);
  theOpticalPhotonScoringProcess->SetFilters( theFilters );

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* itepar = partTable->GetIterator();
  itepar->reset();
  while( (*itepar)() ){
    G4ParticleDefinition* particle = itepar->value();
    if( particle == G4Geantino::Geantino() ) continue;
    G4ProcessManager* pmanagerN = particle->GetProcessManager();
    pmanagerN->AddProcess( theOpticalPhotonScoringProcess, 2, -1, 2 );
    // pmanagerN->SetProcessOrderingToLast(theOpticalPhotonScoringProcess,idxPostStep);
    // pmanagerN->SetProcessOrderingToLast(theOpticalPhotonScoringProcess,idxAtRest);
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmPDSUA::AddScoringProcess added OpticalPhotonScoringProcess to particle " << particle->GetParticleName() << G4endl;
#endif
#endif
  }

  theOpticalPhotonGeantinoScoringProcess = new GmPDSGeantinoProcess("GmPDSGeantinoProcess", fpSteppingManager, G4OpticalPhoton::OpticalPhoton() );
  theOpticalPhotonGeantinoScoringProcess->SetFilters( theFilters );
  G4Geantino::GeantinoDefinition();
  G4ProcessManager* pmanagerG = G4Geantino::Geantino()->GetProcessManager();
  pmanagerG->AddDiscreteProcess( theOpticalPhotonGeantinoScoringProcess );
  /*  G4ProcessVector* pvect = pmanagerG->GetProcessList();
  int jj, sizproc = pvect->size();
  for( jj = 0; jj < sizproc; jj++ ) {
    G4String processName = (*pvect)[jj]->GetProcessName();
    G4cout << "GEANTINO " << processName << " " <<  (*pvect)[jj] << G4endl;
    } */

}
#endif

//------------------------------------------------------------------
void GmPDSUA::EndOfEventAction(const G4Event*)
{
  //---- Count the change in flux for each energy bin in this event, to calculate the errors
  std::map<G4String,GmPDSScore*>::const_iterator itev;
  if( bScoreNeutrons ) {
    for( itev = theScoresN->begin(); itev != theScoresN->end(); itev++ ) {
      ((*itev).second)->UpdateFluxErrorsThisEvent();
    }
  }
  
  if( bScoreGammas ) {
    for( itev = theScoresG->begin(); itev != theScoresG->end(); itev++ ) {
      ((*itev).second)->UpdateFluxErrorsThisEvent();
    }
  }

#ifdef PDS_OP
  if( bScoreOpticalPhotons ) {
    for( itev = theScoresN->begin(); itev != theScoresN->end(); itev++ ) {
      ((*itev).second)->UpdateFluxErrorsThisEvent();
    }
  }
#endif
  
}

//------------------------------------------------------------------
void GmPDSUA::EndOfRunAction( const G4Run* )
{
  if( GmMTMergeMgr::IsWorkerThread() ) {
    MergeScoresForMaster();
    return;
  }
#ifdef PDS_OP
  PrintAllResults( bScoreNeutrons ? theNeutronHelper : 0, bScoreGammas ? theGammaHelper : 0, bScoreOpticalPhotons ? theOpticalPhotonHelper : 0 );
#else
  PrintAllResults( bScoreNeutrons ? theNeutronHelper : 0, bScoreGammas ? theGammaHelper : 0, 0 );
#endif
}

//------------------------------------------------------------------
void GmPDSUA::PrintAllResults( GmPDSProcessHelper* helperN, GmPDSProcessHelper* helperG, GmPDSProcessHelper* helperOP )
{
  if( helperN ) {
    G4String resultsNeutronFile = GetPDSStringParameter("ResultsFileName","neutron","");
    if( resultsNeutronFile == "" ) {
      helperN->PrintResults();
    }else {
      std::ofstream fout(resultsNeutronFile);
      helperN->PrintResults(fout);
      fout.close();      
    }
  }

  if( helperG ) {
    G4String resultsGammaFile = GetPDSStringParameter("ResultsFileName","gamma","");
    if( resultsGammaFile == "" ) {
      helperG->PrintResults();
    }else {
      std::ofstream fout(resultsGammaFile);
      helperG->PrintResults(fout);
      fout.close();      
    }
  }

  if( helperOP ) {
    G4String resultsOpticalPhotonFile = GetPDSStringParameter("ResultsFileName","opticalphoton","");
    if( resultsOpticalPhotonFile == "" ) {
      helperOP->PrintResults();
    }else {
      std::ofstream fout(resultsOpticalPhotonFile);
      helperOP->PrintResults(fout);
      fout.close();      
    }
  }
}

//------------------------------------------------------------------
namespace {
  G4Mutex pdsMergeMutex = G4MUTEX_INITIALIZER;
  struct PDSMergeData {
    G4int runID = -1;
    GmPDSUA* ua = 0;
    GmPDSProcessHelper* helper[3] = {0,0,0};
    std::map<G4String,GmPDSScore*>* scores[3] = {0,0,0};
  };
  std::map<G4String,PDSMergeData>& PDSMerge() {
    static std::map<G4String,PDSMergeData> theData;
    return theData;
  }
  void AddScores( std::map<G4String,GmPDSScore*>* to, std::map<G4String,GmPDSScore*>* from ) {
    if( !to || !from ) return;
    for( auto ite = from->begin(); ite != from->end(); ite++ ) {
      auto itet = to->find(ite->first);
      if( itet != to->end() ) itet->second->Add( ite->second );
    }
  }
}

//------------------------------------------------------------------
void GmPDSUA::MergeScoresForMaster()
{
  // The scores of the first worker that finishes the run receive the scores of the others;
  // the master prints them, with the helper of that worker (worker threads are idle then)
  G4AutoLock l(&pdsMergeMutex);
  G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
  PDSMergeData& md = PDSMerge()[GetName()];
  GmPDSProcessHelper* helpers[3] = { bScoreNeutrons ? theNeutronHelper : 0, bScoreGammas ? theGammaHelper : 0, 0 };
  std::map<G4String,GmPDSScore*>* scores[3] = { bScoreNeutrons ? theScoresN : 0, bScoreGammas ? theScoresG : 0, 0 };
#ifdef PDS_OP
  helpers[2] = bScoreOpticalPhotons ? theOpticalPhotonHelper : 0;
  scores[2] = bScoreOpticalPhotons ? theScoresN : 0;
#endif
  if( md.runID != runID ) {
    md.runID = runID;
    md.ua = this;
    for( int ii = 0; ii < 3; ii++ ) {
      md.helper[ii] = helpers[ii];
      md.scores[ii] = scores[ii];
    }
    G4String uaName = GetName();
    GmMTMergeMgr::RegisterMasterEndOfRunAction("GmPDSUA_"+uaName, [uaName](){
	PDSMergeData& mdm = PDSMerge()[uaName];
	if( mdm.ua ) mdm.ua->PrintAllResults( mdm.helper[0], mdm.helper[1], mdm.helper[2] );
      } );
  } else {
    for( int ii = 0; ii < 3; ii++ ) {
      AddScores( md.scores[ii], scores[ii] );
    }
  }
}
