///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef IdealToroidWithCircularCoilCreator_HH
#define IdealToroidWithCircularCoilCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcommand;
class G4UIdirectory;
class G4MagneticField;
class G4AffineTransform;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

class IdealToroidWithCircularCoilCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   IdealToroidWithCircularCoilCreator();
   ~IdealToroidWithCircularCoilCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* IdealToroidWithCircularCoilCmdDir;
   
   G4UIcmdWithADoubleAndUnit* SetRToroidCmd;
   G4UIcmdWithADoubleAndUnit* SetRoutCoilCmd;
   G4UIcmdWithADoubleAndUnit* SetDcoilCmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   
   G4double RToroid;
   G4double Dcoil;
   G4double RoutCoil;
   G4double I;
 
   
} ;

#endif
