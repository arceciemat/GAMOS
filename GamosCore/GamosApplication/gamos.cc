#define private public
#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "GamosCore/GamosRunManager/include/GmUIterminal.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"

#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#include "G4GeometryManager.hh"

#include "TSystem.h"
#include "TROOT.h"

//---------------------------------------------------------------------------
int main(int argc,char** argv) 
{
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

  GmAnalysisMgr::DeleteInstances();

  G4GeometryManager::GetInstance()->OpenGeometry(); // to avoid warnings

  GmFilterMgr::GetInstance()->DeleteFilters();
  GmClassifierMgr::GetInstance()->DeleteClassifiers();

  // geom =  TClass::GetClass("geomtext",true,true);
  // G4cout << " GEOM PLUGIN " << geom << G4endl;

  exit(1);
  return 0;
}
