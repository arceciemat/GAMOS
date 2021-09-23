#include"IdealToroidWithRectangularCoilCreator.hh"
#include"IdealToroidWithRectangularCoil.hh"
#include"G4UIcommand.hh"
#include"G4UIcmdWithoutParameter.hh"
#include"G4UIdirectory.hh"
#include"G4UIparameter.hh"
#include"MagneticFieldModelManager.hh"

#include"G4UIcmdWithADoubleAndUnit.hh"
#include "CLHEP/Geometry/BasicVector3D.h"

////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithRectangularCoilCreator::IdealToroidWithRectangularCoilCreator():
		ParametrizedFieldCreator(G4String("IdealToroidWithRectangularCoil")) 
{ 
  
} 
////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithRectangularCoilCreator::~IdealToroidWithRectangularCoilCreator()
{
 
}
////////////////////////////////////////////////////////////////////////////////
//
void IdealToroidWithRectangularCoilCreator::CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)
{ 
  G4String dir_name= cmd_dir_name+"/IdealToroidWithRectangularCoil/";
  IdealToroidWithRectangularCoilCmdDir=new G4UIdirectory(dir_name.data());
  IdealToroidWithRectangularCoilCmdDir->SetGuidance("Interactive commands to define a compound field");  
 
 
 
 //Definition of RToroid
  G4String title_cmd =dir_name+"SetRToroid";
  SetRToroidCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRToroidCmd->SetGuidance("Define the Radius of the Toroid");
  SetRToroidCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRToroidCmd->SetUnitCategory("Length");
  
   //Definition of RoutCorner
  title_cmd =dir_name+"SetRoutCorner";
  SetRoutCornerCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetRoutCornerCmd->SetGuidance("Define the outer Radius of the circular sections  of the  Coils in the Toroid");
  SetRoutCornerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetRoutCornerCmd->SetUnitCategory("Length");
 
  //Definition of Dcoil
  title_cmd =dir_name+"SetDcoil";
  SetDcoilCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetDcoilCmd->SetGuidance("Define the width of the the Coil in the Rho direction");
  SetDcoilCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDcoilCmd->SetUnitCategory("Length");
  
  
  
  
  //Definition of Lvert
  title_cmd =dir_name+"SetLVert";
  SetLvertCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLvertCmd->SetGuidance("Define the length of the vertical sections");
  SetLvertCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLvertCmd->SetUnitCategory("Length");
  
  //Definition of Lhoriz
  title_cmd =dir_name+"SetLHoriz";
  SetLhorizCmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetLhorizCmd->SetGuidance("Define the length of the horizontal sections");
  SetLhorizCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLhorizCmd->SetUnitCategory("Length");
  
  //Definition of the the Current
 
  title_cmd =dir_name+"SetI";
  SetICmd = new G4UIcmdWithADoubleAndUnit(title_cmd,aMessenger);
  SetICmd->SetGuidance("Define the current turns nI");
  SetICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetICmd->SetUnitCategory("Electric current");
 
  
  
  
  

}
////////////////////////////////////////////////////////////////////////////////
//
G4bool IdealToroidWithRectangularCoilCreator::DefineParameters(G4UIcommand* aCmd , G4String parameter_string){
	if (aCmd == SetRoutCornerCmd){
		RoutCorner = SetRoutCornerCmd->GetNewDoubleValue(parameter_string);
	}
	if (aCmd == SetRToroidCmd){
		RToroid = SetRToroidCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetDcoilCmd){
		Dcoil = SetDcoilCmd->GetNewDoubleValue(parameter_string);
	}
	
	else if (aCmd == SetLvertCmd){
		Lvert = SetLvertCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetLhorizCmd){
		Lhoriz = SetLhorizCmd->GetNewDoubleValue(parameter_string);
	}
	else if (aCmd == SetICmd){
		I = SetICmd->GetNewDoubleValue(parameter_string);
	}
	else return false;
	
	return true;
	
	

}
////////////////////////////////////////////////////////////////////////////////
//
G4MagneticField* IdealToroidWithRectangularCoilCreator::CreateNewField()
{ nb_fields_of_same_model+=1;
  return new IdealToroidWithRectangularCoil( RToroid,Lhoriz,Lvert, RoutCorner, Dcoil,  I);
 
  
}
