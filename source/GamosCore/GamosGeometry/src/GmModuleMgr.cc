#include "GmModuleMgr.hh"
#include "GmGeomVerbosity.hh"
#include "GmVModule.hh"

GmModuleMgr* GmModuleMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmModuleMgr* GmModuleMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmModuleMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmModuleMgr::GmModuleMgr() 
{
}
 

//-----------------------------------------------------------------------
GmModuleMgr::~GmModuleMgr() 
{
}
 
//------------------------------------------------------------------------
GmVModule* GmModuleMgr::GetModule( G4String name, G4bool bMustExist )
{
  GmVModule* module = 0;
  
  std::map<G4String, GmVModule*>::const_iterator item = theModules.find(name);
  if( item == theModules.end() ) {
    G4ExceptionSeverity es;
    if( bMustExist ) {
      es = FatalException;
    } else {
      es = JustWarning;
    }
    G4Exception("GmModuleMgr::GetModule",
		"",
		es,
		("GAMOS Module does not exist " + name).c_str());    
  } else {
    module = (*item).second;		 
  }

  return module;
}

