#include"CoilType2Creator.hh"
#include"CoilType2.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
CoilType2Creator::CoilType2Creator():
		ParametrizedFieldCreator(G4String("CoilType2")) 
{ CoilType2CmdDir=0;
  SetRoutCornerCmd=0;
  SetD1Cmd=0;
  SetD2Cmd=0;
  SetLVertLeftCmd=0;
  SetLVertRightCmd=0;
  SetLHorizUpCmd=0;
  SetLHorizDownCmd=0; 
  
  
} 
////////////////////////////////////////////////////////////////////////////////
//
CoilType2Creator::~CoilType2Creator()
{
 if (CoilType2CmdDir) delete CoilType2CmdDir;
 if (SetRoutCornerCmd) delete SetRoutCornerCmd;
 if (SetD1Cmd) delete SetD1Cmd;
 if (SetD2Cmd) delete SetD2Cmd;
 if (SetLVertLeftCmd) delete SetLVertLeftCmd;
 if (SetLVertRightCmd) delete SetLVertRightCmd;
 if (SetLHorizUpCmd) delete SetLHorizUpCmd;
 if (SetLHorizDownCmd) delete SetLHorizDownCmd;
 if (SetICmd) delete SetICmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void CoilType2Creator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/CoilType2/";
  CoilType2CmdDir=new G4UIdirectory(dir_name.data());
  CoilType2CmdDir->SetGuidance("Interactive commands to define a Coil of Type 2");
  
  
  
  //Definition of the Radius
  G4String title_cmd =dir_name+"SetRout";
  SetRoutCornerCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRoutCornerCmd->SetGuidance("Define the outer Radius of the circular corners  of the  coil of type 2");
  SetRoutCornerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRoutCornerCmd->SetUnitCategory("Length");
 
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
  
  
  //Definition of LVertLeft
  title_cmd =dir_name+"SetLVertLeft";
  SetLVertLeftCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLVertLeftCmd->SetGuidance("Define the length of the left vertical section");
  SetLVertLeftCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLVertLeftCmd->SetUnitCategory("Length");
  
  //Definition of LVertRight
  title_cmd =dir_name+"SetLVertRight";
  SetLVertRightCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLVertRightCmd->SetGuidance("Define the length of the right vertical section");
  SetLVertRightCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLVertRightCmd->SetUnitCategory("Length");
  
  //Definition of LHorizUp
  title_cmd =dir_name+"SetLHorizUp";
  SetLHorizUpCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLHorizUpCmd->SetGuidance("Define the length of the upper horizontal section");
  SetLHorizUpCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLHorizUpCmd->SetUnitCategory("Length");
  
  //Definition of LHorizDown
  title_cmd =dir_name+"SetLHorizDown";
  SetLHorizDownCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLHorizDownCmd->SetGuidance("Define the length of the lower horizontal section");
  SetLHorizDownCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLHorizDownCmd->SetUnitCategory("Length");
  
  //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current of the coil");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool CoilType2Creator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetRoutCornerCmd){
		RoutCorner = SetRoutCornerCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetD1Cmd){
		D1 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetD2Cmd){
		D2 = SetD2Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetLVertLeftCmd){
		LVertLeft = SetLVertLeftCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetLVertRightCmd){
		LVertRight= SetLVertRightCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetLHorizUpCmd){
		LHorizUp = SetLHorizUpCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetLHorizDownCmd){
		LHorizDown = SetLHorizDownCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* CoilType2Creator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new CoilType2(RoutCorner, D1,D2, LVertLeft,LVertRight,LHorizUp,LHorizDown,I);
}
