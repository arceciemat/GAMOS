#include "GmFactoriesMessenger.hh"
#include "GmRunManager.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

GmFactoriesMessenger::GmFactoriesMessenger(GmRunManager* myua): 
  myAction(myua) 
{
  geometryFCmd = new GmUIcmdWithAString("/gamos/geometry",this);
  geometryFCmd->SetGuidance("Select the geometry by its plugin name");
  geometryFCmd->SetParameterName("choice",true);
  geometryFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  physicsListFCmd = new GmUIcmdWithAString("/gamos/physicsList",this);
  physicsListFCmd->SetGuidance("Select the physics list by its plugin name");
  physicsListFCmd->SetParameterName("choice",true);
  physicsListFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  generatorFCmd = new GmUIcmdWithAString("/gamos/generator",this);
  generatorFCmd->SetGuidance("Select the generator by its plugin name");
  generatorFCmd->SetParameterName("choice",true);
  generatorFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  userActionsFCmd = new GmUIcmdWithAString("/gamos/userAction",this);
  userActionsFCmd->SetGuidance("Enable one userAction by its plugin name");
  userActionsFCmd->SetParameterName("choice",true);
  userActionsFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  verbosityFCmd = new GmUIcmdWithAString("/gamos/verbosity",this);
  verbosityFCmd->SetGuidance("Set the verbosity level for a certain verbosity typel: verbType verbValue (-1,0,1,2,3,4) or (silent,error,warning,info,debug,test)");
  verbosityFCmd->SetParameterName("choice",true);
  verbosityFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  digitizerFCmd = new GmUIcmdWithAString("/gamos/SD/digitizer",this);
  digitizerFCmd->SetGuidance("Add a digitizer for a given sensitive detector type");
  digitizerFCmd->SetParameterName("choice",true);
  digitizerFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  rechitBuilderFCmd = new GmUIcmdWithAString("/gamos/SD/recHitBuilder",this);
  rechitBuilderFCmd->SetGuidance("Add a rechit builder for a given sensitive detector type");
  rechitBuilderFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

GmFactoriesMessenger::~GmFactoriesMessenger() 
{
  if (geometryFCmd) delete geometryFCmd;
  if (physicsListFCmd) delete physicsListFCmd;
  if (generatorFCmd) delete generatorFCmd;
  if (userActionsFCmd) delete userActionsFCmd;
  if (verbosityFCmd) delete verbosityFCmd;
  if (digitizerFCmd) delete digitizerFCmd;
  if (rechitBuilderFCmd) delete rechitBuilderFCmd;
}

void GmFactoriesMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == geometryFCmd) {
    myAction->SelectGeometry(newValues);
  }else if (command == physicsListFCmd) {
    myAction->SelectPhysicsList(newValues);
  }else if (command == generatorFCmd) {
    myAction->SelectGenerator(newValues);
  }else if (command == userActionsFCmd) {
    myAction->SelectUserAction(newValues);
  }else if (command == verbosityFCmd) {
    myAction->SelectVerbosity(newValues);
  }else if (command == digitizerFCmd) {
    GmGenUtils::CheckNWords(newValues,2,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 2 arguments: DigitizerClass SDtype"); 
    G4String Dclass;
    G4String SDtype;
    std::istringstream is((char*)newValues.c_str());
    is >> Dclass >> SDtype;
    GmHitsEventMgr::GetInstance()->AddDigitizer(Dclass, SDtype);

  }else if (command == rechitBuilderFCmd) {
    GmGenUtils::CheckNWords(newValues,2,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 2 arguments: RecHitBuilderClass SDtype"); 
    G4String RHclass;
    G4String SDtype;
    std::istringstream is((char*)newValues.c_str());
    is >> RHclass >> SDtype;
    GmHitsEventMgr::GetInstance()->AddRecHitBuilder(RHclass, SDtype);
  }
  return;
}
