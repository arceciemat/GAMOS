///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef ConfinedFieldCreator_HH
#define ConfinedFieldCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIdirectory;
class G4MagneticField;
class G4AffineTransform;



class ConfinedFieldCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   ConfinedFieldCreator();
   ~ConfinedFieldCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* ConfinedFieldCmdDir;
   G4UIcmdWithADoubleAndUnit* SetXminCmd;
   G4UIcmdWithADoubleAndUnit* SetYminCmd;
   G4UIcmdWithADoubleAndUnit* SetZminCmd;
   G4UIcmdWithADoubleAndUnit* SetXmaxCmd;
   G4UIcmdWithADoubleAndUnit* SetYmaxCmd;
   G4UIcmdWithADoubleAndUnit* SetZmaxCmd;
   G4UIcmdWithAString* SetFieldCmd;

   double Xmin,Ymin,Zmin,Xmax,Ymax,Zmax;
   G4MagneticField* aField;


   
   
   
 
   
} ;

#endif
