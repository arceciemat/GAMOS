///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef MagFieldModManMessenger_h
#define MagFieldModManMessenger_h 1
#include "globals.hh"
#include "G4UImessenger.hh"

class MagneticFieldModelManager;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class MagFieldModManMessenger : public G4UImessenger
{public:
   MagFieldModManMessenger(MagneticFieldModelManager* aMagFieldModManager);
  ~MagFieldModManMessenger();
  
   void SetNewValue(G4UIcommand * command,G4String newValues);
 
 private:
   MagneticFieldModelManager* theMagFieldModManager;
   G4UIdirectory*  MagFieldModManagerDir;
   
   G4UIcommand* CreateAParametrizedFieldCmd;
   G4UIcmdWithAString* SetGlobalFieldCmd;
   G4UIcmdWithoutParameter* RemoveGlobalFieldCmd;

   
   G4UIcommand* ComputeJCurrentBetweenTwoPositionsCmd;
   G4UIcommand* ComputeJCurrentOnACartesianGridCmd;
   G4UIcommand* ComputeBfieldOnACartesianGridCmd;
   G4UIcommand* ComputeBfieldOnACylindricalGridCmd;
   
   
   
   G4UIcmdWithAnInteger* SetNxForBiotSavartIntegrationCmd;
   G4UIcmdWithAnInteger* SetNyForBiotSavartIntegrationCmd;
   G4UIcmdWithAnInteger* SetNphiForBiotSavartIntegrationCmd;
   
   //G4UIcmdWithAnInteger* SetLinearSegmentTypeOfIntegrationCmd;
   G4UIcmdWithAnInteger* SetCurvedSegmentTypeOfIntegrationCmd;
   
   
   //Definition of integration parameters for the Lorentz equation
   //--------------------------------------------------------------
   G4UIcmdWithADouble* SetEpsilonCmd;
   G4UIcmdWithADoubleAndUnit* SetDeltaChordCmd; 
   G4UIcmdWithADoubleAndUnit* SetDeltaIntersectionCmd;
   G4UIcmdWithADoubleAndUnit* SetLargestAcceptableStepCmd; 
   G4UIcmdWithAString* SetStepperCmd;
    
};
#endif
