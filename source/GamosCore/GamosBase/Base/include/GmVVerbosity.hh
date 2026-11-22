#ifndef GmVerbosity_hh
#define  GmVerbosity_hh
#include "globals.hh"
#include <map>

class GmVVerbosity 
{
public:
  GmVVerbosity();
  virtual ~GmVVerbosity(){};

  virtual void SetFilterLevel( int fl ) = 0;
  virtual int GetFilterLevel() const = 0;

  static void AddManager( G4String name, GmVVerbosity* mgr ){
    theManagers[name] = mgr;}
  static GmVVerbosity* FindManager( G4String name, G4bool bMustExist = FALSE );
  static void SetVerbosityLevel( const G4String& verbName, const G4int verbLevel, G4bool bMustExist = FALSE );

private:
#ifdef WIN32
  #if defined GamosCore_GamosBase_Base_EXPORTS 
    __declspec(dllexport) static std::map<G4String,GmVVerbosity*> theManagers;
  #else 
    __declspec(dllimport) static std::map<G4String,GmVVerbosity*> theManagers;
  #endif
#else
  static std::map<G4String,GmVVerbosity*> theManagers;
#endif
};


#endif
