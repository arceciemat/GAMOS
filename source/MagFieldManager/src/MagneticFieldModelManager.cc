///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#include"MagneticFieldModelManager.hh"
#include"MagFieldModManMessenger.hh"
#include"ParametrizedFieldCreator.hh"
#include"UniformFieldCreator.hh"
#include"MagneticDipoleCreator.hh"
#include"CompoundFieldCreator.hh"
#include"CircularCoilCreator.hh"
#include"RacetrackCoilCreator.hh"
#include"CoilType2Creator.hh"
#include"CoilType3Creator.hh"
#include"IdealToroidWithRectangularCoilCreator.hh"
#include"IdealToroidWithCircularCoilCreator.hh"


#include"ReplicateFieldCreator.hh"
#include"LinearSegmentCreator.hh"
#include"InterpolatedFieldCreator.hh"
//#include"CurrentLoopCreator.hh"
#include"CurvedSegmentCreator.hh"
#include "ConfinedFieldCreator.hh"


#include"MultiplyFieldCreator.hh"
#include"CircularSolenoidCreator.hh"


#include"TransformationManager.hh"

//Needed for Integration in Bfield 

#include "G4MagIntegratorStepper.hh"
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4ClassicalRK4.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4Mag_EqRhs.hh"
#include "MagneticEquationOfMotion.hh"


//JCurrent
#include "JCurrentCalculator.hh" 

#include <fstream>
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Geometry/BasicVector3D.h"

using namespace CLHEP;
 
MagneticFieldModelManager* MagneticFieldModelManager::instance = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
//
MagneticFieldModelManager::MagneticFieldModelManager()
{ 
  
  G4cout<<"//////////////////////////////////////////////////////"<<std::endl;
  G4cout<<"//Magnetic Field Model Manager                      //"<<std::endl;
  G4cout<<"//Developed by L. Desorgher (SpaceIT GmbH) for ESA  //"<<std::endl;
  G4cout<<"//////////////////////////////////////////////////////"<<std::endl; 
  
  theMessenger = new MagFieldModManMessenger(this);
  
  
  
  AddParametrizedFieldCreator(new UniformFieldCreator());
  //AddParametrizedFieldCreator(new MagneticDipoleCreator());
  AddParametrizedFieldCreator(new CompoundFieldCreator());
  AddParametrizedFieldCreator(new MultiplyFieldCreator());
 
 // AddParametrizedFieldCreator(new CircularSolenoidCreator());
 // AddParametrizedFieldCreator(new CircularCoilCreator());
  AddParametrizedFieldCreator(new ReplicateFieldCreator());
  AddParametrizedFieldCreator(new RacetrackCoilCreator());
  AddParametrizedFieldCreator(new CoilType2Creator());
  AddParametrizedFieldCreator(new CoilType3Creator());
  
  AddParametrizedFieldCreator(new LinearSegmentCreator());
  AddParametrizedFieldCreator(new CurvedSegmentCreator());
  
  AddParametrizedFieldCreator(new InterpolatedFieldCreator());
  //AddParametrizedFieldCreator(new CurrentLoopCreator()); 
  AddParametrizedFieldCreator(new IdealToroidWithRectangularCoilCreator());
  AddParametrizedFieldCreator(new IdealToroidWithCircularCoilCreator());
  AddParametrizedFieldCreator(new ConfinedFieldCreator());
  
  
  theTransformationManager = TransformationManager::GetInstance();
  
  
  LengthScale = 1.*m;
  fGlobalStepper =0;
  fGlobalChordFinder  =0;
  RecomputeDefaultParametersFromLengthScale();
  DeltaChord =DefaultDeltaChord;
  theGlobalField =0;
  fEquationOfMotion = 0;
  UserSelectedStepperName ="CashKarpRKF45";



 

}
///////////////////////////////////////////////////////////////////////
MagneticFieldModelManager::~MagneticFieldModelManager()
{RemoveGlobalField();
 for (size_t i=0;i<listOfCreatedFields.size();i++){
	 delete (listOfCreatedFields[i]);
 }
 listOfCreatedFields.clear();
}
////////////////////////////////////////////////////////////////////////////////
//
MagneticFieldModelManager* MagneticFieldModelManager::GetInstance()
{ 
  if (!instance) instance = new MagneticFieldModelManager();
  return instance;
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::SelectGlobalField(G4String field_name)
{ 
  G4bool field_have_been_found = false;
  for (size_t i=0;i<listOfCreatedFieldNames.size();i++){
  	if (field_name == listOfCreatedFieldNames[i] && !field_have_been_found){
		field_have_been_found = true;
		theGlobalField=listOfCreatedFields[i];
		G4TransportationManager::GetTransportationManager()->GetFieldManager()->SetDetectorField(theGlobalField);
		if (!fEquationOfMotion) fEquationOfMotion= new MagneticEquationOfMotion(theGlobalField);
		else fEquationOfMotion->SetFieldObj(theGlobalField);
		SetStepper(UserSelectedStepperName, "Global");
	}	
  }
  
 
  if (!field_have_been_found){
  	std::cout<<"Warning! The field that you wanted to set as the gobal field does not  exist."<<std::endl;
  }
  
 	
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::RemoveGlobalField()
{ theGlobalField =0;
  G4TransportationManager::GetTransportationManager()->GetFieldManager()->SetDetectorField(theGlobalField);
}	  
///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::AddField(G4MagneticField* aField,G4String field_name)
{ G4bool field_can_be_added = true;
  for (size_t i=0;i<listOfCreatedFieldNames.size();i++){
  	if (field_name == listOfCreatedFieldNames[i] || aField == listOfCreatedFields[i])
		field_can_be_added =false;
  }
  
  if (field_can_be_added){
	listOfCreatedFieldNames.push_back(field_name);
	listOfCreatedFields.push_back(aField);
  }
  else {
  	std::cout<<"Warning ! This field will not be added to the list of field that can be used."<<std::endl; 
  }
 	
}
///////////////////////////////////////////////////////////////////////
//
G4MagneticField* MagneticFieldModelManager::GetCreatedField(G4String field_name)
{ for (size_t i=0;i<listOfCreatedFieldNames.size();i++){
  	if (field_name == listOfCreatedFieldNames[i]){
		return listOfCreatedFields[i];
	}	
  }
  
  return 0;
}


///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::AddParametrizedFieldCreator(ParametrizedFieldCreator* aCreator)
{ G4bool creator_can_be_added = true;
   if (!aCreator) {
  	creator_can_be_added = false;
  }
  else {
        
  	for (size_t i=0; i<listOfParametrizedFieldCreators.size();i++){ 
		if (aCreator->GetModelName() == listOfParametrizedFieldCreators[i]->GetModelName()){
			creator_can_be_added = false;
		};
	}	
  }
  if (creator_can_be_added){
  	listOfParametrizedFieldCreators.push_back(aCreator);
	aCreator->CreateMacroCommands(G4String("/MagneticFieldModels"),theMessenger);
	
  }
  else {
  	std::cout<<"Warning ! This field creator will not be added to the list of field creators."<<std::endl; 
  }
   
  
}

///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::CreateInstanceOfParametrizedField(G4String aModelName, G4String aFieldName)
{ G4bool did_find_the_model = false;
  for (size_t i=0; i<listOfParametrizedFieldCreators.size();i++){
	if (aModelName == listOfParametrizedFieldCreators[i]->GetModelName()){
		did_find_the_model =true;
		AddField(listOfParametrizedFieldCreators[i]->CreateNewField(),aFieldName);
		break;
	} 
  } 
  if (!did_find_the_model){
  	std::cout<<"Warning! A problem occured when trying to create a parametrized field."<<std::endl; 
  }
}


///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::CalculateJCurrentBetweenTwoPositions(G4String FieldName, G4ThreeVector pos1,G4ThreeVector pos2,G4double t, G4int npos,G4double ds, G4String output_file_name)
{ G4MagneticField* aField = GetCreatedField(FieldName);
  if (! aField){
  	std::cout<<"Warning! The field that you wanted to select does not  exist."<<std::endl;
	return;
  }
  JCurrentCalculator* theCurrentCalculator = JCurrentCalculator::GetInstance();
  theCurrentCalculator->SetField(aField);
  theCurrentCalculator->CalculateJCurrentBetweenTwoPositions(pos1,pos2,t, npos, ds, output_file_name);
	
  
}  
///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::UseDefaultIntegrationParameters(G4String vol_name)
{ SetEpsilon(DefaultEpsilon,vol_name);
  SetDeltaChord(DefaultDeltaChord,vol_name);
  SetDeltaIntersection(DefaultDeltaIntersection,vol_name);
  SetStepper(G4String("CashKarper"),vol_name);
  
}
///////////////////////////////////////////////////////////////////////
//

void MagneticFieldModelManager::SetLengthScale(G4double aVal)
{ LengthScale =aVal;
  RecomputeDefaultParametersFromLengthScale();
  	
}
///////////////////////////////////////////////////////////////////////
//LinearSegment
void MagneticFieldModelManager::RecomputeDefaultParametersFromLengthScale()
{ DefaultDeltaChord=.1*LengthScale;
  DefaultDeltaIntersection=0.01*LengthScale;
  DefaultEpsilon= 1.e-6;
}
///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::SetEpsilon(G4double aVal,G4String )
{//precision for G4Integration methods
  G4TransportationManager::GetTransportationManager()
                                  ->GetPropagatorInField()
                                  ->SetMinimumEpsilonStep(aVal);
  G4TransportationManager::GetTransportationManager()
                                  ->GetPropagatorInField()
                                  ->SetMaximumEpsilonStep(aVal*1.00001);
}
///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::SetDeltaChord(G4double aVal,G4String )
{ if (fGlobalChordFinder) fGlobalChordFinder->SetDeltaChord(aVal);
  DeltaChord=aVal;
}
///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::SetDeltaIntersection(G4double aVal,G4String)
{ //Delta intersection for G4Integration method
  G4TransportationManager::GetTransportationManager()->GetFieldManager()
                                                 ->SetDeltaIntersection(aVal);
}
///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::SetLargestAcceptableStep(G4double aVal,G4String )
{ G4TransportationManager::GetTransportationManager()->GetPropagatorInField()
	                        ->SetLargestAcceptableStep(aVal);
}

///////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::SetStepper(G4String aStepperName, G4String vol_name)
{ UserSelectedStepperName = aStepperName;
  if (vol_name=="Global" && theGlobalField){
  	
  	if (fGlobalStepper) delete fGlobalStepper;
   
  	if (aStepperName == "ExplicitEuler"){
    		fGlobalStepper = new G4ExplicitEuler( fEquationOfMotion );
     		G4cout<<"G4ExplicitEuler is called"<<G4endl;
  	}
  	else if  (aStepperName == "ImplicitEuler"){
    		fGlobalStepper = new G4ImplicitEuler( fEquationOfMotion );
     		G4cout<<"G4ImplicitEuler is called"<<G4endl;
  	}
  	else if  (aStepperName == "SimpleRunge"){
    		fGlobalStepper = new G4SimpleRunge( fEquationOfMotion );
     		G4cout<<"G4SimpleRunge is called"<<G4endl;
  	} 
  	else if  (aStepperName == "ClassicalRK4"){
    		fGlobalStepper = new G4ClassicalRK4( fEquationOfMotion );
     		G4cout<<"G4ClassicalRK4 is called"<<G4endl;
  	}  
  	else if  (aStepperName == "RKG3_Stepper"){
    		fGlobalStepper = new G4RKG3_Stepper( fEquationOfMotion );
     		G4cout<<"G4RKG3_Stepper is called"<<G4endl;
  	}
  	else if  (aStepperName == "CashKarpRKF45"){
    		fGlobalStepper = new G4CashKarpRKF45( fEquationOfMotion );
     		G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
  	}   
  	else {
    		fGlobalStepper = new G4CashKarpRKF45( fEquationOfMotion );
     		G4cout<<"The selected stepper is not available"<<G4endl;
     		G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
  	}
  
  	if (fGlobalChordFinder) delete fGlobalChordFinder;
  	G4double min_step= .000001*DeltaChord;
  	fGlobalChordFinder = new G4ChordFinder(theGlobalField,min_step,fGlobalStepper);
  	fGlobalChordFinder->SetDeltaChord(DeltaChord); 
  
  	G4FieldManager* fieldMgr= 
  	G4TransportationManager::GetTransportationManager()->GetFieldManager();
  	fieldMgr->SetChordFinder(fGlobalChordFinder);
   }	
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::CalculateJCurrentOnACartesianGrid(G4String FieldName,G4double x0,G4double  dx, G4int nX,
					 G4double y0,G4double  dy, G4int nY,
					 G4double z0,G4double  dz, G4int nZ,G4double ds,G4double t,
					 G4String file_output)
{ G4MagneticField* aField = GetCreatedField(FieldName);
  if (! aField){
  	std::cout<<"Warning! The field that you wanted to select does not  exist."<<std::endl;
	return;
  }
  JCurrentCalculator* theCurrentCalculator = JCurrentCalculator::GetInstance();
  theCurrentCalculator->SetField(aField);
  theCurrentCalculator->CalculateJCurrentOnACartesianGrid(x0,dx, nX,
					 y0, dy, nY, z0, dz,  nZ,ds, t,
					 file_output);
}

////////////////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::ComputeBfieldOnACartesianGrid(G4String FieldName,
				G4double x0,G4double  dx, G4int nX,
				G4double y0,G4double  dy, G4int nY,
				G4double z0,G4double  dz, G4int nZ,
				G4String file_output) 
{ 
  clock_t clock1,clock2;
  clock1=clock();
  
  G4MagneticField* aField = GetCreatedField(FieldName);
  if (! aField){
  	std::cout<<"Warning! The field that you wanted to select does not  exist."<<std::endl;
	return;
  }
  
  
  std::fstream OutputFile(file_output, std::ios::out);
  OutputFile.setf(std::ios::scientific);
  OutputFile<<G4String("Cartesian")<<std::endl;
  OutputFile<<nX<<'\t'<<nY<<'\t'<<nZ<<std::endl;
  OutputFile.precision(6);
  OutputFile<<'\t'<<"X[m] "<<'\t'<<"Y[m] "<<'\t'<<"Z[m] "
				  <<"Bx"<<'\t'<<"By"<<'\t'
				  <<"Bz"<<'\t'
				  <<"B [nT]"<<std::endl;
  G4double position[4],B[3];
  position[3] =0;
  
  
 
  for (double  x=x0; x< x0 + double(nX)*dx; x+=dx){
  	for (double  y=y0; y< y0 + double(nY)*dy; y+=dy){
		for (double  z=z0; z< z0 + double(nZ)*dz; z+=dz){
			
			position[0]=x;
			position[1]=y;
			position[2]=z;
			aField->GetFieldValue(position,B);
			G4ThreeVector Bfield = G4ThreeVector(B[0],B[1],B[2])/tesla;
			OutputFile<<x/m<<'\t'<<'\t'
				  <<y/m<<'\t'<<'\t'
				  <<z/m<<'\t'<<'\t'
				  <<Bfield.x()<<'\t'<<Bfield.y()<<'\t'
				  <<Bfield.z()<<'\t'<<Bfield.mag()<<'\t'
				  <<std::endl;	              	
		}
	}
  
  }
  
  OutputFile.close();
   
  clock2=clock();
  double tclock=double(clock2-clock1)/(double(CLOCKS_PER_SEC));
  G4cout<<"time used for the computation: "<<tclock<<" s"<<std::endl;
  
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticFieldModelManager::ComputeBfieldOnACylindricalGrid(G4String FieldName,
				G4double phi0,G4double  dPhi, G4int nPhi,
				G4double r0,G4double  dr, G4int nR,
				G4double z0,G4double  dz, G4int nZ,
				G4String file_output)
{ 
  clock_t clock1,clock2;
  clock1=clock();
  
  G4MagneticField* aField = GetCreatedField(FieldName);
  if (! aField){
  	std::cout<<"Warning! The field that you wanted to select does not  exist."<<std::endl;
	return;
  }
  
  
  std::fstream OutputFile(file_output, std::ios::out);
  OutputFile.setf(std::ios::scientific); 
  OutputFile<<G4String("Cylindrical")<<std::endl;
  OutputFile<<nR<<'\t'<<nPhi<<'\t'<<nZ<<std::endl;
  OutputFile.precision(6);
  OutputFile<<"R[m] "<<'\t'<<"Phi[rad] "<<'\t'<<"Z[m] "<<'\t'
				  <<"Br"<<'\t'<<"Bphi"<<'\t'
				  <<"Bz"<<'\t'
				  <<"B [nT]"<<std::endl;
  G4double position[4],B[3];
  position[3] =0;
  
  
  G4double cphi,sphi;
  G4double Br,Bphi;
  G4double z,phi,r;
  
  for (int  i=0; i<nR;i++){
  	r= r0 + double(i)*dr;
  	for (int j=0;j<nPhi;j++){
		phi= phi0 + double(j)*dPhi;
		cphi=std::cos(phi);
  		sphi=std::sin(phi);
  		for (int k=0;k<nZ;k++){
			z= z0 + double(k)*dz;
			position[0]=r*cphi;
			position[1]=r*sphi;
			position[2]=z;
			aField->GetFieldValue(position,B);
			G4ThreeVector Bfield = G4ThreeVector(B[0],B[1],B[2])/tesla;
			Br = cphi*Bfield.x()+sphi*Bfield.y();
			Bphi = cphi*Bfield.y()-sphi*Bfield.x();
			OutputFile<<r/m<<'\t'<<'\t'
				  <<phi<<'\t'<<'\t'
				  <<z/m<<'\t'<<'\t'
				  <<Br<<'\t'<<Bphi<<'\t'
				  <<Bfield.z()<<'\t'<<Bfield.mag()<<'\t'
				  <<std::endl;	              	
		}
  
  	}
  }	
  
  OutputFile.close();
   
  clock2=clock();
  double tclock=double(clock2-clock1)/(double(CLOCKS_PER_SEC));
  G4cout<<"time used for the computation: "<<tclock<<" s"<<std::endl;
  
}

