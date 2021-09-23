///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef MultiplyFieldCreator_HH
#define MultiplyFieldCreator_HH 1


#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class MultiplyFieldCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   MultiplyFieldCreator();
   ~MultiplyFieldCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* MultiplyFieldCmdDir;
   G4UIcommand* SetFieldAndFactorCmd;
   
   
   G4MagneticField* theField;
   G4double factor;
   
   

} ;

#endif
