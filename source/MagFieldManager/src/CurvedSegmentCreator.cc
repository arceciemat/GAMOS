#include"CurvedSegmentCreator.hh"
#include"CurvedSegment.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
CurvedSegmentCreator::CurvedSegmentCreator():
		ParametrizedFieldCreator(G4String("CurvedSegment")) 
{ CurvedSegmentCmdDir=0;
  SetD1Cmd=0;
  SetRINCmd=0;
  SetROUTCmd=0;
  SetPHI0Cmd=0;
  SetDPHICmd=0;
  SetICmd=0;
} 
////////////////////////////////////////////////////////////////////////////////
//
CurvedSegmentCreator::~CurvedSegmentCreator()
{
 if (CurvedSegmentCmdDir) delete CurvedSegmentCmdDir;
 if (SetD1Cmd) delete SetD1Cmd;
 if (SetRINCmd) delete SetRINCmd;
 if (SetROUTCmd) delete SetROUTCmd;
 if (SetPHI0Cmd) delete SetPHI0Cmd;
 if (SetDPHICmd) delete SetDPHICmd;
 if (SetICmd) delete SetICmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void CurvedSegmentCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/CurvedSegment/";
  CurvedSegmentCmdDir=new G4UIdirectory(dir_name.data());
  CurvedSegmentCmdDir->SetGuidance("Interactive commands to define a CurvedSegment");
  
  
  
 
 
  //Definition of D1
  G4String title_cmd =dir_name+"SetD1";
  SetD1Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetD1Cmd->SetGuidance("Define the D1 parameter of the curved section");
  SetD1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetD1Cmd->SetUnitCategory("Length");
  
  //Definition of RIN
  title_cmd =dir_name+"SetRin";
  SetRINCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRINCmd->SetGuidance("Define the RIN parameter of the curved section");
  SetRINCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRINCmd->SetUnitCategory("Length");
  
   //Definition of ROUT
  title_cmd =dir_name+"SetRout";
  SetROUTCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetROUTCmd->SetGuidance("Define the ROUT parameter of the curved section");
  SetROUTCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetROUTCmd->SetUnitCategory("Length");
  
  //Definition of PHI0
  title_cmd =dir_name+"SetPhi0";
  SetPHI0Cmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetPHI0Cmd->SetGuidance("Define the PHI0 parameter of the curved section");
  SetPHI0Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetPHI0Cmd->SetUnitCategory("Angle");
  
  //Definition of DPHI
  title_cmd =dir_name+"SetDphi";
  SetDPHICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetDPHICmd->SetGuidance("Define the DPHI parameter of the curved section");
  SetDPHICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDPHICmd->SetUnitCategory("Angle");
  
  
  
  
 //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current of the coil");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool CurvedSegmentCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetD1Cmd){
		D1 = SetD1Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetRINCmd){
		RIN = SetRINCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetROUTCmd){
		ROUT = SetROUTCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetPHI0Cmd){
		PHI0 = SetPHI0Cmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetDPHICmd){
		DPHI = SetDPHICmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* CurvedSegmentCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new CurvedSegment(D1,RIN,ROUT,PHI0,DPHI,I);
}
