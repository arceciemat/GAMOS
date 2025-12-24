#include "GmPSPrinterRBE.hh"
#include "GamosCore/GamosScoring/Scorers/include/GmVPSRBEPhenom.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmPSPrinterMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringRun.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"

#include "G4PhantomParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GamosCore/GamosScoring/Management/include/GmPrimitiveScorerFactory.hh"
#include "GamosCore/GamosScoring/Management/include/GmPSPrinterFactory.hh"
#endif


//-----------------------------------------------------------------------
GmPSPrinterRBE::GmPSPrinterRBE(G4String name) : GmVPSPrinter( name )
{
  theScoringMgr = GmScoringMgr::GetInstance();
}

//-------------------------------------------------------------------
void GmPSPrinterRBE::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 6 && params.size() != 4 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    //                                                  params[0]
    // /gamos/scoring/printer RBEPrinter GmPSPrinterRBE sqdoseRBE GmPSRBEPhenom_McNamara doseScorer LETDuScorer
    G4Exception((thePrinterName+"::SetParameters").c_str(),
		"There should be seven or file parameters: printerName printerType RBEPhenomType doseScorer LETScorer (UNIT UNIT_NAME)",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  //----- Create printer of corresponding type
  G4String printerType = params[0];
  G4String printerName = params[0];
  G4String RBEScorerClass = params[1];
  G4String RBEScorerName = params[1];
  //  G4String printerType = params[1]; // GmPSPrinterBinFile / GmPSPrinterSqdose / ...

  std::vector<G4String> params2;
  params2.push_back(printerName);
  params2.push_back(printerType);
  thePrinter = GmPSPrinterMgr::GetInstance()->FindOrBuildPSPrinter(params2);
  //  G4cout << GetPrinterName() << " THEPRINTER " << thePrinter->GetPrinterName() << G4endl; //GDEB

  /*  std::vector<G4String> prtstr;
  prtstr.push_back("sqdose_"+GetPrinterName());
  //  sqdoseRBE_GmPSRBEPhenom_McNamara
  GmParameterMgr::GetInstance()->AddParamOfDefinedType(printerName+"_"+RBEScorerName+":FileName",prtstr,PTstring);
  G4cout << " AddParamOfDefinedType " << printerName+"_"+RBEScorerName  << "  = " << GetPrinterName() << G4endl; //GDEB
  */
  
  /*#ifdef ROOT5
  thePrinter = Reflex::PluginService::Create<GmVPSPrinter*>(params[0],GetPrinterName());
#else
  thePrinter = GmPSPrinterFactory::get()->create(params[0],GetPrinterName());
#endif
  */
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << thePrinterName + "::SetParameters thePrinter " << thePrinter->GetPrinterName() << " " << params[1] << " " << params[0] << G4endl;
#endif
    
  //----- Create RBE scorer, and associate printer to scorer 
  /*#ifdef ROOT5
  RBEScorerGm = Reflex::PluginService::Create<GmVPrimitiveScorer*>(RBEScorerClass,RBEScorerName);
#else
  RBEScorerGm = GmPrimitiveScorerFactory::get()->create(RBEScorerClass,RBEScorerName);
#endif
    */
  std::vector<G4String> params3;
  theScoringMgr->CreateScorer(RBEScorerName,RBEScorerClass, params3);
  GmVPrimitiveScorer* RBEScorerGm = theScoringMgr->GetScorer(RBEScorerName,true);
  theRBEScorer = dynamic_cast<GmVPSRBEPhenom*>(RBEScorerGm); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << thePrinterName + "::SetParameters  " << RBEScorerClass << " " << params[2] << G4endl;
#endif

  /*  std::vector<G4String> params2;
  params2.push_back(printerName);
  params2.push_back(RBEScorerName);
  theScoringMgr->AddPrinterToScorer(params2);
    */ 
   
  if( params.size() == 6 ) {
    theNewUnit = GmGenUtils::GetValue( params[4] );
    theUnitName = params[5];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << this << thePrinterName + "::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif
  }

  theDoseScorerName = params[2];

  theLETScorerName = params[3];

}


//-----------------------------------------------------------------------
void GmPSPrinterRBE::BeginOfRunAction( const G4Run* )
{
  theDoseScorer = theScoringMgr->GetScorer( theDoseScorerName, true );
  
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << thePrinterName + "::BeginOfRunAction  " << theDoseScorer->GetName() << " " << theDoseScorerName << G4endl;
#endif
    
  theLETScorer = theScoringMgr->GetScorer( theLETScorerName, true );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << thePrinterName + "::BeginOfRunAction " << theLETScorer->GetName() << " " << theLETScorerName << G4endl;
#endif
    
}
  
//-----------------------------------------------------------------------
G4double GmPSPrinterRBE::GetDoseNorm()
{
  const G4VUserPrimaryGeneratorAction* pga = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
  const GmGenerator* pgagm = dynamic_cast<const GmGenerator*>(pga);
  if( ! pgagm ) {
    G4Exception("GmPSPrinterRBE::GetDoseNorm ",
		"",
		FatalException,
		"primary generator is not GmGenerator");
  }
  if( pgagm->GetSources().size() != 0 ){
    G4Exception("GmPSPrinterRBE::GetDoseNorm ",
		"",
		FatalException,
		"There can only be one generator source");
  }
  G4String sourceName = pgagm->GetSources()[0]->GetName();

  // N protons per MU
  G4double totalNProt = 0.;
  G4String nbpFileName = GmParameterMgr::GetInstance()->GetStringValue("PTBeamProtonsPerMU:FileName","BeamNProtonsPerMU.lis");
  GmFileIn fin = GmFileIn::GetInstance(nbpFileName,true);
  std::vector<G4String> wl;
  std::map<G4double,G4double> theNProtPerMU;
  for(;;) {
    if(! fin.GetWordsInLine( wl ) ) break;
    theNProtPerMU[GmGenUtils::GetValue(wl[0])] = GmGenUtils::GetValue(wl[1]);
  }
  fin.Close();

  // Number of beams
  G4String rtip = GmParameterMgr::GetInstance()->GetStringValue(sourceName+":PlanFileNamePrefix","source"); 
  G4int nBeams = -1;
  fin = GmFileIn::GetInstance("RTIonPlan",true);
  for(;;) {
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl[1].find("NumberOfBeams") != std::string::npos ) { 	//  FractionGroup_1_NumberOfBeams
      nBeams = GmGenUtils::GetInt(wl[2]);
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPSPrinterRBE::GetDoseNorm " << " nBeams " << nBeams << G4endl; 
#endif
      break;
    }
  }
  fin.Close();
  // Energy, meterset
  G4String rtipbPrefix = GmParameterMgr::GetInstance()->GetStringValue(sourceName+":BeamFileNamePrefix","source");
  G4String rtipcpPrefix = GmParameterMgr::GetInstance()->GetStringValue(sourceName+":ControlPointFileNamePrefix","source");
  for( G4int ib = 1; ib < nBeams+1; ib++ ) {
    fin = GmFileIn::GetInstance(rtipbPrefix+"_"+GmGenUtils::itoa(ib),true);
    int nCP = -1;
    for(;;) {   
      if(! fin.GetWordsInLine( wl ) ) break;
      if( wl[1] == "NumberOfControlPoints" ) {
	nCP = GmGenUtils::GetValue(wl[2]);
	break;
      }
    }
    fin.Close();
    for( G4int icp = 1; icp < nCP; icp+=2 ) {
      fin = GmFileIn::GetInstance(rtipcpPrefix+"_"+GmGenUtils::itoa(ib)+"_"+GmGenUtils::itoa(icp),true);
      G4double MS = 0.;
      G4double energy = 0.;
      for(;;) {   
	if(! fin.GetWordsInLine( wl ) ) break;
	if( wl[1] == "MetersetWeight") {
	  MS = GmGenUtils::GetValue(wl[2]);
	} else if( wl[1] == "NominalBeamEnergy") {
	  energy = GmGenUtils::GetValue(wl[2]);
	}
      }
      fin.Close();
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) )
	G4cout << "GmPSPrinterRBE::GetDoseNorm " << ib << " " << icp << " MS= " << MS << " energy " << energy << G4endl;
#endif
      if( theNProtPerMU.find(energy) == theNProtPerMU.end() ) {
	for( auto ite = theNProtPerMU.begin(); ite != theNProtPerMU.end(); ite++ ) {
	  G4cerr << " NPROTMU " << ite->first << " " << ite->second << G4endl;
	}
	G4Exception("GmPSPrinterRBE::DumpAll ",
		    "",
		    FatalException,
		    ("Energy not found in file "+nbpFileName).c_str());
      }
      G4double nprotMU = theNProtPerMU[energy];
      totalNProt += nprotMU * MS;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) )
	G4cout << "GmPSPrinterRBE::GetDoseNorm totalNProt " << totalNProt << " nprotMu " << nprotMU << " *MS " << MS << G4endl;
#endif
    }
  }

  // multiply by 1.E12
  totalNProt *= 1.E12;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) )
	G4cout << "GmPSPrinterRBE::GetDoseNorm FINAL " << totalNProt << G4endl;
#endif
  return totalNProt;
  
}
//-----------------------------------------------------------------------
void GmPSPrinterRBE::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  G4double theDoseNorm = GetDoseNorm();
  
  const G4Run* aRun = G4RunManager::GetRunManager()->GetCurrentRun();
  std::map<G4String,GmVPrimitiveScorer*> scorers = theScoringMgr->GetScorers();
  GmScoringRun* scrun = (GmScoringRun*)aRun;
  /*for( auto itep = scorers.begin(); itep != scorers.end(); itep++ ) {
    G4cout << " scorers " << itep->first << " " << itep->second << G4endl;
    } 
     for( G4int ii = 0; ii < scrun->GetNumberOfHitsMap(); ii++ ) {
    G4cout << "GETHITSMAP N= " << scrun->GetHitsMap(ii)->entries() << G4endl;  //GDEB
    } */
  G4THitsMap<G4double>* doseScorerRunMap = 0;
  G4THitsMap<G4double>* LETScorerRunMap = 0;
  for( G4int ii = 0; ii < scrun->GetNumberOfHitsMap(); ii++ ) {
    G4THitsMap<G4double>* scorerRunMap = scrun->GetHitsMap(ii);
    if( scorerRunMap->GetName() == theDoseScorerName ) {
      doseScorerRunMap = scorerRunMap;
    } else if( scorerRunMap->GetName() == theLETScorerName ) {
      LETScorerRunMap = scorerRunMap;
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << this << " " << doseScorerRunMap << " " <<scorers.size() <<  " DOSE RUNMAP N= " << doseScorerRunMap->entries() << G4endl; //GDEB
    for( auto iter = doseScorerRunMap->begin(); iter != doseScorerRunMap->end(); iter++ ) {
      G4cout << thePrinterName << " " << &(iter->second) << " DOSE RUNMAP " << iter->first << " : " << *(iter->second) << G4endl;
    }
    G4cout << LETScorerRunMap << " " <<scorers.size() <<  " LET RUNMAP N= " << LETScorerRunMap->entries() << G4endl; //GDEB
    for( auto iter = LETScorerRunMap->begin(); iter != LETScorerRunMap->end(); iter++ ) {
      G4cout << thePrinterName << " LET RUNMAP " << iter->first << " : " << *(iter->second) << G4endl;
    }
  }
#endif
    
  for( auto iteDose = doseScorerRunMap->begin(); iteDose != doseScorerRunMap->end(); iteDose++ ) {
    G4int index = iteDose->first;
    //    G4cout << thePrinterName << " " << &(iteDose->second) << " DOSELOOP_RUNMAP " << index << " : " << *(iteDose->second) << G4endl; //GDEB
    auto iteLET= LETScorerRunMap->GetMap()->find(index);
    G4double rbeVal = 0.;
    if( iteLET != LETScorerRunMap->end() ) {    
      G4double doseVal = *(iteDose->second);
      // dose must be in Gy and normalized to total treatment
      doseVal *= theDoseNorm;      
      G4double LETVal = *(iteLET->second);
      rbeVal = theRBEScorer->GetRBE(doseVal, LETVal, index );
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) )
	G4cout << index << " " << &(iteDose->second) <<  " RBE VAL " << rbeVal << " " << doseVal << " " << LETVal << G4endl;
#endif
    }
    if( RunMap->GetMap()->find(index) == RunMap->GetMap()->end() ) {
      RunMap->add(index,rbeVal);
    } else {
      RunMap->set(index,rbeVal);
    }
  }

  if( !scorer->ScoreErrors() ) {
    G4Exception("GmPSPrinterRBE::DumpAll ",
		"Scorer is not scoring errors",FatalErrorInArgument,scorer->GetName());
  }

  //--- Build scores from dose and LET using RBE phenomenological formula
  thePrinter->DumpAll( RunMap, theRBEScorer );  

}

