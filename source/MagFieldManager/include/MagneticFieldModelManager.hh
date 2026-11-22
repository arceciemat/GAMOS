///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef MagneticFieldModelManager_h
#define MagneticFieldModelManager_h 1

#include"globals.hh"
#include"G4ios.hh"
#include"G4MagneticField.hh"
#include<vector> 
#include"G4ThreeVector.hh"

class MagFieldModManMessenger;
class ParametrizedFieldCreator;

class G4ChordFinder;
class G4MagIntegratorStepper;
class G4Mag_EqRhs;
class TransformationManager;



class MagneticFieldModelManager 
{ public:
  
   MagneticFieldModelManager();
   virtual ~MagneticFieldModelManager();
   
   static MagneticFieldModelManager* GetInstance();
   
   
  public: 
  
  
  //field
   
  void SelectGlobalField(G4String field_name);
  void RemoveGlobalField();
   //void SelectLocalField(G4String volume_name, G4String field_name);
  void AddField(G4MagneticField*,G4String field_name);
  G4MagneticField* GetCreatedField(G4String field_name);
   
   
  //Parametrised Fields
  void CreateInstanceOfParametrizedField(G4String aModelName, G4String aFieldName);
  void AddParametrizedFieldCreator(ParametrizedFieldCreator* aCreator);
  inline std::vector<ParametrizedFieldCreator*> GetListOfParametrizedFieldCreators(){return listOfParametrizedFieldCreators;}
  
  //compute current
  
   void CalculateJCurrentBetweenTwoPositions(G4String FieldName, G4ThreeVector pos1,
  					     G4ThreeVector pos2, G4double t,  G4int npos, G4double ds, 
					     G4String output_file_name);
   
   void CalculateJCurrentOnACartesianGrid(G4String FieldName,G4double x0,G4double  dx, G4int nX,
					 G4double y0,G4double  dy, G4int nY,
					 G4double z0,G4double  dz, G4int nZ,G4double ds,G4double t,
					 G4String file_output);
   
  
   void ComputeBfieldOnACartesianGrid(G4String FieldName,
				G4double x0,G4double  dx, G4int nX,
				G4double y0,G4double  dy, G4int nY,
				G4double z0,G4double  dz, G4int nZ,
				G4String file_output);
  
   void ComputeBfieldOnACylindricalGrid(G4String FieldName,
				G4double phi0,G4double  dPhi, G4int nPhi,
				G4double r0,G4double  dr, G4int nR,
				G4double z0,G4double  dz, G4int nZ,
				G4String file_output);
				
   				
  
  //Integration parameters 
   void UseDefaultIntegrationParameters(G4String vol_name="Global");
   void SetLengthScale(G4double aVal);
   void RecomputeDefaultParametersFromLengthScale();
   void SetEpsilon(G4double aVal,G4String vol_name="Global");
   void SetDeltaChord(G4double aVal,G4String vol_name="Global");
   void SetDeltaIntersection(G4double aVal,G4String vol_name="Global");
   void SetLargestAcceptableStep(G4double aVal,G4String vol_name="Global");
   void SetStepper(G4String, G4String vol_name="Global");
   
   
   //Equation Of Motion
   inline G4Mag_EqRhs* GetEquationOfMotion(){return fEquationOfMotion;}
   void SetEquationOfMotion(G4Mag_EqRhs* anEquation){fEquationOfMotion = anEquation;}
 
 
 private:
 
   static MagneticFieldModelManager* instance;
 
 //attributes
   
   
   //IntegrationParameters
   
   G4double DefaultDeltaChord;
   G4double DefaultDeltaIntersection;
   G4double DefaultEpsilon;
   G4double DefaultLargestAcceptableStep;
   G4double DeltaChord;
   
   G4double LengthScale; //when changing LengthScale all the default integration parameter will change
   
   
   //EquationOfMotion
   G4Mag_EqRhs* fEquationOfMotion;
   
   
   //Global field 
   G4MagneticField* theGlobalField;
   G4String theGlobalModelName;
   G4MagIntegratorStepper* fGlobalStepper;
   G4ChordFinder* fGlobalChordFinder;
   
   //Local fields
   std::vector<G4MagneticField*>  listOfLocalFields; 
   std::vector<G4String>  listOfLocalVolumes;
   
   //List of constructed fields
   std::vector<G4String> listOfCreatedFieldNames;
   std::vector<G4MagneticField*> listOfCreatedFields;
   
   //ParametrizedField
   
   std::vector<ParametrizedFieldCreator*> listOfParametrizedFieldCreators;
   
   
   MagFieldModManMessenger* theMessenger;
   TransformationManager* theTransformationManager;
   
   
   G4String UserSelectedStepperName;
  

   
   
};
#endif
