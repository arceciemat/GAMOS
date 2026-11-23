#include"CompoundFieldCreator.hh"
#include"CompoundField.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithoutParameter.hh"
#include"G4UIdirectory.hh"
#include"G4UIparameter.hh"
#include"MagneticFieldModelManager.hh"
#include"TransformationManager.hh"
#include"G4AffineTransform.hh"
#include "CLHEP/Geometry/BasicVector3D.h"
////////////////////////////////////////////////////////////////////////////////
//
CompoundFieldCreator::CompoundFieldCreator():
		ParametrizedFieldCreator(G4String("CompoundField")) 
{ 
  list_of_fields.clear();
  list_of_transformations.clear();
} 
////////////////////////////////////////////////////////////////////////////////
//
CompoundFieldCreator::~CompoundFieldCreator()
{
 
}
////////////////////////////////////////////////////////////////////////////////
//
void CompoundFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/CompoundField/";
  CompoundFieldCmdDir=new G4UIdirectory(dir_name.data());
  CompoundFieldCmdDir->SetGuidance("Interactive commands to define a compound field");
  
  
  
  //Definition of the compound field
  G4String title_cmd =dir_name+"AddField";
  AddFieldCmd = new G4UIcommand(title_cmd,aMessenger);
  AddFieldCmd->SetGuidance("Add a field to the next created compound field");
  G4UIparameter* field_name = new G4UIparameter("Field name",'s',false);
  G4UIparameter* trans_name = new G4UIparameter("Transformation name",'s',true);
  trans_name->SetDefaultValue("NoTransformation");
  AddFieldCmd->SetParameter(field_name);
  AddFieldCmd->SetParameter(trans_name);
  AddFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  title_cmd =dir_name+"Reset";
  ResetCmd = new G4UIcmdWithoutParameter(title_cmd,aMessenger);
  ResetCmd->SetGuidance("Reset the list of fields contained in the next created compound field");
  ResetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}
////////////////////////////////////////////////////////////////////////////////
//
G4bool CompoundFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == AddFieldCmd){
		const char* paramString= parameter_string;
     		G4String field_name, transformation_name;
     		std::istringstream is((char*)paramString);
     		is>>field_name>>transformation_name;
		G4MagneticField* aField = MagneticFieldModelManager::GetInstance()->GetCreatedField(field_name);
		G4AffineTransform* aTransformation =0;
		if (transformation_name != "NoTransformation") 
			aTransformation = TransformationManager::GetInstance()->GetTransformation(transformation_name);
		if (aField && (aTransformation ||  transformation_name == "NoTransformation")){
			list_of_fields.push_back(aField);
			list_of_transformations.push_back(aTransformation);
		}	
	}
	else if (aCmd == ResetCmd){
		 list_of_fields.clear();
		 list_of_transformations.clear();
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* CompoundFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new CompoundField(list_of_fields,list_of_transformations);
}
