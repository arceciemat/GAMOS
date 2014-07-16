#include "GmPDSUA.hh"
#include "GmPDSNeutronProcess.hh"
#include "GmPDSGammaProcess.hh"
#include "GmPDSGeantinoProcess.hh"
#include "GmPDSScore.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
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
  
  bScoreNeutrons = G4bool(paramMgr->GetNumericValue("GmPDS:ScoreNeutrons",1));
  bScoreGammas = G4bool(paramMgr->GetNumericValue("GmPDS:ScoreGammas",1));
  
  if( bScoreNeutrons ) AddNeutronScoringProcess();
  
  if( bScoreGammas ) AddGammaScoringProcess();
  
  std::map<G4int,GmPDSDetector*> detectors = BuildDetectors();
  
  //------ Neutrons
  if( bScoreNeutrons ) {
    theNeutronHelper = new GmPDSProcessHelper( TRUE ); // isFromNeutron = TRUE
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
    theGammaHelper = new GmPDSProcessHelper( FALSE ); // isFromNeutron = FALSE
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
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPDSUA::BuildDetectors = " << detName << G4endl;
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
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSUA::BuildDetectors building detector " << touch->GetLongName() << G4endl;
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
    //  G4Neutron::NeutronDefinition();
    pmanagerN->AddDiscreteProcess( theNeutronScoringProcess );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmPDSUA::AddScoringProcess added NeutronScoringProcess to particle " << particle->GetParticleName() << G4endl;
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
    pmanagerN->AddDiscreteProcess( theGammaScoringProcess );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmPDSUA::AddScoringProcess added GammaScoringProcess to particle " << particle->GetParticleName() << G4endl;
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

}

//------------------------------------------------------------------
void GmPDSUA::EndOfRunAction( const G4Run* )
{

  if( bScoreNeutrons) {
    G4String resultsNeutronFile = GetPDSStringParameter("ResultsFileName","neutron","");
    if( resultsNeutronFile == "" ) {
      theNeutronHelper->PrintResults();
    }else {
      std::ofstream fout(resultsNeutronFile);
      theNeutronHelper->PrintResults(fout);
      fout.close();      
    }
  }

  if( bScoreGammas) {
    G4String resultsGammaFile = GetPDSStringParameter("ResultsFileName","gamma","");
    if( resultsGammaFile == "" ) {
      theGammaHelper->PrintResults();
    }else {
      std::ofstream fout(resultsGammaFile);
      theGammaHelper->PrintResults(fout);
      fout.close();      
    }
  }
  
}
