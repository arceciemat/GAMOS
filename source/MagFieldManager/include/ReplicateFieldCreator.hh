///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef ReplicateFieldCreator_HH
#define ReplicateFieldCreator_HH 1


#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcommand;
class G4UIdirectory;
class G4MagneticField;
class G4AffineTransform;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class ReplicateFieldCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   ReplicateFieldCreator();
   ~ReplicateFieldCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* ReplicateFieldCmdDir;
   G4UIcmdWithAString* SelectFieldCmd;
   G4UIcmdWithADoubleAndUnit* SetRCmd;
   G4UIcmdWithADoubleAndUnit* SetPhi0Cmd;
   G4UIcmdWithADoubleAndUnit* SetdPhiCmd;
   G4UIcmdWithAnInteger*  SetNReplicateCmd;
   
   G4double phi0; 
   G4double dphi;
   size_t n_replicate;
   G4double r; 
   G4MagneticField* FieldToReplicate;
   
   
   
   
 
   
} ;

#endif
