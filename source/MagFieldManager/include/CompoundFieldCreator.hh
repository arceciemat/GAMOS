///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CompoundFieldCreator_HH
#define CompoundFieldCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcommand;
class G4UIdirectory;
class G4MagneticField;
class G4AffineTransform;
class G4UIcmdWithoutParameter;


class CompoundFieldCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   CompoundFieldCreator();
   ~CompoundFieldCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* CompoundFieldCmdDir;
   G4UIcommand* AddFieldCmd;
   G4UIcmdWithoutParameter* ResetCmd;
   
   std::vector<G4MagneticField*> list_of_fields;
   std::vector<G4AffineTransform*> list_of_transformations;
   
   
   
 
   
} ;

#endif
