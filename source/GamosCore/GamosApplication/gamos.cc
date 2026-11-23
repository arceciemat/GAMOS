//#define private protected
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "GamosCore/GamosRunManager/include/GmUIterminal.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmMTMergeMgr.hh"

#include "G4UImanager.hh"
#ifndef _WIN32  // tcsh terminal: not available on Windows
#include "G4UItcsh.hh"
#endif
#include "G4GeometryManager.hh"

#ifdef WIN32
#define G4VIS_USE //WINDEB
#endif

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif


//---------------------------------------------------------------------------
int main(int argc,char** argv) 
{
  if( argc > 1 ) G4cout << "@@@@@ Running GAMOS with script: " << argv[1] << G4endl;
  
  GmRunManager* runManager = new GmRunManager();
  runManager->CreateG4RunManager();

#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialise();
#endif

  // Get the pointer to the User Interface manager 
  G4UImanager * UI = G4UImanager::GetUIpointer();  
  UI->SetVerboseLevel(1);

  GmUIterminal* sessionGm = 0;

  if(argc==1)
  // Define (G)UI terminal for interactive mode  
  { 
    // G4UIterminal is a (dumb) terminal.
    G4UIsession * session = 0;
    session = new GmUIterminal();

    sessionGm = (GmUIterminal*)(session);

    UI->SetCoutDestination( sessionGm );
    sessionGm->SetCoutFile( "gamos.log" );
    sessionGm->SetCerrFile( "gamos_error.log" );

    sessionGm->ExecuteCommand("/control/verbose 1");
    sessionGm->ExecuteCommand("/run/verbose 1");

    session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  { 
    sessionGm = new GmUIterminal;
    UI->SetCoutDestination( sessionGm );
    sessionGm->SetCoutFile( "gamos.log" );
    sessionGm->SetCerrFile( "gamos_error.log" );

    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);

  }
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  //  G4cout << "Run Manager deleting. " << G4endl;
  delete runManager;

  //--- Multithreading: merge the files written by each worker thread (NAME.thN.EXT -> NAME.EXT)
  GmMTMergeMgr::MergeThreadFiles( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmMTMergeMgr:KeepThreadFiles",0)) );

  GmAnalysisMgr::DeleteInstances();

  G4GeometryManager::GetInstance()->OpenGeometry(); // to avoid warnings

  GmFilterMgr::GetInstance()->DeleteFilters();
  GmClassifierMgr::GetInstance()->DeleteClassifiers();

  exit(0);
  return 0;
}
