#include"MultiplyFieldCreator.hh"
#include"MultiplyField.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithADoubleAndUnit.hh"
#include"G4UIdirectory.hh"
#include"MagneticFieldModelManager.hh"

////////////////////////////////////////////////////////////////////////////////
//
MultiplyFieldCreator::MultiplyFieldCreator():
		ParametrizedFieldCreator(G4String("MultiplyField")) 
{ MultiplyFieldCmdDir=0;
  SetFieldAndFactorCmd =0;
} 
////////////////////////////////////////////////////////////////////////////////
//
MultiplyFieldCreator::~MultiplyFieldCreator()
{
 if (MultiplyFieldCmdDir) delete MultiplyFieldCmdDir;
 if (SetFieldAndFactorCmd) delete SetFieldAndFactorCmd;
 
}
////////////////////////////////////////////////////////////////////////////////
//
void MultiplyFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 G4String dir_name= cmd_dir_name+"/MultiplyField/";
 
  MultiplyFieldCmdDir=new G4UIdirectory(dir_name.data());
  MultiplyFieldCmdDir->SetGuidance("Interactive commands to define a field multiplied by a constant factor");
  
  //Definition of the compound field
  G4String title_cmd =dir_name+"DefineFieldAndFactor";
  SetFieldAndFactorCmd = new G4UIcommand(title_cmd,aMessenger);
  SetFieldAndFactorCmd->SetGuidance("Define the field and the factor");
  G4UIparameter* field_name = new G4UIparameter("Field name",'s',false);
  G4UIparameter* factor_par = new G4UIparameter("Factor",'d',true);
  SetFieldAndFactorCmd->SetParameter(field_name);
  SetFieldAndFactorCmd->SetParameter(factor_par);
  SetFieldAndFactorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool MultiplyFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd ==SetFieldAndFactorCmd){
		const char* paramString= parameter_string;
     		G4String field_name;
     		std::istringstream is((char*)paramString);
     		is>>field_name>>factor;
		theField = MagneticFieldModelManager::GetInstance()->GetCreatedField(field_name);
			
	}
	
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* MultiplyFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new MultiplyField(theField, factor);
}
