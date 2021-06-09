#include "GmScoringMgr.hh"
#include "GmVPrimitiveScorer.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosScoring/Management/include/GmPSPrinterMgr.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#include "Reflex/PluginService.h"

GmScoringMgr* GmScoringMgr::theInstance = 0;

//----------------------------------------------------------------------
GmScoringMgr::GmScoringMgr()
{
  bPrintingUA = FALSE;

}

//----------------------------------------------------------------------
GmScoringMgr* GmScoringMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmScoringMgr;
  }

  return theInstance;

}

//----------------------------------------------------------------------
GmScoringMgr::~GmScoringMgr()
{
  std::map<G4String,G4MultiFunctionalDetector*>::iterator item;
  for( item = theMFDs.begin(); item != theMFDs.end(); item++ ){
    delete (*item).second;
  }

  std::map<G4String,GmVPrimitiveScorer*>::iterator ites;
  for( ites = theScorers.begin(); ites != theScorers.end(); ites++ ){
    delete (*ites).second;
  }
  
  std::map<G4String,GmVPSPrinter*>::iterator itep;
  for( itep = thePrinters.begin(); itep != thePrinters.end(); itep++ ){
    delete (*itep).second;
  }
  //  G4cout << " GANOS scoring manager deleting " << G4endl;
 
}

//----------------------------------------------------------------------
void GmScoringMgr::CreateMFD( const std::vector<G4String>& wl )
{
  G4String name = wl[0];
  if( theMFDs.find(name) != theMFDs.end() ) {
    G4Exception("GmScoringMgr::CreateMFD",
		"Warning",
		JustWarning,
		("Repeated declaration of G4MultiFunctionalDetector with name: " + name).c_str() );
  }

  G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector(name);

  theMFDs[name] = det;

  G4SDManager::GetSDMpointer()->AddNewDetector(det);
 

  for( unsigned int ii = 1; ii < wl.size(); ii++ ){
    std::vector<G4LogicalVolume*> lvs = GmGeometryUtils::GetInstance()->GetLogicalVolumes(wl[ii], true); 
    
    std::vector<G4LogicalVolume*>::const_iterator ite;
    for( ite = lvs.begin(); ite != lvs.end(); ite++ ){
      if( (*ite)->GetSensitiveDetector() != 0 ){ 
	G4Exception("GmScoringMgr::CreateMFD",
		    "Volume has already a detector associated",
		    FatalErrorInArgument,
		    G4String("Trying to set detector= "+name+", while it has already detector= "+ (*ite)->GetSensitiveDetector()->GetName()).c_str());
      }

      (*ite)->SetSensitiveDetector(det);
      //    G4cout << " SD set " << (*ite) << " = " << (*itemfd).first << G4endl;
    }
  }
 
}


//----------------------------------------------------------------------
void GmScoringMgr::AddScorer2MFD( const G4String& scorerName, const G4String& scorerClass, const G4String& MFDname, std::vector<G4String>& params )
{
  //----- Check that MFD exists
  std::map<G4String,G4MultiFunctionalDetector*>::iterator itemfd = theMFDs.find(MFDname);
  if( itemfd == theMFDs.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/addScorer2MFD " << scorerName << " " << scorerClass << " " << MFDname << std::endl;
    G4Exception("GmScoringMgr::AddScorer2MFD",
		"Wrong argument",
		FatalErrorInArgument,
		(" Trying to add an scorer to a non-existing multifunctional detector named: " + MFDname).c_str() );
  }

  GmVPrimitiveScorer* scorer = Reflex::PluginService::Create<GmVPrimitiveScorer*>(scorerClass,scorerName);
  if( !scorer ) {
    G4Exception(" GmScoringMgr::AddScorer2MFD",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Scorer class not found " + scorerClass + " Please check documentation and your /gamos/scoring/addScorer2MFD commands").c_str());
  }
  scorer->SetParameters( params );
  if( scorer == 0 ) {
    G4cerr << scorer << "!!! ERROR: at command /gamos/scoring/addScorer2MFD " << scorerClass << " " << scorerName << " " << MFDname << std::endl 
	   << " no scorer exists of type " << scorerClass << " Please check documentation and your /gamos/scoring/addScorer2MFD commands " << G4endl;
  } else {
    ((*itemfd).second)->RegisterPrimitive(scorer);
    scorer->RegisterMFD( ((*itemfd).second) );
    theScorers[scorerName] = scorer;
  }

}

//----------------------------------------------------------------------
void GmScoringMgr::AddFilter2Scorer( std::vector<G4String> params )
{
  G4String scorerName = params[1];

  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/addFilter2Scorer " << params[0] << " " << params[1] << " " << scorerName << std::endl;
      G4Exception("GmScoringMgr::AddFilter2Scorer",
		  "Wrong argument",
		  FatalErrorInArgument,
		  (" Trying to add a filter to a non-existing scorer named: " + scorerName).c_str() );
  }
 
  // Replace scorer name by filter name 
  std::vector<G4String>::iterator ite = params.begin(); 
  ite++;
  (*ite) = params[0];
  GmVFilter* filter = GmFilterMgr::GetInstance()->FindOrBuildFilter(params,1);

  ((*itesco).second)->SetGmFilter(filter);
  
  //  theFilters[params[0]] = filter;

}


//----------------------------------------------------------------------
void GmScoringMgr::AddPrinter2Scorer( std::vector<G4String> params )
{
  G4String scorerName = params[1];

  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/addPrinter2Scorer " << params[1] << " " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddPrinter2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		(" Trying to add a printer to a non-existing scorer named: " + scorerName).c_str() );
  }
  
  // Replace scorer name by printer name 
  std::vector<G4String>::iterator ite = params.begin(); 
  ite++;
  (*ite) = params[0];
  
  GmVPSPrinter* printer = GmPSPrinterMgr::GetInstance()->FindOrBuildPSPrinter(params,1);
  
  ((*itesco).second)->AddPrinter(printer);
 
  printer->SetScoreByEvent( (*itesco).second->ScoreByEvent() );
  
  thePrinters[params[0]] = printer;

}


//----------------------------------------------------------------------
void GmScoringMgr::AssignClassifier2Scorer( std::vector<G4String>& params )   
{
  G4String scorerName = params[1];

  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/addClassifier2Scorer " << params[0] << " " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddClassifier2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to add a classifier to a non-existing scorer named: " + scorerName).c_str() );
  }
  
  // Replace scorer name by classifier name 
  std::vector<G4String>::iterator ite = params.begin(); 
  ite++; 
  (*ite) = params[0];

  GmVClassifier* classifier = GmClassifierMgr::GetInstance()->FindOrBuildClassifier(params,1);

  ((*itesco).second)->SetClassifier(classifier);

  //  theClassifiers[params[0]] = classifier;

  //GmG4PSDoseDeposit uses voxel numbers calculated by G4RegularNavigationHelper, so the classifier should be GmClassifierBy1Ancestor. It should check it 
  /*
  if( static_cast<GmG4PSDoseDeposit*>((*itesco).second) && params[1] != "GmClassifierBy1Ancestor" ){
    G4Exception("GmScoringMgr::AssignClassifier2Scorer",
		"scorer of type GmG4PSDoseDeposit should have as classifier GmClassifierBy1Ancestor, because it uses the voxel numbers calculated by G4RegularNavigationHelper, when SkipEqualMaterials is 1",
		JustWarning,
		G4String("classifier set is: "+params[1]).c_str());
  }
  */

}


//----------------------------------------------------------------------
void GmScoringMgr::AddTrkWeight2Scorer( const G4String& scorerName, G4String& val )   
{
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/useTrackWeight " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddTrkWeight2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to set track weight to a non-existing scorer named: " + scorerName).c_str() );
  }

  G4bool bVal = GmGenUtils::GetBoolean(val);
  ((*itesco).second)->SetUseTrackWeight( bVal );
}

//----------------------------------------------------------------------
void GmScoringMgr::AddScoreErrors2Scorer( const G4String& scorerName, G4String& val )   
{
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/scoreErrors " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddScoreErrors2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to set track weight to a non-existing scorer named: " + scorerName).c_str() );
  }
  
  G4bool bVal = GmGenUtils::GetBoolean(val);
  ((*itesco).second)->SetScoreErrors( bVal );
}
//----------------------------------------------------------------------
void GmScoringMgr::AddPrintByEvent2Printer( const G4String& printerName, G4String& val )   
{
  std::map<G4String,GmVPSPrinter*>::iterator itepr = thePrinters.find(printerName);
  if( itepr == thePrinters.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/printByEvent " << printerName << std::endl;
    G4Exception("GmScoringMgr::AddPrintByEvent2Printer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to set track weight to a non-existing printer named: " + printerName).c_str() );
  }

  G4bool bVal = GmGenUtils::GetBoolean(val);
  ((*itepr).second)->SetScoreByEvent( bVal );
}

//----------------------------------------------------------------------
void GmScoringMgr::ChangeScorerUnit( const G4String& scorerName, const G4String& unitName, G4String& unitValue )
{
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4Exception("GmScoringMgr::ChangeScorerUnit",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(G4String("At command /gamos/scoring/scorerUnit ") + " Trying to set unit to a non-existing scorer named: " + scorerName).c_str() );
  }

  G4double val = GmGenUtils::GetValue(unitValue);
  ((*itesco).second)->SetUnit( unitName, val );

}


//----------------------------------------------------------------------
void GmScoringMgr::PrintAllScorers()
{
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  for(ite = theScorers.begin(); ite != theScorers.end(); ite++ ){
    G4cout << " printing scorer " << (*ite).first << G4endl;
    ((*ite).second)->PrintAll();
  }


}

#include "G4Step.hh"

//----------------------------------------------------------------------
void GmScoringMgr::CountSumsForLastEventInAllScorers()
{
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  for( ite = theScorers.begin(); ite != theScorers.end(); ite++ ){
    (*ite).second->SetNewEvent( TRUE );
    (*ite).second->FillScorer((G4Step*)(0), 0., 1.);
  }
  
}
