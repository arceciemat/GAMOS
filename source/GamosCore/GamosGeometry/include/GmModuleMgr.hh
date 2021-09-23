#ifndef GmModuleMgr_H
#define GmModuleMgr_H

class GmVModule;
#include <map>
#include "globals.hh"

class GmModuleMgr 
{
 public:
  static GmModuleMgr* GetInstance();

  ~GmModuleMgr();

private:
  GmModuleMgr();

public:
  void RegisterModule(GmVModule* mod, G4String name ) {
    theModules[name] = mod;
  }
  GmVModule* GetModule( G4String name, G4bool bMustExist = true );

private:
  static GmModuleMgr* theInstance;

  std::map<G4String, GmVModule*> theModules;
};

#endif
