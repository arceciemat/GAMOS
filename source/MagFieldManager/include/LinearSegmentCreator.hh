///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef LinearSegmentCreator_HH
#define LinearSegmentCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class LinearSegmentCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   LinearSegmentCreator();
   ~LinearSegmentCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* LinearSegmentCmdDir;
   G4UIcmdWithADoubleAndUnit* SetD1Cmd;
   G4UIcmdWithADoubleAndUnit* SetD2Cmd;
   G4UIcmdWithADoubleAndUnit* SetLCmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   G4double D1;
   G4double D2;
   G4double L;
   G4double I;
   
   

} ;

#endif
