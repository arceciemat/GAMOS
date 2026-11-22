#ifndef GmUIterminal_h
#define GmUIterminal_h 1

#include "G4UIterminal.hh"
#include "globals.hh"
#include <fstream>

// class description:
//
//  This is a base class of all (G)UI session.
//  SessionStart() method should be called to start the session.
//

class GmUIterminal : public G4UIterminal
{
  // Base class of UI/GUI session
  
public:
  GmUIterminal(G4VUIshell* aShell=0, G4bool qsig=true);
  virtual ~GmUIterminal();
  
  virtual G4int ReceiveG4cout(const G4String& coutString);
  virtual G4int ReceiveG4cerr(const G4String& cerrString);
  // These two methods will be invoked by G4strstreambuf.
  
  void SetCoutFile( const G4String& fname );
  void SetCerrFile( const G4String& fname );
  void SetWriteLog( G4bool bWrite );

private:
  G4String theCoutFileName;
  G4String theCerrFileName;
  std::ofstream* fcout;
  std::ofstream* fcerr;
  G4bool bWriteLogFiles;

};

#endif

