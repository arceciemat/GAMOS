#include "GmVVerbosity.hh"
#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmVerbosityFactory.hh"
#endif
/** Constructor   
 */
std::map<G4String,GmVVerbosity*>  GmVVerbosity::theManagers;

GmVVerbosity::GmVVerbosity(){
}


GmVVerbosity* GmVVerbosity::FindManager(  G4String name, G4bool bMustExist )
{
  static std::map<G4String,GmVVerbosity*>::const_iterator ite = theManagers.find(name);
  if( ite == theManagers.end() ){
    if( bMustExist ) {
      for( ite = theManagers.begin(); ite != theManagers.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
	//      G4cerr << "VerbosityMgr: " << (*ite).first << G4endl;
#endif
      }
      G4Exception(" GmVVerbosity::FindManager ",   
		  " Manager name not found",
		  FatalErrorInArgument,
		  name);
    } else {
      return 0;
    }
  }

  return (*ite).second;
}

void GmVVerbosity::SetVerbosityLevel( const G4String& verbName, const G4int verbLevel, G4bool bMustExist )
{

  GmVVerbosity* verbMgr = GmVVerbosity::FindManager( verbName, bMustExist );
  if( !verbMgr ) {
#ifdef ROOT5
    verbMgr = Reflex::PluginService::Create<GmVVerbosity*>(verbName);
#else
    verbMgr = GmVerbosityFactory::get()->create(verbName);
#endif
    GmVVerbosity::AddManager( verbName, verbMgr );
  }
  if( verbMgr == 0 ) {
    G4Exception("GmRunManager::SelectVerbosity",
		"verbosity does not exists ",
		FatalErrorInArgument,
		verbName);
  }
   
  verbMgr->SetFilterLevel( verbLevel );
  
}
