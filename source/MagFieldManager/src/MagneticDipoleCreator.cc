#include"MagneticDipoleCreator.hh"
#include"MagneticDipole.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"G4UIdirectory.hh"

////////////////////////////////////////////////////////////////////////////////
//
MagneticDipoleCreator::MagneticDipoleCreator():
		ParametrizedFieldCreator(G4String("MagneticDipole")) 
{ MagneticDipoleCmdDir=0;
  SetDipoleMomentCmd=0;
  SetDipoleCenterCmd=0;
} 
////////////////////////////////////////////////////////////////////////////////
//
MagneticDipoleCreator::~MagneticDipoleCreator()
{
 if (MagneticDipoleCmdDir) delete MagneticDipoleCmdDir;
 if (SetDipoleMomentCmd) delete SetDipoleMomentCmd;
 if (SetDipoleCenterCmd) delete SetDipoleCenterCmd;
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticDipoleCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/MagneticDipole/";
  MagneticDipoleCmdDir=new G4UIdirectory(dir_name.data());
  MagneticDipoleCmdDir->SetGuidance("Interactive commands to define a magnetic dipole");
  
  
  
  //Definition of the dipole moment
  G4String title_cmd =dir_name+"SetDipoleMoment";
  SetDipoleMomentCmd = new G4UIcmdWith3VectorAndUnit(title_cmd,aMessenger);
  SetDipoleMomentCmd->SetGuidance("Define the dipole moment");
  SetDipoleMomentCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDipoleMomentCmd->SetUnitCategory("Magnetic moment");
  
  //Definition of the dipole center
  title_cmd =dir_name+"SetDipoleCenter";
  SetDipoleCenterCmd = new G4UIcmdWith3VectorAndUnit(title_cmd,aMessenger);
  SetDipoleCenterCmd->SetGuidance("Define the dipole center");
  SetDipoleCenterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDipoleCenterCmd->SetUnitCategory("Length");
  
  
  
  
 
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
G4bool MagneticDipoleCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetDipoleMomentCmd){
		 theDipoleMoment = SetDipoleMomentCmd->GetNew3VectorValue(parameter_string);
	}
	else if (aCmd == SetDipoleCenterCmd){
		 theDipoleCenter = SetDipoleCenterCmd->GetNew3VectorValue(parameter_string);
	}
	else return false;
	
	return true;

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* MagneticDipoleCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new MagneticDipole(theDipoleMoment, theDipoleCenter);
}
