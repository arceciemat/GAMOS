///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef InterpolatedFieldCreator_HH
#define InterpolatedFieldCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

class InterpolatedFieldCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   InterpolatedFieldCreator();
   ~InterpolatedFieldCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* InterpolatedFieldCmdDir;
   
  //commands
   G4UIcmdWithAString* SetGridFileNameCmd;

  //parameter
   G4String file_name;
  
   
   

} ;

#endif
