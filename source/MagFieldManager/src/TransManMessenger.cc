#include"TransManMessenger.hh"
#include"TransformationManager.hh"
#include"G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIparameter.hh"
#include "ParametrizedFieldCreator.hh"
//units
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "CLHEP/Geometry/BasicVector3D.h"

TransManMessenger::TransManMessenger(TransformationManager* aTransManager)
{  theTransManager = aTransManager;
  
  
   TransManagerDir = new G4UIdirectory("/CoordinateTransformation/");
   TransManagerDir->SetGuidance("Definition of coordinate transformation");
  
  
   SelectTransformationCmd = new G4UIcmdWithAString("/CoordinateTransformation/Select",this);
   SelectTransformationCmd->SetGuidance("Select a transformation already registered");
   SelectTransformationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
   
   ResetToUnitaryCmd = new G4UIcmdWithoutParameter("/CoordinateTransformation/ResetToUnit",this);
   ResetToUnitaryCmd->SetGuidance("Reset to the unit transformation"); 
   ResetToUnitaryCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
   
   AddRotationAroundXCmd = new G4UIcmdWithADoubleAndUnit("/CoordinateTransformation/AddRotationX",this);
   AddRotationAroundXCmd->SetGuidance("Add a rotation around the X axis");
   AddRotationAroundXCmd->SetUnitCategory("Angle");
   AddRotationAroundXCmd->SetParameterName("Angle",false);
   
   AddRotationAroundYCmd = new G4UIcmdWithADoubleAndUnit("/CoordinateTransformation/AddRotationY",this);
   AddRotationAroundYCmd->SetGuidance("Add a rotation around the Y axis");
   AddRotationAroundYCmd->SetUnitCategory("Angle");
   AddRotationAroundYCmd->SetParameterName("Angle",false);
   
   AddRotationAroundZCmd = new G4UIcmdWithADoubleAndUnit("/CoordinateTransformation/AddRotationZ",this);
   AddRotationAroundZCmd->SetGuidance("Add a rotation around the Z axis");
   AddRotationAroundZCmd->SetUnitCategory("Angle");
   AddRotationAroundZCmd->SetParameterName("Angle",false);
   
   
   AddRotationAroundAnAxisCmd = new G4UIcommand("/CoordinateTransformation/AddRotationAroundAnAxis",this);
   AddRotationAroundAnAxisCmd->SetGuidance("Add a rotation around a given axis");
   G4UIparameter* dirX_param = new G4UIparameter("dirX",'d',false);
   G4UIparameter* dirY_param = new G4UIparameter("dirY",'d',false);
   G4UIparameter* dirZ_param = new G4UIparameter("dirZ",'d',false);
   G4UIparameter* angle_param = new G4UIparameter("Angle",'d',false);
   G4UIparameter* angle_unit_param = new G4UIparameter("AngleUnit",'s',false);
   angle_unit_param->SetParameterCandidates("degree rad");
   AddRotationAroundAnAxisCmd->SetParameter(dirX_param);
   AddRotationAroundAnAxisCmd->SetParameter(dirY_param);
   AddRotationAroundAnAxisCmd->SetParameter(dirZ_param);
   AddRotationAroundAnAxisCmd->SetParameter(angle_param);
   AddRotationAroundAnAxisCmd->SetParameter(angle_unit_param);
   
   
   AddTranslationCmd = new G4UIcmdWith3VectorAndUnit("/CoordinateTransformation/AddTranslation",this);
   AddTranslationCmd->SetGuidance("Add a translation");
   AddTranslationCmd->SetUnitCategory("Length");
   
   
   MultiplyTransformationsCmd = new G4UIcommand("/CoordinateTransformation/MultiplyTransformations",this);
   G4UIparameter* name_param = new G4UIparameter("Transformation Name",'s',false);
   MultiplyTransformationsCmd->SetGuidance("Multiply two transformations");
   MultiplyTransformationsCmd->SetParameter(name_param);
   MultiplyTransformationsCmd->SetParameter(name_param);
   
   
   MultiplyByCmd = new G4UIcmdWithAString("/CoordinateTransformation/MultiplyBy",this);
   MultiplyByCmd->SetParameterName("Transformation Name",false);
   MultiplyByCmd->SetGuidance("Multiply by a transformation");
   
   SaveTransformationCmd = new G4UIcmdWithAString("/CoordinateTransformation/Register",this);
   SaveTransformationCmd->SetParameterName("Transformation Name",false);
   SaveTransformationCmd->SetGuidance("Register a transformation");
   
   ClearListOfTransformationsCmd = new G4UIcmdWithoutParameter("/CoordinateTransformation/ClearAllTransformations",this);
   SaveTransformationCmd->SetGuidance("Clear all registered transformation");
   
 
  
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
TransManMessenger::~TransManMessenger()
{ 
}		  
////////////////////////////////////////////////////////////////////////////////////////////////
//
void TransManMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{  if (command == SelectTransformationCmd){
	theTransManager->SelectTransformation(newValues);
   }
   else if (command ==  ResetToUnitaryCmd){
   	theTransManager->ResetToUnitary();
   }
   else if (command ==AddRotationAroundXCmd){
   	theTransManager->AddRotationAroundX(AddRotationAroundXCmd->GetNewDoubleValue(newValues));
   }
   else if (command ==AddRotationAroundYCmd){
   	theTransManager->AddRotationAroundY(AddRotationAroundYCmd->GetNewDoubleValue(newValues));
   }
   else if (command ==AddRotationAroundZCmd){
   	theTransManager->AddRotationAroundZ(AddRotationAroundZCmd->GetNewDoubleValue(newValues));
   }
   else if (command ==AddRotationAroundAnAxisCmd){
   	const char* paramString=newValues;
     	G4double dirX, dirY, dirZ, angle;
	G4String angle_unit;
     	std::istringstream is((char*)paramString);
     	is>>dirX>>dirY>>dirZ>>angle>>angle_unit;
	angle*=G4UnitDefinition::GetValueOf(angle_unit);
	theTransManager->AddRotationAroundAnAxis(angle, G4ThreeVector(dirX,dirY,dirZ));
	
	
   }
   else if (command == AddTranslationCmd){
   	theTransManager->AddTranslation(AddTranslationCmd->GetNew3VectorValue(newValues));
	
   }
   else if (command == MultiplyTransformationsCmd){
   	const char* paramString=newValues;
	G4String name1,name2;
     	std::istringstream is((char*)paramString);
     	is>>name1>>name2;
	theTransManager->MultiplyTransformations(name1,name2);
   	
   }	
   else if (command == MultiplyByCmd){
   	theTransManager->MultiplyBy(newValues);
   }
   else if (command == SaveTransformationCmd){
   	theTransManager->SaveTransformation(newValues);
   }
   else if (command == ClearListOfTransformationsCmd){
   	theTransManager->ClearListOfTransformations();
   }
   
 
  
	    				      
}












