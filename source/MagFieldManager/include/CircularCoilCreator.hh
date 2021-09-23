///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////

#ifndef CircularCoilCreator_HH
#define CircularCoilCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class CircularCoilCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   CircularCoilCreator();
   ~CircularCoilCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* CircularCoilCmdDir;
   G4UIcmdWithADoubleAndUnit* SetRadiusCmd;
   G4UIcmdWithADoubleAndUnit* SetDZCmd;
   G4UIcmdWithADoubleAndUnit* SetDRCmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   G4double Radius;
   G4double DR;
   G4double DZ;
   G4double I;
   

} ;

#endif
