#include"MagFieldModManMessenger.hh"
#include"MagneticFieldModelManager.hh"
#include"BiotSavartIntegrator.hh"
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

MagFieldModManMessenger::MagFieldModManMessenger(MagneticFieldModelManager* aMagFieldModManager)
{ theMagFieldModManager = aMagFieldModManager;
  
  
  MagFieldModManagerDir = new G4UIdirectory("/MagneticFieldModels/");
  MagFieldModManagerDir->SetGuidance("Definition of magnetic fields");
  
  G4UIparameter* model_name_param = new G4UIparameter("ModelName",'s',false);
  G4UIparameter* field_name_param = new G4UIparameter("FieldName",'s',false);
  
  
  CreateAParametrizedFieldCmd  = new G4UIcommand("/MagneticFieldModels/CreateAParametrizedField",this);
  CreateAParametrizedFieldCmd->SetGuidance("Create a parametrized field");
  CreateAParametrizedFieldCmd->SetParameter(model_name_param);
  CreateAParametrizedFieldCmd->SetParameter(field_name_param);
  CreateAParametrizedFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetGlobalFieldCmd = new G4UIcmdWithAString("/MagneticFieldModels/SelectGlobalField",this);
  SetGlobalFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  RemoveGlobalFieldCmd = new G4UIcmdWithoutParameter("/MagneticFieldModels/RemoveGlobalField",this);
  RemoveGlobalFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  G4UIparameter* output_file_name_param = new G4UIparameter("OutputFileName",'s',false);
  G4UIparameter* Xpos_param = new G4UIparameter("Xpos",'d',false);
  G4UIparameter* Ypos_param = new G4UIparameter("Ypos",'d',false);
  G4UIparameter* Zpos_param = new G4UIparameter("Zpos",'d',false);
  G4UIparameter* ds_param = new G4UIparameter("ds",'d',false);
  G4UIparameter* length_unit_param = new G4UIparameter("LengthUnit",'s',false);
  G4UIparameter* angle_unit_param = new G4UIparameter("AngleUnit",'s',false);
  G4UIparameter* npos_param = new G4UIparameter("Npos",'i',false);
  
  ComputeJCurrentBetweenTwoPositionsCmd  = new G4UIcommand("/MagneticFieldModels/ComputeJCurrentBetweenTwoPositions",this);
  ComputeJCurrentBetweenTwoPositionsCmd->SetGuidance("Compute the current between two positions for a given  field model"); 
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(field_name_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(Xpos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(Ypos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(Zpos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(Xpos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(Ypos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(Zpos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(ds_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(length_unit_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(npos_param);
  ComputeJCurrentBetweenTwoPositionsCmd->SetParameter(output_file_name_param);
  
  G4UIparameter* Phi0_param = new G4UIparameter("Phi0",'d',false);
  G4UIparameter* X0_param = new G4UIparameter("X0",'d',false);
  G4UIparameter* Y0_param = new G4UIparameter("Y0",'d',false);
  G4UIparameter* Z0_param = new G4UIparameter("Z0",'d',false);
  G4UIparameter* R0_param = new G4UIparameter("R0",'d',false);
  
  G4UIparameter* dX_param = new G4UIparameter("dX",'d',false);
  G4UIparameter* dY_param = new G4UIparameter("dY",'d',false);
  G4UIparameter* dZ_param = new G4UIparameter("dZ",'d',false);
  G4UIparameter* dR_param = new G4UIparameter("dR",'d',false);
  G4UIparameter* dPhi_param = new G4UIparameter("dPhi",'d',false);
  
  G4UIparameter* nX_param = new G4UIparameter("nX",'i',false);
  G4UIparameter* nY_param = new G4UIparameter("nY",'i',false);
  G4UIparameter* nZ_param = new G4UIparameter("nZ",'i',false);
  G4UIparameter* nR_param = new G4UIparameter("nR",'i',false);
  G4UIparameter* nPhi_param = new G4UIparameter("nPhi",'i',false);
  
  ComputeJCurrentOnACartesianGridCmd = new G4UIcommand("/MagneticFieldModels/ComputeJCurrentOnACartesianGrid",this);
  ComputeJCurrentOnACartesianGridCmd->SetGuidance("Compute the curret density on a grid for a given  field model"); 
  ComputeJCurrentOnACartesianGridCmd->SetParameter(field_name_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(X0_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(dX_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(nX_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(Y0_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(dY_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(nY_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(Z0_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(dZ_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(nZ_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(ds_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(length_unit_param);
  ComputeJCurrentOnACartesianGridCmd->SetParameter(output_file_name_param);
  ComputeJCurrentOnACartesianGridCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  
  
  
  ComputeBfieldOnACartesianGridCmd = new G4UIcommand("/MagneticFieldModels/ComputeBfieldOnACartesianGrid",this);
  ComputeBfieldOnACartesianGridCmd->SetGuidance("Compute the field on a grid for a given  field model"); 
  ComputeBfieldOnACartesianGridCmd->SetParameter(field_name_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(X0_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(dX_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(nX_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(Y0_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(dY_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(nY_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(Z0_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(dZ_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(nZ_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(length_unit_param);
  ComputeBfieldOnACartesianGridCmd->SetParameter(output_file_name_param);
  ComputeBfieldOnACartesianGridCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  ComputeBfieldOnACylindricalGridCmd = new G4UIcommand("/MagneticFieldModels/ComputeBfieldOnACylindricalGrid",this);
  ComputeBfieldOnACylindricalGridCmd->SetGuidance("Compute the field on a grid for a given  field model"); 
  ComputeBfieldOnACylindricalGridCmd->SetParameter(field_name_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(Phi0_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(dPhi_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(nPhi_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(angle_unit_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(R0_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(dR_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(nR_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(Z0_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(dZ_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(nZ_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(length_unit_param);
  ComputeBfieldOnACylindricalGridCmd->SetParameter(output_file_name_param);
  ComputeBfieldOnACylindricalGridCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  
  
 /* SetLinearSegmentType1Cmd = new G4UIcmdWithABool("/MagneticFieldModels/BiotSavartIntegrator/SetLinearSegmentType1",this);
  SetLinearSegmentType1Cmd->SetGuidance("Choose the type of Linear Segment integration"); 
  SetLinearSegmentType1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);*/
  
 
  
  SetNxForBiotSavartIntegrationCmd = new G4UIcmdWithAnInteger("/MagneticFieldModels/BiotSavartIntegrator/SetNx",this);
  SetNxForBiotSavartIntegrationCmd->SetGuidance("Set the number of step for numerical integration along X"); 
  SetNxForBiotSavartIntegrationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetNyForBiotSavartIntegrationCmd = new G4UIcmdWithAnInteger("/MagneticFieldModels/BiotSavartIntegrator/SetNy",this);
  SetNyForBiotSavartIntegrationCmd->SetGuidance("Set the number of step for numerical integration along Y");
  SetNyForBiotSavartIntegrationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SetNphiForBiotSavartIntegrationCmd = new G4UIcmdWithAnInteger("/MagneticFieldModels/BiotSavartIntegrator/SetNphi",this);
  SetNphiForBiotSavartIntegrationCmd->SetGuidance("Set the number of step for numerical over the azimuth phi");
  SetNphiForBiotSavartIntegrationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  SetCurvedSegmentTypeOfIntegrationCmd = new G4UIcmdWithAnInteger("/MagneticFieldModels/BiotSavartIntegrator/SetCurvedSegmentType",this);
  SetCurvedSegmentTypeOfIntegrationCmd->SetGuidance("Set the  type of curved segment");
  SetCurvedSegmentTypeOfIntegrationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  
  //Definition of integration parameters for the Lorentz equation
  //--------------------------------------------------------------
  
  SetEpsilonCmd = new G4UIcmdWithADouble("/LorentzEquationIntegrator/SetPrecision",this);
  SetEpsilonCmd->SetGuidance("Set the relative precision for integration of the Lorentz equation motion");
  SetEpsilonCmd->SetParameterName("precision",false);
  SetEpsilonCmd->SetRange("precision <= 1.e-3  && precision >=1.e-8");
  SetEpsilonCmd->AvailableForStates(G4State_PreInit,G4State_Idle); 
  
  
  SetDeltaChordCmd = new G4UIcmdWithADoubleAndUnit("/LorentzEquationIntegrator/SetMaxStep",this);
  SetDeltaChordCmd->SetGuidance("Set the maximal integrating step allowed for G4 integration");
  SetDeltaChordCmd->SetParameterName("MaxStep",false);
  SetDeltaChordCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDeltaChordCmd->SetRange("MaxStep > 0.");
  SetDeltaChordCmd->SetUnitCategory("Length");
  
  SetDeltaIntersectionCmd = new G4UIcmdWithADoubleAndUnit("/LorentzEquationIntegrator/SetDeltaIntersection",this);
  SetDeltaIntersectionCmd->SetGuidance("Set the precision for crossing boundary");
  SetDeltaIntersectionCmd->SetParameterName("DeltaIntersection",false);
  SetDeltaIntersectionCmd->SetRange("DeltaIntersection > 0.");
  SetDeltaIntersectionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetDeltaIntersectionCmd->SetUnitCategory("Length");
  
  SetLargestAcceptableStepCmd = new G4UIcmdWithADoubleAndUnit("/LorentzEquationIntegrator/SetLargestAcceptableStep",this);
  SetLargestAcceptableStepCmd->SetGuidance("Set the laegest acceptable step for integration");
  SetLargestAcceptableStepCmd->SetParameterName("LargestAcceptableStep",false);
  SetDeltaIntersectionCmd->SetRange("LargestAcceptableStep > 0.");
  SetLargestAcceptableStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetLargestAcceptableStepCmd->SetUnitCategory("Length");
  
  

  
  SetStepperCmd = new
                G4UIcmdWithAString("/LorentzEquationIntegrator/SetStepperModel",this);
  SetStepperCmd
      ->SetGuidance("Set the stepper model for the G4Integration method ");
  SetStepperCmd->SetParameterName("choice",true);
  SetStepperCmd->SetDefaultValue("CashKarpRKF45");
  SetStepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  G4String candidates = "ExplicitEuler ImplicitEuler SimpleRunge ClassicalRK4 ";
  candidates += "CashKarpRKF45 RKG3_Stepper";
  SetStepperCmd->SetCandidates(candidates);
  
  
  
  
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
MagFieldModManMessenger::~MagFieldModManMessenger()
{ delete MagFieldModManagerDir;
  delete CreateAParametrizedFieldCmd;
  delete SetGlobalFieldCmd;
  delete RemoveGlobalFieldCmd;
  delete ComputeJCurrentBetweenTwoPositionsCmd;
  delete ComputeJCurrentOnACartesianGridCmd;
  delete ComputeBfieldOnACartesianGridCmd;
  delete ComputeBfieldOnACylindricalGridCmd;
  
  delete SetEpsilonCmd;
  delete SetDeltaChordCmd;
  delete SetDeltaIntersectionCmd;
  delete SetLargestAcceptableStepCmd;
  delete SetStepperCmd;
}		  
////////////////////////////////////////////////////////////////////////////////////////////////
//
void MagFieldModManMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{ 
  std::vector<ParametrizedFieldCreator*> theListOfParametrizedFieldCreators = theMagFieldModManager->GetListOfParametrizedFieldCreators();
  G4bool did_find_a_cmd = false;
  for (size_t i=0;i<theListOfParametrizedFieldCreators.size();i++){
  	did_find_a_cmd = theListOfParametrizedFieldCreators[i]->DefineParameters(command, newValues);
  
  }
  if (!did_find_a_cmd){
  	if (command == CreateAParametrizedFieldCmd){
		const char* paramString=newValues;
     		G4String aModelName, aFieldName;
     		std::istringstream is((char*)paramString);
     		is>>aModelName>>aFieldName;
		theMagFieldModManager->CreateInstanceOfParametrizedField(aModelName,aFieldName);
	}
	else if (command == SetGlobalFieldCmd){
		theMagFieldModManager->SelectGlobalField(newValues);
	}
	else if (command == RemoveGlobalFieldCmd){
		theMagFieldModManager->RemoveGlobalField();
	}
	else if (command == ComputeJCurrentBetweenTwoPositionsCmd){
		const char* paramString=newValues;
		G4String aFieldName, output_file_name;
		G4double X1,Y1,Z1,X2,Y2,Z2,ds;
		G4String length_unit;
		G4int npos;
		std::istringstream is((char*)paramString);
     		is>>aFieldName>>X1>>Y1>>Z1>>X2>>Y2>>Z2>>ds>>length_unit>>npos>>output_file_name;
		G4ThreeVector pos1=G4ThreeVector(X1,Y1,Z1)*G4UnitDefinition::GetValueOf(length_unit);
		G4ThreeVector pos2=G4ThreeVector(X2,Y2,Z2)*G4UnitDefinition::GetValueOf(length_unit);
		ds *=G4UnitDefinition::GetValueOf(length_unit);
		theMagFieldModManager->CalculateJCurrentBetweenTwoPositions(aFieldName,pos1,
  					  pos2,0.,npos,ds, output_file_name);
		
	}
	else if (command == ComputeJCurrentOnACartesianGridCmd){
		const char* paramString=newValues;
		G4String aFieldName, output_file_name;
		G4double X0,Y0,Z0,dX,dY,dZ,ds;
		G4String length_unit;
		G4int nX,nY,nZ;
		std::istringstream is((char*)paramString);
     		is>>aFieldName>>X0>>dX>>nX>>Y0>>dY>>nY>>Z0>>dZ>>nZ>>ds>>length_unit>>output_file_name;
		X0*=G4UnitDefinition::GetValueOf(length_unit);
		Y0*=G4UnitDefinition::GetValueOf(length_unit);
		Z0*=G4UnitDefinition::GetValueOf(length_unit);
		dX*=G4UnitDefinition::GetValueOf(length_unit);
		dY*=G4UnitDefinition::GetValueOf(length_unit);
		dZ*=G4UnitDefinition::GetValueOf(length_unit);
		ds*=G4UnitDefinition::GetValueOf(length_unit);
		theMagFieldModManager->CalculateJCurrentOnACartesianGrid(aFieldName,X0,dX,nX,
										    Y0,dY,nY,
										    Z0,dZ,nZ,
										    ds,0.,output_file_name);
	}
	else if (command == ComputeBfieldOnACartesianGridCmd){
		const char* paramString=newValues;
		G4String aFieldName, output_file_name;
		G4double X0,Y0,Z0,dX,dY,dZ;
		G4String length_unit;
		G4int nX,nY,nZ;
		std::istringstream is((char*)paramString);
     		is>>aFieldName>>X0>>dX>>nX>>Y0>>dY>>nY>>Z0>>dZ>>nZ>>length_unit>>output_file_name;
		X0*=G4UnitDefinition::GetValueOf(length_unit);
		Y0*=G4UnitDefinition::GetValueOf(length_unit);
		Z0*=G4UnitDefinition::GetValueOf(length_unit);
		dX*=G4UnitDefinition::GetValueOf(length_unit);
		dY*=G4UnitDefinition::GetValueOf(length_unit);
		dZ*=G4UnitDefinition::GetValueOf(length_unit);
		theMagFieldModManager->ComputeBfieldOnACartesianGrid(aFieldName,X0,dX,nX,Y0,dY,nY,Z0,dZ,nZ,output_file_name);
	}
	else if (command == ComputeBfieldOnACylindricalGridCmd){
		const char* paramString=newValues;
		G4String aFieldName, output_file_name;
		G4double Phi0,R0,Z0,dR,dZ,dPhi;
		G4String length_unit, angle_unit;
		G4int nR,nZ,nPhi;
		std::istringstream is((char*)paramString);
     		is>>aFieldName>>Phi0>>dPhi>>nPhi>>angle_unit>>R0>>dR>>nR>>Z0>>dZ>>nZ>>length_unit>>output_file_name;
		Phi0*=G4UnitDefinition::GetValueOf(angle_unit);
		dPhi*=G4UnitDefinition::GetValueOf(angle_unit);
		R0*=G4UnitDefinition::GetValueOf(length_unit);
		Z0*=G4UnitDefinition::GetValueOf(length_unit);
		dR*=G4UnitDefinition::GetValueOf(length_unit);
		dZ*=G4UnitDefinition::GetValueOf(length_unit);
		theMagFieldModManager->ComputeBfieldOnACylindricalGrid(aFieldName,Phi0,dPhi,nPhi,R0,dR,nR,Z0,dZ,nZ,output_file_name);
	}
	
	
	/*else if  (command == SetLinearSegmentType1Cmd){
		BiotSavartIntegrator::GetInstance()->SetLinearSegmentType1(SetLinearSegmentType1Cmd->GetNewBoolValue(newValues));
	}*/
	
	else if  (command == SetNxForBiotSavartIntegrationCmd){
		BiotSavartIntegrator::GetInstance()->SetNx(SetNxForBiotSavartIntegrationCmd->GetNewIntValue(newValues));
	}
	else if  (command == SetNyForBiotSavartIntegrationCmd){
		BiotSavartIntegrator::GetInstance()->SetNy(SetNyForBiotSavartIntegrationCmd->GetNewIntValue(newValues));
	}
	else if  (command == SetNphiForBiotSavartIntegrationCmd){
		BiotSavartIntegrator::GetInstance()->SetNphi(SetNphiForBiotSavartIntegrationCmd->GetNewIntValue(newValues));
	}
	else if  (command == SetCurvedSegmentTypeOfIntegrationCmd){
		BiotSavartIntegrator::GetInstance()->SetCurvedSegmentTypeOfIntegration(SetCurvedSegmentTypeOfIntegrationCmd->GetNewIntValue(newValues));
	}
	
	else if (command == SetEpsilonCmd)
              	theMagFieldModManager->SetEpsilon(SetEpsilonCmd->GetNewDoubleValue(newValues)); 

  	else if (command == SetDeltaChordCmd) 
              	theMagFieldModManager->SetDeltaChord(SetDeltaChordCmd->GetNewDoubleValue(newValues));  
	      
        else if (command == SetDeltaIntersectionCmd) 
              	theMagFieldModManager->SetDeltaIntersection(SetDeltaIntersectionCmd->GetNewDoubleValue(newValues));   
  	
	else if ( command == SetStepperCmd )
            	theMagFieldModManager->SetStepper(newValues);			
	
  
  
  }
  
	    				      
}












