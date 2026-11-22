#include "GmUIterminal.hh"
#include "GmUIMessenger.hh"


//-------------------------------------------------------------------------
GmUIterminal::GmUIterminal(G4VUIshell* aShell, G4bool qsig) 
  : G4UIterminal( aShell, qsig )
{
  fcout = 0;
  fcerr = 0;
  new GmUIMessenger(this);
  bWriteLogFiles = true;
}


//-------------------------------------------------------------------------
GmUIterminal::~GmUIterminal() 
{
  if(fcout) delete fcout;
  if(fcerr) delete fcerr;
}


//-------------------------------------------------------------------------
G4int GmUIterminal::ReceiveG4cout(const G4String& coutString)
{
  if( fcout && bWriteLogFiles ) {
    *fcout <<  coutString << std::flush; 
  }
  std::cout <<  coutString << std::flush;
  return 0;
}


//-------------------------------------------------------------------------
G4int GmUIterminal::ReceiveG4cerr(const G4String& cerrString)
{
  if( fcerr && bWriteLogFiles ) {
    *fcerr <<  cerrString << std::flush; 
  }
  std::cerr <<  cerrString << std::flush;
  return 0;
}                                                                       


//-------------------------------------------------------------------------
void GmUIterminal::SetCoutFile( const G4String& fname )
{
  //--- Check if cout and cerr go to the same file
  if( fcerr && theCerrFileName == fname ) {
    fcout = fcerr;
  } else {
    theCoutFileName = fname;
    fcout = new std::ofstream(fname);
  }
}


//-------------------------------------------------------------------------
void GmUIterminal::SetCerrFile( const G4String& fname )
{
  //--- Check if cout and cerr go to the same file
  if( fcout && theCoutFileName == fname ) {
    fcerr = fcout;
  } else {
    theCerrFileName = fname;
    fcerr = new std::ofstream(fname);
  }
}


//-------------------------------------------------------------------------
void GmUIterminal::SetWriteLog( G4bool bWrite )
{
  bWriteLogFiles = bWrite;
}
