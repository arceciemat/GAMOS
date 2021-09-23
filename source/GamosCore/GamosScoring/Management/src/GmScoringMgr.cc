#include "GmScoringMgr.hh"
#include "GmVPrimitiveScorer.hh"
#include "GmCompoundScorer.hh"
#include "GmScoringRun.hh"
#include "GmScoringVerbosity.hh"

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

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GamosCore/GamosScoring/Management/include/GmPrimitiveScorerFactory.hh"
#endif

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
    //t delete (*ites).second;
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
void GmScoringMgr::CreateScorer( const G4String& scorerName, const G4String& scorerClass, std::vector<G4String>& params )
{
  if( ScoringVerb(debugVerb) ) G4cout << " @@@ CreateScorer " << scorerName << " " << scorerClass << " " << params.size() << G4endl;
  GmVPrimitiveScorer* scorer = 0;
#ifdef ROOT5
  scorer = Reflex::PluginService::Create<GmVPrimitiveScorer*>(scorerClass,scorerName);
#else
  scorer = GmPrimitiveScorerFactory::get()->create(scorerClass,scorerName);
#endif
  
  if( !scorer ) {
    G4Exception(" GmScoringMgr::CreateScorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Scorer class not found " + scorerClass + " Please check documentation and your /gamos/scorer commands").c_str());
  }
  //  if( params.size() != 0 ) scorer->SetParameters( params );
  scorer->SetParameters( params ); // GmPSLETD builds the parameters itself

  theScorers[scorer->GetName()] = scorer;

}

//----------------------------------------------------------------------
void GmScoringMgr::AddScorer2MFD( std::vector<G4String>& wl )
{
  if( ScoringVerb(debugVerb) ) G4cout << " @@@ AddScorer2MFD " << wl[0] << " " << wl[1] << " " << wl.size() << G4endl; 
  G4String scorerName = wl[0];
  G4String scorerClass = "";
  G4String MFDName = "";
  GmVPrimitiveScorer* scorer = 0;
  //----- Check that Scorer exists  
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( ScoringVerb(debugVerb) ) {
    for( std::map<G4String,GmVPrimitiveScorer*>::iterator ite = theScorers.begin(); ite != theScorers.end(); ite++ ) {
      G4cout << " @@@ SCORER " << ite->first << " " << ite->second->GetName() << G4endl; 
    }
  }
  if( itesco != theScorers.end() ) {
    if( wl.size() < 2 ) G4Exception("GmScoringMgr::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("/gamos/scoring/addScorer2MFD  needs only 2 arguments: ScorerName MFDname").c_str()); 
    
    if( wl.size() > 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("/gamos/scoring/addScorer2MFD  needs only 2 arguments: ScorerName MFDname \n . If you need to give parameters to the scorers, use the command  /gamos/scoring/createScorer" ).c_str()); 

    scorer = (*itesco).second;
    MFDName = wl[1];

  //--- Create Scorer
  } else {
    if( wl.size() < 3 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("/gamos/scoring/addScorer2MFD  needs only 3 arguments: ScorerName ScorerClass MFDname ").c_str()); 
    if( wl.size() > 3 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("/gamos/scoring/addScorer2MFD  needs only 3 arguments: ScorerName ScorerClass MFDname \n . If you need to give parameters to the scorers, use the command  /gamos/scoring/createScorer" ).c_str()); 

    scorerClass = wl[1];
    /*    if( scorerClass == "GmCompoundScorer" ) {
    G4Exception("GmScoringMgr::AddScorer2MFD",
		"Wrong argument",
		FatalErrorInArgument,
		(" Trying to add an scorer of type 'GmCompoundScorer', please defined it previously with user command /gamos/scoring/createScorer: " + scorerName).c_str() );
		}*/
 
#ifdef ROOT5
    scorer = Reflex::PluginService::Create<GmVPrimitiveScorer*>(scorerClass,scorerName);
#else
    scorer = GmPrimitiveScorerFactory::get()->create(scorerClass,scorerName);
#endif
    if( !scorer ) {
      G4Exception(" GmScoringMgr::AddScorer2MFD",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Scorer class not found " + scorerClass + " Please check documentation and your /gamos/scoring/addScorer2MFD commands").c_str());
    }
    MFDName = wl[2];
    std::vector<G4String> params;
    scorer->SetParameters( params ); // GmPSLETD builds the parameters itself
  }

  //----- Check that MFD exists
  std::map<G4String,G4MultiFunctionalDetector*>::iterator itemfd = theMFDs.find(MFDName);
  if( itemfd == theMFDs.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/addScorer2MFD " << scorerName << " " << scorerClass
	   << " " << MFDName << std::endl;
    G4Exception("GmScoringMgr::AddScorer2MFD",
		"Wrong argument",
		FatalErrorInArgument,
		(" Trying to add an scorer to a non-existing multifunctional detector named: " + MFDName).c_str() );
  }

  G4MultiFunctionalDetector* mfd = (*itemfd).second;
  mfd->RegisterPrimitive(scorer);
  scorer->RegisterMFD( mfd );
  scorer->PropagateMFDToSubScorers(); 

  theScorers[scorerName] = scorer;

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

  ((*itesco).second)->PropagateFilterToSubScorers(); 

}


//----------------------------------------------------------------------
void GmScoringMgr::AddPrinter2Scorer( std::vector<G4String> params )
{
  G4String scorerName = params[1];

  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/addPrinter2Scorer " << params[0] << " " << scorerName << std::endl;
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
 
  thePrinters[params[0]] = printer;

  //-  ((*itesco).second)->PropagatePrinterToSubScorers(printer); 

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

  ((*itesco).second)->PropagateClassifierToSubScorers(); 

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

  ((*itesco).second)->PropagateTrkWeightToSubScorers(); 
}

//----------------------------------------------------------------------
void GmScoringMgr::AddScoreErrors2Scorer( const G4String& scorerName, G4String& val )   
{
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco;
  for( itesco = theScorers.begin(); itesco != theScorers.end(); itesco++ ) {
    if(  GmGenUtils::AreWordsEquivalent(scorerName, (*itesco).first ) ) break;
  }

  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/scoreErrors " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddScoreErrors2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to set track weight to a non-existing scorer named: " + scorerName).c_str() );
  }
  
  G4bool bVal = GmGenUtils::GetBoolean(val);
  ((*itesco).second)->SetScoreErrors( bVal );

  ((*itesco).second)->PropagateScoreErrorsToSubScorers(); 
}

//----------------------------------------------------------------------
void GmScoringMgr::AddPrintByEvent2Scorer( const G4String& scorerName, G4String& val )   
{

  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco;
  for( itesco = theScorers.begin(); itesco != theScorers.end(); itesco++ ) {
    if(  GmGenUtils::AreWordsEquivalent(scorerName, (*itesco).first ) ) break;
  }

  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/scoreByEvent " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddScoreErrors2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to set track weight to a non-existing scorer named: " + scorerName).c_str() );
  }
  
  G4bool bVal = GmGenUtils::GetBoolean(val);
  ((*itesco).second)->SetScoreByEvent( bVal );

}

//----------------------------------------------------------------------
void GmScoringMgr::AddPrintNEventsType2Scorer( const G4String& scorerName, G4String& val )   
{

  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco;
  for( itesco = theScorers.begin(); itesco != theScorers.end(); itesco++ ) {
    if(  GmGenUtils::AreWordsEquivalent(scorerName, (*itesco).first ) ) break;
  }

  if( itesco == theScorers.end() ) {
    G4cerr << "!!! ERROR: at command /gamos/scoring/scoreNEventsType " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddScoreErrors2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Trying to set track weight to a non-existing scorer named: " + scorerName).c_str() );
  }

  ScoreNEventsType net;
  if( val == "ByRun" ) {
    net = SNET_ByRun;
  } else if( val == "ByEvent" ) {
    net = SNET_ByRun;
  } else if( val == "ByNFilled" ) {
    net = SNET_ByRun;
  } else {
    G4cerr << "!!! ERROR: at command /gamos/scoring/scoreNEventsType " << scorerName << std::endl;
    G4Exception("GmScoringMgr::AddScoreErrors2Scorer",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(" Second argument must be ByRun / ByEvent / ByNFilled, while it is:  " + val).c_str() );
  }

  ((*itesco).second)->SetNEventsType( net );

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

//----------------------------------------------------------------------
G4THitsMap<G4double>* GmScoringMgr::GetRunMap(GmVPrimitiveScorer* scorer )
{
  return theScoringRun->GetRunMap( scorer );
}

//----------------------------------------------------------------------
GmVPrimitiveScorer* GmScoringMgr::GetScore( G4String name, G4bool bMustExist ) const
{
  GmVPrimitiveScorer* scorer = 0;
  
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite = theScorers.find( name );

  if( ite == theScorers.end() ) {
    if( bMustExist ) {
      G4Exception("GmcoringMgr::GetScore",
		  "",
		  FatalException,
		  ("Scorer does not exists " + name).c_str());
    } else {
      G4Exception("GmcoringMgr::GetScore",
		  "",
		  JustWarning,
		  ("Scorer does not exists " + name).c_str());
    }
  } else {
    scorer = (*ite).second;
  }

  return scorer;
}
