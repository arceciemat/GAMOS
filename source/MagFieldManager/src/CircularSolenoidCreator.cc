#include"CircularSolenoidCreator.hh"
#include"CircularSolenoid.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
CircularSolenoidCreator::CircularSolenoidCreator():
		ParametrizedFieldCreator(G4String("CircularSolenoid")) 
{ CircularSolenoidCmdDir=0;
  SetRadiusCmd=0;
  SetDZCmd=0;
  SetDRCmd=0;
  SetICmd=0;
} 
////////////////////////////////////////////////////////////////////////////////
//
CircularSolenoidCreator::~CircularSolenoidCreator()
{
 if (CircularSolenoidCmdDir) delete CircularSolenoidCmdDir;
 if (SetRadiusCmd) delete SetRadiusCmd;
 if (SetDRCmd) delete SetDRCmd;
 if (SetDZCmd) delete SetDZCmd;
 if (SetICmd) delete SetICmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void CircularSolenoidCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/CircularSolenoid/";
  CircularSolenoidCmdDir=new G4UIdirectory(dir_name.data());
  CircularSolenoidCmdDir->SetGuidance("Interactive commands to define a spreadout current loop");
  
  
  
  //Definition of the Radius
  G4String title_cmd =dir_name+"SetRadius";
  SetRadiusCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRadiusCmd->SetGuidance("Define the internal Radius of the circular solenoid");
  SetRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRadiusCmd->SetUnitCategory("Length");
 
  //Definition of DR
  title_cmd =dir_name+"SetDR";
  SetDRCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetDRCmd->SetGuidance("Define the radial extension  of the current loop");
  SetDRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDRCmd->SetUnitCategory("Length");
  
  //Definition of DZ
  title_cmd =dir_name+"SetDZ";
  SetDZCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetDZCmd->SetGuidance("Define the dZ extension of the current loop");
  SetDZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDZCmd->SetUnitCategory("Length");
  //Definition of the the Current
 
  title_cmd =dir_name+"SetCurrent";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current of the current loop");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool CircularSolenoidCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetRadiusCmd){
		Radius = SetRadiusCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetDRCmd){
		DR = SetDRCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetDZCmd){
		DZ = SetDRCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* CircularSolenoidCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new CircularSolenoid(Radius, DR,DZ, I);
}
