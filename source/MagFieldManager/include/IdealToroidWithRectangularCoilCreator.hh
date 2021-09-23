///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef IdealToroidWithRectangularCoilCreator_HH
#define IdealToroidWithRectangularCoilCreator_HH 1


#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcommand;
class G4UIdirectory;
class G4MagneticField;
class G4AffineTransform;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

class IdealToroidWithRectangularCoilCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   IdealToroidWithRectangularCoilCreator();
   ~IdealToroidWithRectangularCoilCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* IdealToroidWithRectangularCoilCmdDir;
   
   G4UIcmdWithADoubleAndUnit* SetRToroidCmd;
   G4UIcmdWithADoubleAndUnit* SetRoutCornerCmd;
   G4UIcmdWithADoubleAndUnit* SetDcoilCmd;
   G4UIcmdWithADoubleAndUnit* SetLvertCmd;
   G4UIcmdWithADoubleAndUnit* SetLhorizCmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   
   G4double RToroid;
   G4double Dcoil;
   G4double Lvert;
   G4double Lhoriz;
   G4double RoutCorner;
   G4double I;
 
   
} ;

#endif
