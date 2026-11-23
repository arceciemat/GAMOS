#include"RacetrackCoilCreator.hh"
#include"RacetrackCoil.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
RacetrackCoilCreator::RacetrackCoilCreator():
		ParametrizedFieldCreator(G4String("Coil")) 
{ RacetrackCoilCmdDir=0;
  SetRoutCmd=0;
  SetD1Cmd=0;
  SetD2Cmd=0;
  SetL1Cmd=0;
  SetL2Cmd=0;
  SetICmd=0;
} 
////////////////////////////////////////////////////////////////////////////////
//
RacetrackCoilCreator::~RacetrackCoilCreator()
{
 if (RacetrackCoilCmdDir) delete RacetrackCoilCmdDir;
 if (SetRoutCmd) delete SetRoutCmd;
 if (SetD1Cmd) delete SetD1Cmd;
 if (SetD2Cmd) delete SetD2Cmd;
 if (SetL1Cmd) delete SetL1Cmd;
 if (SetL2Cmd) delete SetL2Cmd;
 if (SetICmd) delete SetICmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void RacetrackCoilCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/Coil/";
  RacetrackCoilCmdDir=new G4UIdirectory(dir_name.data());
  RacetrackCoilCmdDir->SetGuidance("Interactive commands to define a RacetrackCoil");
  
  
  
  //Definition of the Radius
  G4String title_cmd =dir_name+"SetRout";
  SetRoutCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRoutCmd->SetGuidance("Define the outer Radius of the circular sections  of the  Racetrack Coil");
  SetRoutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRoutCmd->SetUnitCategory("Length");
 
  //Definition of D1
  title_cmd =dir_name+"SetD1";
  SetD1Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD1Cmd->SetGuidance("Define the D1 of the coil section");
  SetD1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD1Cmd->SetUnitCategory("Length");
  
  //Definition of D2
  title_cmd =dir_name+"SetD2";
  SetD2Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD2Cmd->SetGuidance("Define the D2 of the coil section");
  SetD2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD2Cmd->SetUnitCategory("Length");
  //Definition of the the Current
  
  
  //Definition of L1
  title_cmd =dir_name+"SetL1";
  SetL1Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetL1Cmd->SetGuidance("Define the length of the vertical sections");
  SetL1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetL1Cmd->SetUnitCategory("Length");
  
  //Definition of L2
  title_cmd =dir_name+"SetL2";
  SetL2Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetL2Cmd->SetGuidance("Define the length of the horizontal sections");
  SetL2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetL2Cmd->SetUnitCategory("Length");
  
  //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current of the coil");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool RacetrackCoilCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetRoutCmd){
		Rout = SetRoutCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetD1Cmd){
		D1 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetD2Cmd){
		D2 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetL1Cmd){
		L1 = SetL1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetL2Cmd){
		L2 = SetL2Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* RacetrackCoilCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new RacetrackCoil(Rout, D1/2.,D2/2., L1/2.,L2/2.,I);
}
