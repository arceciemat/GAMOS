#include"UniformFieldCreator.hh"
#include"G4UniformMagField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
UniformFieldCreator::UniformFieldCreator():
		ParametrizedFieldCreator(G4String("UniformField")) 
{theCmd =0;
 UniformFieldDir =0;
} 
////////////////////////////////////////////////////////////////////////////////
//
UniformFieldCreator::~UniformFieldCreator()
{ if (theCmd) delete theCmd;
  if (UniformFieldDir) delete UniformFieldDir;
}
////////////////////////////////////////////////////////////////////////////////
//
void UniformFieldCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
 
  G4String dir_name= cmd_dir_name+"/UniformField/";
  
  UniformFieldDir=new G4UIdirectory(dir_name.data());
  UniformFieldDir->SetGuidance("Interactive commands to define a uniform field");
  
  
  G4String title_cmd =cmd_dir_name+"/UniformField/SetBvec";
  theCmd = new G4UIcmdWith3VectorAndUnit(title_cmd,aMessenger);
  theCmd->SetGuidance("Define a uniform field");
  theCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  theCmd->SetUnitCategory("Magnetic flux density");
  
    
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool UniformFieldCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string)
{ if (aCmd != theCmd) return false;
  B_vec = theCmd->GetNew3VectorValue(parameter_string);
  return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* UniformFieldCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new G4UniformMagField(B_vec);

}
