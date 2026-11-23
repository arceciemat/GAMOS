#include"CoilType3Creator.hh"
#include"CoilType3.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
CoilType3Creator::CoilType3Creator():
		ParametrizedFieldCreator(G4String("CoilType3")) 
{ CoilType3CmdDir=0;
  SetRoutCmd=0;
  SetRinCmd=0;
  SetRoutCornerCmd=0; 
  SetD1Cmd=0;
  SetD2Cmd=0;
  SetAlpha0Cmd=0;
  SetdAlphaCmd =0; 
  SetICmd=0;
  
  
  
} 
////////////////////////////////////////////////////////////////////////////////
//
CoilType3Creator::~CoilType3Creator()
{
 if (CoilType3CmdDir) delete CoilType3CmdDir;
 if (SetRoutCmd) delete SetRoutCmd;
 if (SetRoutCornerCmd) delete SetRoutCornerCmd;
 if (SetRinCmd) delete SetRinCmd;
 if (SetD1Cmd) delete SetD1Cmd;
 if (SetD2Cmd) delete SetD2Cmd;
 if (SetAlpha0Cmd) delete SetAlpha0Cmd;
 if (SetdAlphaCmd) delete SetdAlphaCmd;
 if (SetICmd) delete SetICmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void CoilType3Creator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/CoilType3/";
  CoilType3CmdDir=new G4UIdirectory(dir_name.data());
  CoilType3CmdDir->SetGuidance("Interactive commands to define a Coil of Type 3");
  
  
  
  //Definition of the Outer radius of the external big circular section
  G4String title_cmd =dir_name+"SetRout";
  SetRoutCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRoutCmd->SetGuidance("Define the outer radius of the external big circular section of the   Coil of type 3");
  SetRoutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRoutCmd->SetUnitCategory("Length");
  
   //Definition of the Outer radius of the external big circular section
  title_cmd =dir_name+"SetRin";
  SetRinCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRinCmd->SetGuidance("Define the inner radius of the internal big circular section of the   Coil of type 3");
  SetRinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRinCmd->SetUnitCategory("Length");
  
 //Definition of the Outer radius of the external big circular section
  title_cmd =dir_name+"SetRoutCorner";
  SetRoutCornerCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRoutCornerCmd->SetGuidance("Define the outer radius of the circular corners of the   Coil of type 3");
  SetRoutCornerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRoutCornerCmd->SetUnitCategory("Length");
 
  //Definition of D1
  title_cmd =dir_name+"SetD1";
  SetD1Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD1Cmd->SetGuidance("Define the X size  of the coil section");
  SetD1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD1Cmd->SetUnitCategory("Length");
  
  //Definition of D2
  title_cmd =dir_name+"SetD2";
  SetD2Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD2Cmd->SetGuidance("Define the Y size  of the coil section");
  SetD2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD2Cmd->SetUnitCategory("Length");
  
  //Definition of the start angle of the big circular section
  title_cmd =dir_name+"SetAlpha0";
  SetAlpha0Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetAlpha0Cmd->SetGuidance("Define the start angle from the Z axis for the big circular sections of the Coil of type 3");
  SetAlpha0Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetAlpha0Cmd->SetUnitCategory("Angle");
  
  //Definition of the angle covered by the coil
  title_cmd =dir_name+"SetdAlpha";
  SetdAlphaCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetdAlphaCmd->SetGuidance("Define the angle covered by the the big circular sections of the Coil of type 3");
  SetdAlphaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetdAlphaCmd->SetUnitCategory("Angle");
  
  
  
  //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current of the coil");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool CoilType3Creator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetRoutCmd){
		Rout = SetRoutCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetRinCmd){
		Rin = SetRinCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetRoutCornerCmd){
		RoutCorner = SetRoutCornerCmd->GetNewDoubleValue(parameter_string);
	}
	
	else if (aCmd == SetD1Cmd){
		D1 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetD2Cmd){
		D2 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetAlpha0Cmd){
		Alpha0 = SetAlpha0Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetdAlphaCmd){
		dAlpha = SetdAlphaCmd->GetNewDoubleValue(parameter_string);
	}
	
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* CoilType3Creator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new CoilType3(RoutCorner,D1, D2, Rin, Rout, Alpha0, dAlpha,I);
}
