///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef TransManMessenger_h
#define TransManMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class TransformationManager;
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

class TransManMessenger : public G4UImessenger
{public:
   TransManMessenger(TransformationManager* aTransManager);
  ~TransManMessenger();
  
   void SetNewValue(G4UIcommand * command,G4String newValues);
 
 private:
   TransformationManager* theTransManager;
   G4UIdirectory*  TransManagerDir;
   
   G4UIcmdWithAString* SelectTransformationCmd;
   G4UIcmdWithoutParameter* ResetToUnitaryCmd;
   G4UIcmdWithADoubleAndUnit* AddRotationAroundXCmd;
   G4UIcmdWithADoubleAndUnit* AddRotationAroundYCmd;
   G4UIcmdWithADoubleAndUnit* AddRotationAroundZCmd;
   G4UIcommand* AddRotationAroundAnAxisCmd;
   G4UIcmdWith3VectorAndUnit* AddTranslationCmd;
   
   G4UIcommand* MultiplyTransformationsCmd;
   G4UIcmdWithAString* MultiplyByCmd;
   G4UIcmdWithAString* SaveTransformationCmd;
   G4UIcmdWithoutParameter* ClearListOfTransformationsCmd; 
  
  
 
   //  commands
   
};
#endif
