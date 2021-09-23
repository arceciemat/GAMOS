#include"InterpolatedFieldCreator.hh"
#include"InterpolatedField.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"
#include"G4UIcmdWithABool.hh"
#include"G4UIcmdWithAString.hh"
#include"G4UIcmdWithADouble.hh"

////////////////////////////////////////////////////////////////////////////////
//
InterpolatedFieldCreator::InterpolatedFieldCreator():
		ParametrizedFieldCreator(G4String("InterpolatedField")) 
{ InterpolatedFieldCmdDir=0;
  SetGridFileNameCmd=0;

} 
////////////////////////////////////////////////////////////////////////////////
//
InterpolatedFieldCreator::~InterpolatedFieldCreator()
{
  if (InterpolatedFieldCmdDir) delete InterpolatedFieldCmdDir;
  if (SetGridFileNameCmd) delete SetGridFileNameCmd;
;
}
////////////////////////////////////////////////////////////////////////////////
//
void InterpolatedFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/InterpolatedField/";
  InterpolatedFieldCmdDir=new G4UIdirectory(dir_name.data());
  InterpolatedFieldCmdDir->SetGuidance("Interactive commands to define a InterpolatedField");
  
  
  
 
 
 
  
  //File from which the grid can be readed
  G4String title_cmd =dir_name+"SetGridFileName";
  SetGridFileNameCmd = new G4UIcmdWithAString(title_cmd,aMessenger);
  SetGridFileNameCmd->SetGuidance("Define the file where the grid to be interpolated is conatined");
  SetGridFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool InterpolatedFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetGridFileNameCmd){
		file_name = parameter_string;
	}
	
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* InterpolatedFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new InterpolatedField(file_name);
}
